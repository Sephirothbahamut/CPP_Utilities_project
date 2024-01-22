#pragma once

#include "math.h"

namespace utils::math
	{
	template <concepts::number T, T MIN, T MAX>
	class ranged
		{
		public:
			using value_type = T;
			inline static constexpr value_type min{MIN};
			inline static constexpr value_type max{MAX};

			utils_gpu_available ranged(const value_type& value = value_type{0}) noexcept : _value{ensure_value(value)} {}
			utils_gpu_available ranged& operator=(const value_type& value) noexcept { set_value(value); }

			utils_gpu_available const value_type& get_value() const noexcept { return _value; }

			utils_gpu_available ranged& set_value(const value_type& value) noexcept
				{
				_value = ensure_value(value);
				return *this;
				}

			__declspec(property(get = get_value, put = set_value)) value_type value;

			utils_gpu_available operator value_type() const noexcept { return get_value(); }

		private:
			value_type _value;

			utils_gpu_available value_type ensure_value(const value_type& value) { return std::clamp(value, MIN, MAX); }
		};
	}