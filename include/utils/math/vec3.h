#pragma once
#include <ostream>
#include <cmath>
#include <numbers>
#include <algorithm>

#include "../custom_operators.h"
#include "math.h"

#include "angle.h"

#ifdef COUT_CONTAINERS
#include "../cout_utilities.h"
#endif

namespace utils::math
	{
	template <typename T> class vec2;
	}

#ifdef COUT_CONTAINERS
namespace utils::cout
	{
	template <typename T>
	inline std::ostream& operator<<(std::ostream& os, const utils::math::vec2<T>& vec) noexcept
		{
		namespace ccu = cout::support;
		return os << ccu::type << "vec2" << ccu::brace << "(" << ccu::value << vec.x << ccu::separ << ", " << ccu::value << vec.y << ccu::brace << ")";
		}
	}
#endif

namespace utils::math
	{
	template <template<typename> class To, typename T, template<typename> class From, typename F>
	inline static To<T> vec_cast(From<F> from) { return {static_cast<T>(from.x), static_cast<T>(from.y)}; }

	//fast typenames
	using vec3i = vec3<int>;
	using vec3i8 = vec3<int8_t>;
	using vec3i16 = vec3<int16_t>;
	using vec3i32 = vec3<int32_t>;
	using vec3i64 = vec3<int64_t>;

	using vec3u = vec3<unsigned>;
	using vec3u8 = vec3<uint8_t>;
	using vec3u16 = vec3<uint16_t>;
	using vec3u32 = vec3<uint32_t>;
	using vec3u64 = vec3<uint64_t>;

	using vec3s = vec3<size_t>;
	using vec3f = vec3<float>;
	using vec3d = vec3<double>;

	template <typename T>
	class vec3
		{
		public:
			T x, y, z;
		};
	}