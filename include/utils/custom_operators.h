#pragma once

namespace utils
	{
	template <typename ret_t, typename lhs_t, typename rhs_t, ret_t(*func)(const lhs_t&, const rhs_t&)>
	struct custom_operator
		{
		class _inner;

		inline constexpr friend _inner operator-(const lhs_t& lhs, const custom_operator& proxy) noexcept { return {lhs}; }

		class _inner
			{
			public:
				constexpr ret_t operator-(const rhs_t& rhs) const noexcept { return func(lhs, rhs); }
				constexpr _inner(const lhs_t& lhs) noexcept : lhs{lhs} {}
			private:
				const lhs_t& lhs;
			};
		};
	}