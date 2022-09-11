#pragma once

namespace utils
	{
	struct nocopy
		{
		nocopy() = default;
		nocopy(const nocopy& copy) = delete;
		nocopy& operator=(const nocopy& copy) = delete;
		};
	struct nomove
		{
		nomove() = default;
		nomove(nomove&& move) = delete;
		nomove& operator=(nomove&& move) = delete;
		};
	}