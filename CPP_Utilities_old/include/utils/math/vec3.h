#pragma once
#include "vec.h"

namespace utils::math
	{
	template <typename T>
	using vec3 = vec<T, 3>;

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
	}