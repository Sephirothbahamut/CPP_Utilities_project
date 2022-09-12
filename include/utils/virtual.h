#pragma once

#define utils_virtualize   protected: virtual void utils_i_am_virtual() = 0;
#define utils_devirtualize protected: virtual void utils_i_am_virtual() final override { };