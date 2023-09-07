#pragma once

#if defined(_WIN32)
    #define utils_compilation_os_windows
#elif defined(__linux__) 
	#define utils_compilation_os_linux
#endif

namespace utils::compilation
	{
	enum class os_t { Windows, Linux };

	inline constexpr os_t os
#ifdef utils_compilation_os_windows
		{os_t::Windows};
#elif utils_compilation_os_linux
		{OS::Linux};
#else
#error Compilation target OS not recognized.
#endif
	}