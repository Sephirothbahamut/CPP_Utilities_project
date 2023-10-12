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
	struct state_base
		{
		using value_type = T;
		T current;
		T previous;
		void change(const T& new_value) noexcept { previous = current; current = new_value; }
		bool changed() const noexcept { return current != previous; }
		};
	
	namespace event::details
		{
		struct root
			{
			virtual void update() const noexcept = 0;
			virtual on_completion operator()() const noexcept = 0;
			};
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
			virtual event::details::root& get_event() noexcept = 0;
			};

		//mappings are branches. The last branch's parent is an event.
		struct branch : node
			{
			branch(node& parent) : parent{parent} {}

			virtual event::details::root& get_event() noexcept final override
				{
				return parent.get().get_event();
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
				auto it{callbacks_keep_set.begin()};
				while (it != callbacks_keep_set.end())
					{
					const auto& callback{*it};
					if (callback() == on_completion::remove)
						{
						it = callbacks_keep_set.erase(it);
						}
					else { it++; }
					}

				while (!callbacks_set.empty())
					{
					auto callback{std::move(callbacks_set.extract(callbacks_set.begin()).value())};
					if (callback() == on_completion::keep)
						{
						callbacks_keep_set.insert(std::move(callback));
						}
					}
				}

			void insert(event::details::root& event) noexcept 
				{
				auto it{callbacks_keep_set.find(event)};
				if (it != callbacks_keep_set.end()) { event.update(); }
				else 
					{
					auto result{callbacks_set.insert(event)};
					if (result.second) { event.update(); } //update first time inserted
					}
				}

		private:
			details::events_observer_set callbacks_set;
			details::events_observer_set callbacks_keep_set;
		};

	namespace input
		{
		template <typename T>
		struct base : utils::oop::non_copyable, utils::oop::non_movable
			{
			public:
				using state_type = state_base<T>;
				using value_type = typename state_base<T>::value_type;
				const state_type& value() const noexcept { return _value; }
				      state_type& value()       noexcept { return _value; }

				void change(manager& manager, const value_type& new_value) noexcept
					{
					_value.change(new_value);
					for (auto node : nodes) { manager.insert(node.get().get_event()); }
					}

				void map  (details::node& node) noexcept { nodes.insert(node); }
				void unmap(details::node& node) noexcept { nodes.erase (node); }

			private:
				state_type _value;
				details::nodes nodes;
			};
		using analog  = base<float>;
		using digital = base<bool >;

		namespace concepts
			{
			template <typename T>
			concept input = std::derived_from<T, base<typename T::value_type>>;
			}
		}

	namespace mapping
		{
		template <typename T>
		struct base : details::branch
			{
			using branch::branch;
			using state_type = state_base<T>;
			using value_type = typename state_base<T>::value_type;
			virtual state_type value() const noexcept = 0;
			};

		namespace button
			{
			using base = mapping::base<bool>;

			struct device_input : base
				{
				std::reference_wrapper<input::digital> input;

				device_input(details::node& parent, input::digital& input) : base{parent}, input{input}
					{
					input.map(*this);
					}
				~device_input() { input.get().unmap(*this); }

				virtual state_type value() const noexcept final override
					{
					return input.get().value();
					};
				};
			}

		namespace axis1d
			{
			using base = mapping::base<float>;

			struct device_input : base
				{
				std::reference_wrapper<input::analog> input;

				device_input(details::node& parent, input::analog& input) : base{parent}, input{input}
					{
					input.map(*this);
					}
				~device_input() { input.get().unmap(*this); }

				virtual state_type value() const noexcept final override
					{
					return input.get().value();
					};
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

				virtual state_type value() const noexcept final override
					{
					return 
						{
						axis_own->value().current  > threshold,
						axis_own->value().previous > threshold
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
				
				virtual state_type value() const noexcept final override
					{
					return
						{
						static_cast<float>(b_own->value().current  - a_own->value().current ),
						static_cast<float>(b_own->value().previous - a_own->value().previous),
						};
					};
				};
			}
		
		namespace axis2d
			{
			using base = mapping::base<utils::math::vec2f>;
		
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

				virtual state_type value() const noexcept final override
					{
					return
						{
						::utils::math::vec2f{x_own->value().current , y_own->value().current },
						::utils::math::vec2f{x_own->value().previous, y_own->value().previous},
						};
					};
				};
			}
		}

	namespace event
		{
		template <typename T>
		class mapped : public details::root, public input_system::details::node
			{
			public:
				using state_type = state_base<T>;
				using value_type = typename state_base<T>::value_type;
				using callback_signature = on_completion(const state_type& state);

				mapped() = default;
				mapped(std::function<callback_signature> callback) : callback{callback} {}

				std::function<callback_signature> callback;
				std::unique_ptr<mapping::base<T>> mapping;

				state_type value() const noexcept { return mapping->value(); }

				template <std::derived_from<mapping::base<T>> T, typename ...Args>
				T& emplace_mapping(Args&& ...args) noexcept
					{
					std::unique_ptr<T> created{std::make_unique<T>(*this, std::forward<Args>(args)...)};
					utils::observer_ptr<T> created_obs{created.get()};
					mapping = std::move(created);
					return *created_obs;
					}

				virtual void update() const noexcept final override { state = value(); }
				virtual on_completion operator()() const noexcept 
					{
					auto result{callback(state)};
					if (result == on_completion::keep) { state.change(state.current); }
					return result;
					}

			private:
				mutable state_type state;

				virtual event::details::root& get_event() noexcept final override { return *this; }
			};
		}
	}
