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

#ifdef utils_is_CUDA
#include <stdexcept>

#include "cuda_runtime.h"
#include "device_launch_parameters.h"

namespace utils::CUDA
	{
	struct error : std::runtime_error { using std::runtime_error::runtime_error; };

	/// <summary>
	/// Prints eventual CUDA errors before terminating the program
	/// </summary>
	inline void throw_if_failed(cudaError_t err)
		{
		if (err != cudaSuccess)
			{
			throw error{cudaGetErrorString(err)};
			}
		}
	}

// Makes sure intellisense recognizes __syncthreads as a defined function
#ifdef __INTELLISENSE__
void __syncthreads() {};
#endif

#endif