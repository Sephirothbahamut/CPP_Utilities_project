#pragma once

#include "../common.h"

namespace utils::input_system::device
	{
	namespace inputs
		{
		template <typename DERIVED_T, input::concepts::input input_T, typename id_T>
		struct base_crtp
			{
			using derived_t = DERIVED_T;
			using id_t      = id_T;
			using input_t   = input_T;

			constexpr const derived_t& derived() const noexcept { return static_cast<const derived_t&>(*this); }
			constexpr       derived_t& derived()       noexcept { return static_cast<      derived_t&>(*this); }

			const input_t& operator[](const id_t& id) const noexcept { return derived()[id]; }
				    input_t& operator[](const id_t& id)       noexcept { return derived()[id]; }

			void change(manager& manager, const id_t& id, const typename input_t::value_type& new_value) noexcept
				{
				operator[](id).change(manager, new_value);
				}
			};

		template <input::concepts::input input_T, size_t SIZE>
		class static_array : public base_crtp<static_array<input_T, SIZE>, input_T, size_t>
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
		class static_enum : public base_crtp<static_enum<input_T, id_enum>, input_T, id_enum>
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
		class dynamic : public base_crtp<dynamic<input_T, id_T>, input_T, id_T>
			{
			public:
				using id_t    = id_T;
				using input_t = input_T;

						input_t& operator[](id_t id)       noexcept { return container[id]; }
				const input_t& operator[](id_t id) const noexcept { return container[id]; }

			private:
				std::unordered_map<id_t, input_t> container;
			};

		namespace concepts
			{
			template <typename T>
			concept inputs = std::derived_from<T, base_crtp<typename T::derived_t, typename T::input_t, typename T::id_t>>;
			template <typename T>
			concept digital = inputs<T> && std::same_as<typename T::input_t, input::digital>;
			template <typename T>
			concept analog  = inputs<T> && std::same_as<typename T::input_t, input::analog >;
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

		void change(manager& manager, const typename digital_t::id_t& digital_id, const typename digital_t::input_t::value_type& new_value) noexcept
			requires different_id_t
			{
			change_digital(manager, digital_id, new_value);
			}
		void change(manager& manager, const typename analog_t ::id_t& analog_id , const typename analog_t ::input_t::value_type& new_value) noexcept
			requires different_id_t
			{
			change_analog (manager, analog_id, new_value);
			}

		void change_digital(manager& manager, const typename digital_t::id_t& id, const typename digital_t::input_t::value_type& new_value) noexcept
			{
			digital.change(manager, id, new_value);
			}
		void change_analog (manager& manager, const typename analog_t ::id_t& id , const typename analog_t ::input_t::value_type& new_value) noexcept
			{
			analog .change(manager, id, new_value);
			}
		};
	}