#pragma once

#if defined(_WIN32)
    #define utils_os_windows
#elif defined(__linux__) 
	#define utils_os_linux
#endif

namespace utils::compilation
	{
	enum class os_t { Windows, Linux };

	inline constexpr os_t os
#ifdef _WIN32
		{os_t::Windows};
#elif __linux__
		{OS::Linux};
#else
#error Compilation target OS not recognized.
#endif
	}