#pragma once

namespace utils
	{
	void discard(const auto& v) { static_cast<void>(v); }
	}