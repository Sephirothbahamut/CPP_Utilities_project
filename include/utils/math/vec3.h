#pragma once

#include "vec.h"


namespace utils::math
	{
	template <typename T> 
	using vec3 = vec<T, 3>;
	
	//fast typenames
	template <typename T>
	using vecref3 = vecref<T, 3>;
	
	using vec3i      = vec3   <int          >;
	using vec3i8     = vec3   <int8_t       >;
	using vec3i16    = vec3   <int16_t      >;
	using vec3i32    = vec3   <int32_t      >;
	using vec3i64    = vec3   <int64_t      >;
	using vec3u      = vec3   <unsigned     >;
	using vec3u8     = vec3   <uint8_t      >;
	using vec3u16    = vec3   <uint16_t     >;
	using vec3u32    = vec3   <uint32_t     >;
	using vec3u64    = vec3   <uint64_t     >;
	using vec3s      = vec3   <size_t       >;
	using vec3f      = vec3   <float        >;
	using vec3d      = vec3   <double       >;
	using vec3l      = vec3   <long         >;
	using vec3ul     = vec3   <unsigned long>;
	using vecref3i   = vecref3<int          >;
	using vecref3i8  = vecref3<int8_t       >;
	using vecref3i16 = vecref3<int16_t      >;
	using vecref3i32 = vecref3<int32_t      >;
	using vecref3i64 = vecref3<int64_t      >;
	using vecref3u   = vecref3<unsigned     >;
	using vecref3u8  = vecref3<uint8_t      >;
	using vecref3u16 = vecref3<uint16_t     >;
	using vecref3u32 = vecref3<uint32_t     >;
	using vecref3u64 = vecref3<uint64_t     >;
	using vecref3s   = vecref3<size_t       >;
	using vecref3f   = vecref3<float        >;
	using vecref3d   = vecref3<double       >;
	using vecref3l   = vecref3<long         >;
	using vecref3ul  = vecref3<unsigned long>;
	}