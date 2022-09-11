#pragma once

#include "console_io.h"

#include <iostream>

//TODO linux compatible colouring
#ifdef __linux__
#error Linux compatible console colouring is not yet available
#endif

namespace utils
	{
	namespace cout
		{
		enum class color : WORD
			{
			dkred = FOREGROUND_RED,
			dkgreen = FOREGROUND_GREEN,
			dkblue = FOREGROUND_BLUE,
			dkyellow = FOREGROUND_RED | FOREGROUND_GREEN,
			dkmagenta = FOREGROUND_RED | FOREGROUND_BLUE,
			dkcyan = FOREGROUND_GREEN | FOREGROUND_BLUE,
			dkwhite = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
			red = FOREGROUND_RED | FOREGROUND_INTENSITY,
			green = FOREGROUND_GREEN | FOREGROUND_INTENSITY,
			blue = FOREGROUND_BLUE | FOREGROUND_INTENSITY,
			yellow = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
			magenta = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
			cyan = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
			white = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
			_ = FOREGROUND_INTENSITY,
			//aliases
			gray = dkwhite,
			grey = dkwhite,
			dr = dkred, dg = dkgreen, db = dkblue, dy = dkyellow, dm = dkmagenta, dc = dkcyan, dw = dkwhite,
			r = red, g = green, b = blue, y = yellow, m = magenta, c = cyan, w = white
			};

		inline std::ostream& operator<< (std::ostream& s, const color& c)
			{
			if (s.rdbuf() == std::cout.rdbuf())
				{
				HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
				SetConsoleTextAttribute(console, (WORD)c);
				}
			return s;
			}

		namespace support
			{
			constexpr /*inline*/ color brace = color::dkmagenta;
			constexpr /*inline*/ color separ = color::dkwhite;
			constexpr /*inline*/ color value = color::dkyellow;
			constexpr /*inline*/ color type = color::cyan;
			}
		}
	}