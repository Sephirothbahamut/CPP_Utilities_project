#pragma once

#include <iomanip>
#include <iostream>

#include "common.h"

namespace utils::input_system::device
	{
	namespace details::keyboard
		{
		enum class button_id
			{
			unknown,
			//letters
			a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z,
			//numbers
			num_0, num_1, num_2, num_3, num_4, num_5, num_6, num_7, num_8, num_9,
			//numpad
			numpad_0, numpad_1, numpad_2, numpad_3, numpad_4, numpad_5, numpad_6, numpad_7, numpad_8, numpad_9,
			num_lock, numpad_divide, numpad_multiply, numpad_subtract, numpad_add, numpad_decimal, numpad_separator,
			//function
			F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15, F16, F17, F18, F19, F20, F21, F22, F23, F24,
			//stuff
			space, tab, caps_lock, esc, enter, del, backspace, ins,
			ctrl, alt, shift, win, menu/*the one between right control and right windows keys*/,
			ctrl_left, ctrl_right, alt_left, alt_right, shift_left, shift_right, win_left, win_right, menu_left, menu_right,
			//special children
			print_screen, scroll_lock, pause,
			//movement
			arrow_right, arrow_up, arrow_left, arrow_down, end, home, page_up, page_down,
			//media controls
			media_stop, media_pause, media_next, media_previous, media_mute, media_volume_up, media_volume_down,
			//country based keys (VK_OEM# in Win32 API)
			add, comma, subtract, period,
			oem_1, oem_2, oem_3, oem_4, oem_5, oem_6, oem_7, oem_8, oem_102
			//TODO
			};
		
		using axis_id = void;

		using buttons_t = device::inputs::dynamic<input::digital, button_id>;
		using axes_t    = device::inputs::none<input::analog>;
		}

	struct keyboard : device::base<details::keyboard::buttons_t, details::keyboard::axes_t>
		{
		using button_id = details::keyboard::button_id;
		using axis_id   = details::keyboard::axis_id  ;

		using buttons_t = details::keyboard::buttons_t;
		using axes_t    = details::keyboard::axes_t   ;
		};

	//using ⌨ = keyboard;
	}