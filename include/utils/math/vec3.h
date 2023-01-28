#pragma once
#include "vec.h"

namespace utils::math
	{
	template <typename T> 
	using vec3 = vec<T, 3>;
	
	//fast typenames
	template <typename T>
	using vecref3 = vecref<T, 3>;
	
	using vec3i      = vec   <int          , 3>;
	using vec3i8     = vec   <int8_t       , 3>;
	using vec3i16    = vec   <int16_t      , 3>;
	using vec3i32    = vec   <int32_t      , 3>;
	using vec3i64    = vec   <int64_t      , 3>;
	using vec3u      = vec   <unsigned     , 3>;
	using vec3u8     = vec   <uint8_t      , 3>;
	using vec3u16    = vec   <uint16_t     , 3>;
	using vec3u32    = vec   <uint32_t     , 3>;
	using vec3u64    = vec   <uint64_t     , 3>;
	using vec3s      = vec   <size_t       , 3>;
	using vec3f      = vec   <float        , 3>;
	using vec3d      = vec   <double       , 3>;
	using vec3l      = vec   <long         , 3>;
	using vec3ul     = vec   <unsigned long, 3>;
	using vecref3i   = vecref<int          , 3>;
	using vecref3i8  = vecref<int8_t       , 3>;
	using vecref3i16 = vecref<int16_t      , 3>;
	using vecref3i32 = vecref<int32_t      , 3>;
	using vecref3i64 = vecref<int64_t      , 3>;
	using vecref3u   = vecref<unsigned     , 3>;
	using vecref3u8  = vecref<uint8_t      , 3>;
	using vecref3u16 = vecref<uint16_t     , 3>;
	using vecref3u32 = vecref<uint32_t     , 3>;
	using vecref3u64 = vecref<uint64_t     , 3>;
	using vecref3s   = vecref<size_t       , 3>;
	using vecref3f   = vecref<float        , 3>;
	using vecref3d   = vecref<double       , 3>;
	using vecref3l   = vecref<long         , 3>;
	using vecref3ul  = vecref<unsigned long, 3>;
	}