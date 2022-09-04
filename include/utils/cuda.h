#pragma once

#ifdef UTILS_CUDA_EXPOSE
#define UTILS_CUDA_EXPOSE __host__ __device__
#else
#define UTILS_CUDA_EXPOSE 
#endif // CUDA_EXPOSE
