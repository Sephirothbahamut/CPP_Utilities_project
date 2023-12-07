#pragma once

#include "../common.h"

namespace utils::input_system::device
	{
	namespace inputs
		{
		namespace details
			{
			template <typename DERIVED_T, input::concepts::input input_T, typename id_T>
			class base_crtp;

			namespace concepts
				{
				template <typename T>
				concept base = std::derived_from<T, details::base_crtp<typename T::derived_t, typename T::input_t, typename T::id_t>>;
				}

			template <typename T, typename id_T>
			struct state : input_system::state<T>
				{
				using id_t = id_T;
				state() = default;
				state(const id_t& id, const input_system::state<T>& input_state) : input_system::state<T>{input_state}, id{id} {}
				id_t id;
				};

			template <typename inputs_t>
			using evaluate_state_type = state<typename inputs_t::input_t::state_type::value_type, typename inputs_t::id_t>;

			template <concepts::base inputs_T>
			class event_raw_input : public event::details::base<evaluate_state_type<inputs_T>, event::details::root>, public utils::oop::non_copyable, public utils::oop::non_movable
				{
				template <typename DERIVED_T, input::concepts::input input_T, typename id_T>
				friend class base_crtp;
				using base = event::details::base<evaluate_state_type<inputs_T>, event::details::root>;
				public:
					using inputs_t   = inputs_T;
					using id_t       = typename inputs_t::id_t;
					using state_type = typename inputs_t::state_type;
					using callback_signature = typename base::callback_signature;

					event_raw_input(inputs_t& inputs) : inputs{inputs} { inputs.map(*this); };
					event_raw_input(inputs_t& inputs, std::function<callback_signature> callback) : inputs{inputs}, base{callback} 
						{
						inputs.map(*this);
						}
					~event_raw_input() { inputs.get().unmap(*this); }
			
					virtual on_completion operator()() noexcept final override
						{
						if (states_history.size() > 1)
							{
							for (size_t i{0}; i < states_history.size() - 1; i++)
								{
								this->callback(states_history[i]);
								}
							}
						assert(states_history.size() >= 1);
						auto last_state{states_history[states_history.size() - 1]};
						states_history.clear();
						auto result{this->callback(last_state)};
						if (result == on_completion::keep)
							{
							last_state.change(last_state.current);
							states_history.push_back(last_state);
							}
						return result;
						}

					void push_state(const state_type& state) noexcept { states_history.push_back(state); }

				private:
					std::reference_wrapper<inputs_t> inputs;
					std::vector<state_type> states_history;
				};

			template <typename DERIVED_T, input::concepts::input input_T, typename id_T>
			class base_crtp
				{
				public:
					using derived_t = DERIVED_T;
					using id_t      = id_T;
					using input_t   = input_T;
					using state_type = state<typename input_t::state_type::value_type, id_t>;
					using event_raw_input_t = event_raw_input<base_crtp<derived_t, input_t, id_t>>;

					constexpr const derived_t& derived() const noexcept { return static_cast<const derived_t&>(*this); }
					constexpr       derived_t& derived()       noexcept { return static_cast<      derived_t&>(*this); }

					const input_t& operator[](const id_t& id) const noexcept { return derived()[id]; }
						  input_t& operator[](const id_t& id)       noexcept { return derived()[id]; }

					void change(manager& manager, const id_t& id, const typename input_t::state_type::value_type& new_value) noexcept
						{
						auto& input{operator[](id)};
						input.change(manager, new_value);
						for (const auto& event : events)
							{
							typename event_raw_input_t::state_type state(id, input.state());
							event.get().push_state(state);
							manager.insert(event);
							}
						}
					
					void map  (event_raw_input_t& event) noexcept { events.insert(event); }
					void unmap(event_raw_input_t& event) noexcept { events.erase (event); }

					event_raw_input_t emplace_event(std::function<typename event_raw_input_t::callback_signature> callback)
						{
						return {*this, callback};
						}
					event_raw_input_t bind_debug_callback()
						{
						return emplace_event([](const state_type& state)
							{
							std::cout << utils::enums::enum_name<id_t>(state.id) << " ";
							if (state.changed()) { std::cout << "(changed)"; }
							else { std::cout << "(unchanged)"; }
							std::cout << " " << state.previous << " > " << state.current << std::endl;
							return utils::input_system::on_completion::remove;
							});
						}

				private:
					using unordered_set_t = input_system::details::reference_wrapper_utils<event_raw_input_t>::unordered_set;
					unordered_set_t events;
				};
			}

		template <input::concepts::input input_T, size_t SIZE>
		class static_array : public details::base_crtp<static_array<input_T, SIZE>, input_T, size_t>
			{
			public:
				using id_t    = size_t;
				using input_t = input_T;
					
				inline static constexpr const size_t count{SIZE};
		
						input_t& operator[](id_t id) utils_if_release(noexcept)
					{
					if constexpr (utils::compilation::debug) { return at(id); }
					else { return array[id]; }
					}
				const input_t& operator[](id_t id) const utils_if_release(noexcept)
					{
					if constexpr (utils::compilation::debug) { return at(id); }
					else { return array[id]; }
					}
				      input_t& at(id_t id)       { return array.at(static_cast<size_t>(id)); }
				const input_t& at(id_t id) const { return array.at(static_cast<size_t>(id)); }

			private:
				std::array<input_t, SIZE> array;
			};

		template <input::concepts::input input_T, typename id_enum>
		class static_enum : public details::base_crtp<static_enum<input_T, id_enum>, input_T, id_enum>
			{
			public:
				using id_t    = id_enum;
				using input_t = input_T;
				inline static constexpr const size_t count{utils::enums::enum_count<id_enum>()};
		
						input_t& operator[](id_t id) utils_if_release(noexcept)
					{
					if constexpr (utils::compilation::debug) { return at(id); }
					else { return array[static_cast<size_t>(id)]; }
					}
				const input_t& operator[](id_t id) const utils_if_release(noexcept)
					{
					if constexpr (utils::compilation::debug) { return at(id); }
					else { return array[static_cast<size_t>(id)]; }
					}
				      input_t& at(id_t id)       { return array.at(static_cast<size_t>(id)); }
				const input_t& at(id_t id) const { return array.at(static_cast<size_t>(id)); }
		
			private:
				std::array<input_t, count> array;
			};

		template <input::concepts::input input_T, typename id_T>
		class dynamic : public details::base_crtp<dynamic<input_T, id_T>, input_T, id_T>
			{
			public:
				using id_t    = id_T;
				using input_t = input_T;

				      input_t& operator[](id_t id)       noexcept { return container[id]; }
				const input_t& operator[](id_t id) const noexcept { return container[id]; }

			private:
				std::unordered_map<id_t, input_t> container;
			};

		template <input::concepts::input input_T>
		struct none { using id_t = size_t; using input_t = input_T; };

		namespace concepts
			{
			template <typename T>
			concept none = std::same_as<T, inputs::none<typename T::input_t>>;
			template <typename T>
			concept inputs = none<T> || details::concepts::base<T>;
			template <typename T>
			concept digital = none<T> || (inputs<T> && std::same_as<typename T::input_t, input::digital>);
			template <typename T>
			concept analog  = none<T> || (inputs<T> && std::same_as<typename T::input_t, input::analog >);
			}
		}

	template <inputs::concepts::digital digital_T, inputs::concepts::analog analog_T>
	struct base
		{
		using digital_t = digital_T;
		using analog_t  = analog_T ;
		inline static constexpr bool different_id_t{!std::same_as<typename digital_t::id_t, typename analog_t::id_t>};

		uintptr_t id;

		digital_t digital;
		analog_t  analog ;

		void change(manager& manager, const typename digital_t::id_t& digital_id, const typename digital_t::input_t::state_type::value_type& new_value) noexcept
			requires different_id_t && !inputs::concepts::none<digital_t>
			{
			change_digital(manager, digital_id, new_value);
			}
		void change(manager& manager, const typename analog_t ::id_t& analog_id , const typename analog_t ::input_t::state_type::value_type& new_value) noexcept
			requires different_id_t && !inputs::concepts::none<analog_t >
			{
			change_analog (manager, analog_id, new_value);
			}

		void change_digital(manager& manager, const typename digital_t::id_t& id, const typename digital_t::input_t::state_type::value_type& new_value) noexcept
			requires !inputs::concepts::none<digital_t>
			{
			digital.change(manager, id, new_value);
			}

		void change_analog (manager& manager, const typename analog_t ::id_t& id , const typename analog_t ::input_t::state_type::value_type& new_value) noexcept
			requires !inputs::concepts::none<analog_t >
			{
			analog .change(manager, id, new_value);
			}

		class debug_callbacks
			{
			friend struct base;
			private:
				debug_callbacks(base& device) requires(!inputs::concepts::none<digital_t> && !inputs::concepts::none<analog_t >):
					event_digital{device.digital.bind_debug_callback()},
					event_analog {device.analog .bind_debug_callback()}
					{}
				std::conditional_t<inputs::concepts::none<digital_t>, void, typename digital_t::event_raw_input_t> event_digital;
				std::conditional_t<inputs::concepts::none<analog_t >, void, typename analog_t ::event_raw_input_t> event_analog ;
			};
		
		debug_callbacks bind_debug_callbacks() noexcept { return debug_callbacks{*this}; }
		};
	}

namespace utils::input_system::event
	{
	template <device::inputs::details::concepts::base inputs_t>
	using raw_input = device::inputs::details::event_raw_input<inputs_t>;
	}