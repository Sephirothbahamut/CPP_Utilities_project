#include "io.h"

#include "../compilation/OS.h"

//getch
#ifdef utils_compilation_os_windows
//	{
	#include <conio.h>
//	}
#elif utils_compilation_os_linux
//	{
	#include <unistd.h>
	#include <termios.h>

	char _getch()
		{
		//Do not question, I don't know either. It just works.
		char buf = 0;
		struct termios old = {0};
		fflush(stdout);
		if (tcgetattr(0, &old) < 0)
			perror("tcsetattr()");
		old.c_lflag &= ~ICANON;
		old.c_lflag &= ~ECHO;
		old.c_cc[VMIN] = 1;
		old.c_cc[VTIME] = 0;
		if (tcsetattr(0, TCSANOW, &old) < 0)
			perror("tcsetattr ICANON");
		if (read(0, &buf, 1) < 0)
			perror("read()");
		old.c_lflag |= ICANON;
		old.c_lflag |= ECHO;
		if (tcsetattr(0, TCSADRAIN, &old) < 0)
			perror("tcsetattr ~ICANON");
		return buf;
		}
//	}
#else
//	{
	#error Compilation target OS not recognized.
//	}
#endif

//size
#ifdef utils_compilation_os_windows
//	{
	#include <Windows.h>
//	}
#elif utils_compilation_os_linux
//	{
	#include <sys/ioctl.h>
//	}
#endif

namespace utils::console
	{
	char get_ch() noexcept { return static_cast<char>(_getch()); }

	utils::math::vec2s size() noexcept
		{// https://stackoverflow.com/questions/23369503/get-size-of-terminal-window-rows-columns
		#ifdef utils_compilation_os_windows
		//	{
			CONSOLE_SCREEN_BUFFER_INFO csbi;
			GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
			const auto width {static_cast<size_t>(csbi.srWindow.Right - csbi.srWindow.Left + 1)};
			const auto height{static_cast<size_t>(csbi.srWindow.Bottom - csbi.srWindow.Top + 1)};
			return {width, height};
		//	}
		#elif utils_compilation_os_linux
		//	{
			struct winsize w;
			ioctl(fileno(stdout), TIOCGWINSZ, &w);
			const auto width {static_cast<size_t>(w.ws_col)};
			const auto height{static_cast<size_t>(w.ws_row)};
			return {width, height};
		//	}
		#endif
		}
	}