#pragma once

#include <utility>

#include "../../flags.h"

namespace utils::containers::multithreading
	{
	enum class operation_flag_bits : uint8_t { pre = 0b00000001, post = 0b00000010 };
	}