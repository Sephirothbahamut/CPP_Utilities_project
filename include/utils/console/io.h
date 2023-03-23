#pragma once
#include "../compilation/OS.h"

#ifdef utils_os_windows

	#ifdef NOMINMAX
		#include <windows.h>
		#include <conio.h>
	#else
		#define NOMINMAX
		#include <windows.h>
		#include <conio.h>
		#undef NOMINMAX
	#endif

#elif utils_os_linux
	char _getch()
		{
		//Do not question, I don't know either. It just works.
	#include <unistd.h>
	#include <termios.h>
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
#else
#error Compilation target OS not recognized.
#endif