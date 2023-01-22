#pragma once

namespace utils::compilation
	{
	inline constexpr bool CUDA
#ifdef __NVCC__
		{true};
#else
		{false};
#endif
	}

#ifdef __NVCC__
#define utils_is_CUDA
#define utils_if_CUDA(x)
#endif