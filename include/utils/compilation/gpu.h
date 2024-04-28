#pragma once

namespace utils::compilation::gpu
	{
	//The macro __HIPCC__ is set if either __HCC__ or __CUDACC__ is defined.
	inline constexpr bool device
#if defined(__HIPCC__) || defined(__NVCC__)
		{true};
#else
	{false};
#endif

	inline constexpr bool CUDA
#ifdef __NVCC__
		{true};
#else
	{false};
#endif
	inline constexpr bool HIP
#ifdef __HCC__
		{true};
#else
	{false};
#endif

	enum class target_t { nvcc, hip, none };
	inline constexpr target_t target
#ifdef HIPCC_PLATFORM_HCC 
		{target_t::hip};
#elif defined(HIPCC_PLATFORM_NVCC)
		{target_t::nvcc};
#else
		{target_t::none};
#endif
	}

#if defined __NVCC__ || defined __HCC__
	#define utils_gpu_available __device__ __host__
	#define utils_is_gpu
	#define utils_if_gpu(x) x
#else
	#define utils_gpu_available
	#define utils_if_gpu(x)
#endif