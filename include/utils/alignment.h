#pragma once

namespace utils
	{
	struct alignment
		{
		enum class horizontal { left, centre, right  };
		enum class vertical   { top , middle, bottom };
		
		horizontal horizontal_alignment;
		vertical   vertical_alignment  ;
		};
	}