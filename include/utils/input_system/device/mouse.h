#pragma once

#include <iomanip>
#include <iostream>

#include "common.h"

namespace utils::input_system::device
	{
	namespace details::mouse
		{
		enum class button_id
			{
			left,
			right,
			middle,
			backward,
			forward
			};

		enum class axis_id { x, y };
		
		using buttons_t = device::inputs::static_enum<input::digital, button_id>;
		using axes_t    = device::inputs::static_enum<input::analog , axis_id  >;
		}

	struct mouse : device::base<details::mouse::buttons_t, details::mouse::axes_t>
		{
		using button_id = details::mouse::button_id;
		using axis_id   = details::mouse::axis_id  ;

		using buttons_t = details::mouse::buttons_t;
		using axes_t    = details::mouse::axes_t   ;
		};

	using maus = mouse;
	using 🐭  = mouse;
	}