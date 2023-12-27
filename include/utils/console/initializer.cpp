#include "initializer.h"

#include "../compilation/OS.h"

#ifdef utils_compilation_os_windows
//	{
	#include <Windows.h>

	namespace utils::console
		{
		initializer::initializer() noexcept
			{
			SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), ENABLE_VIRTUAL_TERMINAL_INPUT);
			}
		}
//	}
#elif utils_compilation_os_linux
//	{
	namespace utils::console
		{
		initializer::initializer() noexcept {}
		}
//	}
#endif