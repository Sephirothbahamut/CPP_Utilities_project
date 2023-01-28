#pragma once

#include <concepts>
#include <ranges>

#include "common.h"
#include "../../concepts.h"

namespace utils::details::vec
	{
	namespace details 
		{
		template <typename T, std::size_t... Seq, typename... Args>
		constexpr std::array<T, sizeof...(Seq)> make_array_inner(std::index_sequence<Seq...>, Args &... args)
			{
			return {{(static_cast<void>(Seq), T(Seq, args...))...}};
			}

		template <typename T, std::size_t N, typename... Args>
		constexpr std::array<T, N> make_array(Args &... args)
			{
			return make_array_inner<T>(std::make_index_sequence<N>(), args...);
			}
		} 


	template<typename T, size_t SIZE, typename DERIVED_T>
	class factories
		{
		public:
			using derived_t = DERIVED_T;

		private:
			constexpr const derived_t& derived() const noexcept { return static_cast<const derived_t&>(*this); }
			constexpr       derived_t& derived()       noexcept { return static_cast<      derived_t&>(*this); }
			constexpr const auto     & get_arr() const noexcept { return derived().array; }
			constexpr       auto     & get_arr()       noexcept { return derived().array; }

			using arr_t = std::array<T, SIZE>;

		public:
			inline static constexpr const size_t static_size{std::tuple_size_v<arr_t>};
			using value_type = typename arr_t::value_type;

			inline static constexpr derived_t zero() noexcept
				{
				derived_t ret{.array{details::make_array<value_type, static_size>(0)}};
				return ret;
				}
			
			template<std::convertible_to<value_type> ...Args>
				requires(sizeof...(Args) == static_size)
			inline static constexpr derived_t from(Args&& ...args) noexcept
				{
				return derived_t{.array{std::forward<Args>(args)...}};
				}

			/// <summary> If there are less parameters, the last one is used as filler for the rest of the array </summary>
			template<std::convertible_to<value_type> ...Args>
				requires(sizeof...(Args) < static_size && utils::concepts::default_constructible<value_type>)
			inline static constexpr derived_t from(Args&& ...args) noexcept
				{
				derived_t ret{.array{std::forward<Args>(args)...}};

				for (size_t i = sizeof...(Args); i < static_size; i++)
					{
					ret[i] = ret[sizeof...(Args) - 1];
					}

				return ret;
				}

			template<std::ranges::random_access_range T2>
				requires(std::convertible_to<value_type, typename T2::value_type> && utils::concepts::default_constructible<value_type>)
			inline static constexpr derived_t from(const T2& source, value_type default_value = 0) noexcept
				{
				derived_t ret;

				size_t i = 0;
				for (; i < std::min(static_size, source.size()); i++) { ret[i] = source[i]; }
				for (; i < static_size; i++) { ret[i] = default_value; }

				return ret;
				}
		};
	}