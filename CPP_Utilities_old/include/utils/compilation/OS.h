#pragma once

namespace utils::compilation
	{
	enum class OS { Windows, Linux };

	inline constexpr OS os
#ifdef _WIN32
		{OS::Windows};
#elif __linux__
	{ OS::Linux };
#else
#error Compilation target OS not recognized.
#endif
	}