
#define utils_c_libraries_implementation
#include "include/utils/math/geometry/voronoi/voronoi.h"

//#define utils_implementation
#include "include/utils/console/colour.h"

#include <Windows.h>
namespace utils::console
	{
	initializer::initializer() noexcept
		{
		SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), ENABLE_VIRTUAL_TERMINAL_INPUT);
		}
	}