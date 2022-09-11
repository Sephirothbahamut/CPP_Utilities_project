#pragma once
#include <limits>
#include <type_traits>
#include <algorithm>

namespace utils
	{
	template <typename T, T MIN = std::numeric_limits<T>::min(), T MAX = std::numeric_limits<T>::max()>
	class bounded
		{
		private:
			T value;

			inline T get() const noexcept { return value; };
			inline void set(const T& value) noexcept { this->value = value; };
			inline static void verify(T value) { if (value <= MIN || value >= MAX) { throw "Bounded variable was assigned values out of bounds."; } }

		public:
			bounded() = default;
			bounded(T value) : value(value) { verify(value); };
			inline static T clamp(T value) { return std::clamp(value, MIN, MAX); }
			
			bounded  operator+ (const bounded& oth) const noexcept { return {value +  oth.value}; };
			bounded  operator- (const bounded& oth) const noexcept { return {value -  oth.value}; };
			bounded  operator* (const bounded& oth) const noexcept { return {value *  oth.value}; };
			bounded  operator/ (const bounded& oth) const noexcept { return {value /  oth.value}; };
			bounded  operator||(const bounded& oth) const noexcept { return {value || oth.value}; };
			bounded  operator&&(const bounded& oth) const noexcept { return {value && oth.value}; };
			bounded  operator< (const bounded& oth) const noexcept { return {value <  oth.value}; };
			bounded  operator> (const bounded& oth) const noexcept { return {value >  oth.value}; };
			bounded  operator==(const bounded& oth) const noexcept { return {value == oth.value}; };
			bounded  operator<=(const bounded& oth) const noexcept { return {value <= oth.value}; };
			bounded  operator>=(const bounded& oth) const noexcept { return {value >= oth.value}; };
			
			bounded& operator+=(const bounded& oth) noexcept { return *this = *this + oth; };
			bounded& operator-=(const bounded& oth) noexcept { return *this = *this - oth; };
			bounded& operator*=(const bounded& oth) noexcept { return *this = *this * oth; };
			bounded& operator/=(const bounded& oth) noexcept { return *this = *this / oth; };

			bounded& operator= (const bounded& oth) noexcept { value = oth.value; return *this; };
			bounded& operator= (const T& oth) noexcept { verify(oth); value = oth; return *this; };
		};
	}

namespace std
	{
	template <typename T, T MIN, T MAX>
	class numeric_limits<utils::bounded<T, MIN, MAX>>
		{
		public:
			static utils::bounded<T, MIN, MAX> min() { return {MIN}; };
			static utils::bounded<T, MIN, MAX> max() { return {MAX}; };
			static utils::bounded<T, MIN, MAX> epsilon() { return std::numeric_limits<T>::epsilon(); };
		};
	}