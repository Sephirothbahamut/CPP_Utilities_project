#pragma once

//#define utils_virtualize   protected: virtual void utils_i_am_virtual() = 0;
//#define utils_devirtualize protected: virtual void utils_i_am_virtual() final override { };
namespace utils
	{
	struct virtualize { virtual void utils_i_am_virtual() = 0; };
	struct devirtualize : virtual virtualize { virtual void utils_i_am_virtual() final override {} };
	}