#pragma once

#include <array>
#include <vector>
#include <cstdint>
#include <functional>
#include <unordered_set>
#include <unordered_map>

#include "../hash.h"
#include "../enum.h"
#include "../oop/disable_move_copy.h"
#include "../memory.h"
#include "../math/vec2.h"
#include "../math/vec3.h"
#include "../containers/object_pool.h"

namespace utils::input_system
	{
	enum class on_completion { remove, keep };

	template <typename T>
	struct state
		{
		using value_type = T;
		T current;
		T previous;
		void change(const T& new_value) noexcept { previous = current; current = new_value; }
		bool changed() const noexcept { return current != previous; }
		};
	
	namespace concepts
		{
		template <typename T>
		concept state = std::derived_from<T, input_system::state<typename T::value_type>>;
		}

	namespace event
		{
		namespace details
			{
			struct root
				{
				virtual on_completion operator()() noexcept = 0;
				};
			struct mapped_root : root
				{
				virtual void push_source(utils::observer_ptr<void> source) noexcept = 0;
				};

			template <concepts::state STATE_TYPE, typename root_t>
			struct base : root_t
				{
				using state_type = STATE_TYPE;
				using callback_signature = on_completion(const state_type& state);

				base() = default;
				base(std::function<callback_signature> callback) : callback{callback} {}

				std::function<callback_signature> callback;
				};
			}
		template <typename T>
		class mapped;
		}

	namespace details
		{
		template <typename T>
		struct reference_wrapper_utils
			{
			struct address_hash
				{
				size_t operator()(const std::reference_wrapper<T>& rw) const { return std::hash<T*>{}(&rw.get()); }
				};
			struct address_equality
				{
				size_t operator()(const std::reference_wrapper<T>& a, const std::reference_wrapper<T>& b) const
					{
					return (&a.get()) == (&b.get());
					}
				};
			
			using unordered_set = std::unordered_set
				<
				std::reference_wrapper<T>,
				address_hash,
				address_equality
				>;
			};

		using events_observer_set = reference_wrapper_utils<event::details::root>::unordered_set;

		//mappings and events are nodes. Events are the roots of the tree
		struct node : utils::oop::non_copyable, utils::oop::non_movable 
			{
			virtual event::details::mapped_root& event() noexcept = 0;
			};

		//mappings are branches. The last branch's parent is an event.
		struct branch : node
			{
			branch(node& parent) : parent{parent} {}

			virtual event::details::mapped_root& event() noexcept final override
				{
				return parent.get().event();
				}

			std::reference_wrapper<node> parent;
			};

		using nodes = reference_wrapper_utils<node>::unordered_set;
		}

	class manager
		{
		public:
			void step() noexcept 
				{
				auto it{callbacks_set.begin()};
				while (it != callbacks_set.end())
					{
					const auto& callback{*it};
					if (callback() == on_completion::remove)
						{
						it = callbacks_set.erase(it);
						}
					else { it++; }
					}
				}

			void insert(event::details::root& event) noexcept { callbacks_set.insert(event); }

		private:
			details::events_observer_set callbacks_set;
		};

	namespace input
		{
		namespace details
			{
			template <typename T>
			struct base : utils::oop::non_copyable, utils::oop::non_movable
				{
				public:
					using state_type = input_system::state<T>;
					const state_type& state() const noexcept { return _state; }
						  state_type& state()       noexcept { return _state; }

					void change(manager& manager, const typename state_type::value_type& new_value) noexcept
						{
						for (auto node : nodes) 
							{
							auto& event{node.get().event()};
							event.push_source(this);
							manager.insert(event); 
							}
						//change state AFTER event.push_source, important
						_state.change(new_value); 
						}

					void map  (input_system::details::node& node) noexcept { nodes.insert(node); }
					void unmap(input_system::details::node& node) noexcept { nodes.erase (node); }

				private:
					state_type _state;
					input_system::details::nodes nodes;
				};
			}
		using analog  = details::base<float>;
		using digital = details::base<bool >;

		namespace concepts
			{
			template <typename T>
			concept input = std::derived_from<T, details::base<typename T::state_type::value_type>>;
			}
		}

	namespace mapping
		{
		namespace details
			{
			template <typename T>
			struct base : input_system::details::branch
				{
				using input_system::details::branch::branch;
				using state_type = input_system::state<T>;
				virtual state_type state() const noexcept = 0;
				};
			}

		namespace button
			{
			using base = details::base<bool>;

			struct device_input : base
				{
				std::reference_wrapper<input::digital> input;

				device_input(input_system::details::node& parent, input::digital& input) : base{parent}, input{input}
					{
					input.map(*this);
					}
				~device_input() { input.get().unmap(*this); }

				virtual state_type state() const noexcept final override { return input.get().state(); };
				};
			}

		namespace axis1d
			{
			using base = details::base<float>;

			struct device_input : base
				{
				std::reference_wrapper<input::analog> input;

				device_input(input_system::details::node& parent, input::analog& input) : base{parent}, input{input}
					{
					input.map(*this);
					}
				~device_input() { input.get().unmap(*this); }

				virtual state_type state() const noexcept final override { return input.get().state(); };
				};
			}

		namespace button
			{
			struct axis_threshold : base
				{
				using base::base;

				float threshold{.5f};
				std::unique_ptr<axis1d::base> axis_own;

				template <std::derived_from<axis1d::base> T, typename ...Args>
				T& emplace_axis(Args&& ...args) noexcept
					{
					std::unique_ptr<T> created{std::make_unique<T>(*this, std::forward<Args>(args)...)};
					utils::observer_ptr<T> created_obs{created.get()};
					axis_own = std::move(created);
					return *created_obs;
					}

				virtual state_type state() const noexcept final override
					{
					return 
						{
						axis_own->state().current  > threshold,
						axis_own->state().previous > threshold
						};
					}
				};
			};

		namespace axis1d
			{
			struct b_minus_a : base
				{
				using base::base;

				std::unique_ptr<axis1d::base> a_own;
				std::unique_ptr<axis1d::base> b_own;

				template <std::derived_from<axis1d::base> T, typename ...Args>
				T& emplace_a(Args&& ...args) noexcept
					{
					std::unique_ptr<T> created{std::make_unique<T>(*this, std::forward<Args>(args)...)};
					utils::observer_ptr<T> created_obs{created.get()};
					a_own = std::move(created);
					return *created_obs;
					}
				template <std::derived_from<axis1d::base> T, typename ...Args>
				T& emplace_b(Args&& ...args) noexcept
					{
					std::unique_ptr<T> created{std::make_unique<T>(*this, std::forward<Args>(args)...)};
					utils::observer_ptr<T> created_obs{created.get()};
					b_own = std::move(created);
					return *created_obs;
					}
				
				virtual state_type state() const noexcept final override
					{
					return
						{
						static_cast<float>(b_own->state().current  - a_own->state().current ),
						static_cast<float>(b_own->state().previous - a_own->state().previous),
						};
					};
				};
			}
		
		namespace axis2d
			{
			using base = details::base<utils::math::vec2f>;
		
			struct from_axes : base
				{
				using base::base;

				std::unique_ptr<axis1d::base> x_own;
				std::unique_ptr<axis1d::base> y_own;

				template <std::derived_from<axis1d::base> T, typename ...Args>
				T& emplace_x(Args&& ...args) noexcept
					{
					std::unique_ptr<T> created{std::make_unique<T>(*this, std::forward<Args>(args)...)};
					utils::observer_ptr<T> created_obs{created.get()};
					x_own = std::move(created);
					return *created_obs;
					}
				template <std::derived_from<axis1d::base> T, typename ...Args>
				T& emplace_y(Args&& ...args) noexcept
					{
					std::unique_ptr<T> created{std::make_unique<T>(*this, std::forward<Args>(args)...)};
					utils::observer_ptr<T> created_obs{created.get()};
					y_own = std::move(created);
					return *created_obs;
					}

				virtual state_type state() const noexcept final override
					{
					return
						{
						::utils::math::vec2f{x_own->state().current , y_own->state().current },
						::utils::math::vec2f{x_own->state().previous, y_own->state().previous},
						};
					};
				};
			}
		}

	namespace event
		{
		template <typename T>
		class mapped : public details::base<input_system::state<T>, details::mapped_root>, public input_system::details::node
			{
			friend class input::details::base<T>;
			using base = details::base<input_system::state<T>, details::mapped_root>;
			public:
				using state_type         = typename base::state_type;
				using callback_signature = typename base::callback_signature;

				mapped() = default;
				mapped(std::function<callback_signature> callback) : base{callback} {}
				std::unique_ptr<mapping::details::base<T>> mapping;

				state_type state() const noexcept { return mapping->state(); }
				virtual event::details::mapped_root& event() noexcept final override { return *this; }

				template <std::derived_from<mapping::details::base<T>> T, typename ...Args>
				T& emplace_mapping(Args&& ...args) noexcept
					{
					std::unique_ptr<T> created{std::make_unique<T>(*this, std::forward<Args>(args)...)};
					utils::observer_ptr<T> created_obs{created.get()};
					mapping = std::move(created);
					return *created_obs;
					}

				virtual on_completion operator()() noexcept final override
					{
					if (!current_state_sources.empty())  //if not empty, there's a pending in this step
						{
						for (const auto& state : states_history)
							{
							this->callback(state);
							}
						states_history.clear();
						current_state_sources.clear();

						//NOTE last history state wasn't added to states_history yet.
						//also only last state's return value matters for keeping
						auto last_state{state()};
						auto result{this->callback(last_state)};
						if (result == on_completion::keep)
							{
							last_state.change(last_state.current); //make last_state's current be equal to its previous
							states_history.push_back(last_state);
							}
						return result;
						}
					else //if empty, we're calling an event which return was "keep" in previous frames
						{
						assert(states_history.size() == 1);

						auto result{this->callback(states_history[0])};
						if (result == on_completion::remove) { states_history.clear(); }
						return result;
						}
					}

			private:
				virtual void push_source(utils::observer_ptr<void> source) noexcept final override
					{
					push_source_inner(source);
					}

				std::vector<state_type> states_history;
				std::vector<utils::observer_ptr<void>> current_state_sources;

				void push_source_inner(utils::observer_ptr<void> source) noexcept
					{
					auto it{std::find(current_state_sources.begin(), current_state_sources.end(), source)};
					if (it != current_state_sources.end())
						{//source already exists, push current state and add as new source
						states_history.push_back(state());
						current_state_sources.clear();
						}
					current_state_sources.push_back(source);
					}
			};
		}
	}
