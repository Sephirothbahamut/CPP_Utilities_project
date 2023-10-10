#pragma once

#include <array>
#include <vector>
#include <cstdint>
#include <functional>
#include <unordered_set>
#include <unordered_map>

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
			};

		struct callback_wrapper
			{
			void* identifier;
			bool operator== (const callback_wrapper& other) const noexcept { return identifier ==  other.identifier; }
			auto operator<=>(const callback_wrapper& other) const noexcept { return identifier <=> other.identifier; }
			on_completion operator()() const noexcept { return callback(); }
			std::function<on_completion()> callback;

			struct hash_function
				{
				size_t operator()(const callback_wrapper& cw) const { return std::hash<void*>{}(cw.identifier); }
				};
			};
		using callbacks_wrappers = std::unordered_set<callback_wrapper, callback_wrapper::hash_function>;

		//mappings and events are nodes. Events are the roots of the tree
		struct node : utils::oop::non_copyable, utils::oop::non_movable 
			{
			virtual callback_wrapper get_callback_wrapped() noexcept = 0; 
			};

		//mappings are branches. The last branch's parent is an event.
		struct branch : node
			{
			branch(node& parent) : parent{parent} {}

			virtual details::callback_wrapper get_callback_wrapped() noexcept final override 
				{
				return parent.get().get_callback_wrapped(); 
				}

			std::reference_wrapper<node> parent;
			};

		using nodes = std::unordered_set
			<
			std::reference_wrapper<branch>,
			reference_wrapper_utils<branch>::address_hash,
			reference_wrapper_utils<branch>::address_equality
			>;
		}

	class manager
		{
		public:
			void step() noexcept
				{
				auto it{callbacks.begin()};
				while (it != callbacks.end())
					{
					const auto& callback{*it};
					if (callback() == on_completion::remove)
						{
						it = callbacks.erase(it);
						}
					else { it++; }
					}
				}

			void insert(const details::callback_wrapper& callback_wrapper) noexcept { callbacks.insert(callback_wrapper); }

		private:
			details::callbacks_wrappers callbacks;
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

				details::nodes nodes;
				void change(manager& manager, const value_type& new_value) noexcept
					{
					_value.change(new_value);
					for (auto node : nodes) { manager.insert(node.get().get_callback_wrapped()); }
					}

				void map  (details::nodes& node) noexcept { nodes.insert(node); }
				void unmap(details::nodes& node) noexcept { nodes.erase (node); }

			private:
				state_type _value;
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
					input.nodes.insert(*this);
					}
				~device_input() { input.get().nodes.erase(*this); }

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
					input.nodes.insert(*this);
					}
				~device_input() { input.get().nodes.erase(*this); }

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
		
	template <typename T>
	class event : public details::node
		{
		public:
			using state_type = state_base<T>;
			using value_type = typename state_base<T>::value_type;

			event() = default;
			event(std::function<on_completion(const state_type& state)> callback) : callback{callback} {}

			using state_type = state_base<T>;
			using value_type = typename state_base<T>::value_type;

			std::unique_ptr<mapping::base<T>> mapping;
			std::function<on_completion(const state_type& state)> callback;

			template <std::derived_from<mapping::base<T>> T, typename ...Args>
			T& emplace_mapping(Args&& ...args) noexcept
				{
				std::unique_ptr<T> created{std::make_unique<T>(*this, std::forward<Args>(args)...)};
				utils::observer_ptr<T> created_obs{created.get()};
				mapping = std::move(created);
				return *created_obs;
				}

		private:
			state_type state;
			virtual details::callback_wrapper get_callback_wrapped() noexcept final override
				{
				return {reinterpret_cast<void*>(this), [this]()
					{
					state = mapping->value();
					auto result{callback(state)};
					if (result == on_completion::keep) { state.change(state.current); }
					return result;
					}};
				};
		};
	}
