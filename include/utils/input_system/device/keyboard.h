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
			//letters
			a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z,
			//numbers
			num_0, num_1, num_2, num_3, num_4, num_5, num_6, num_7, num_8, num_9,
			//numpad
			numpad_0, numpad_1, numpad_2, numpad_3, numpad_4, numpad_5, numpad_6, numpad_7, numpad_8, numpad_9,
			num_lock, numpad_divide, numpad_multiply, numapd_subtract, numapd_add,
			//function
			F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15, F16, F17, F18, F19, F20, F21, F22, F23, F24,
			//stuff
			space, tab, caps_lock, esc, enter, del, backspace, menu/*the one between right control and right windows keys*/,
			//modifiers
			ctrl_left, ctrl_right, alt_left, alt_right, shift_left, shift_right, win_left, win_right,
			//special children
			print_screen, scroll_lock, pause,
			//movement
			arrow_right, arrow_up, arrow_left, arrow_down, end, home, page_up, page_down,
			//media controls
			media_stop, media_pause, media_next, media_previous, media_mute, media_volume_up, media_volume_down,
			//country based keys (VK_OEM# in Win32 API)
			//TODO
			};
		
		using axis_id = void;

		using buttons_t = device::inputs::static_enum<input::digital, button_id>;
		using axes_t    = device::inputs::none;
		}

	struct keyboard : device::base<details::keyboard::buttons_t, details::keyboard::axes_t>
		{
		using button_id = details::keyboard::button_id;
		using axis_id   = details::keyboard::axis_id  ;

		using buttons_t = details::keyboard::buttons_t;
		using axes_t    = details::keyboard::axes_t   ;

		class debug_callbacks
			{
			public:
				struct callback_digital_t
					{
					callback_digital_t(button_id id) : id{id} {}
					button_id id;
					utils::input_system::on_completion operator()(const utils::input_system::state_base<bool>& state) const noexcept
						{
						std::cout << utils::enums::enum_name<button_id>(id) << " ";
						if (state.changed()) { std::cout << "(changed)"; }
						else { std::cout << "(unchanged)"; }
						std::cout << " " << state.previous << " > " << state.current << std::endl;
						return utils::input_system::on_completion::remove;
						}
					};

				debug_callbacks(keyboard& device) :
					callbacks_digital
						{
						utils::input_system::event<bool>{callback_digital_t{button_id::left    }},
						utils::input_system::event<bool>{callback_digital_t{button_id::right   }},
						utils::input_system::event<bool>{callback_digital_t{button_id::middle  }},
						utils::input_system::event<bool>{callback_digital_t{button_id::backward}},
						utils::input_system::event<bool>{callback_digital_t{button_id::forward }}
						},
					callback_analog{[](const utils::input_system::state_base<utils::math::vec2f>& state) -> utils::input_system::on_completion 
						{
						std::cout << "XY : (" << state.previous.x << ", " <<state.previous.y << ") > ("
							<< state.current.x << ", " << state.current.y << ")" << std::endl;
						return utils::input_system::on_completion::remove;
						}}
					{
					callbacks_digital[static_cast<size_t>(button_id::left    )].emplace_mapping<utils::input_system::mapping::button::device_input>(device.digital[button_id::left    ]);
					callbacks_digital[static_cast<size_t>(button_id::right   )].emplace_mapping<utils::input_system::mapping::button::device_input>(device.digital[button_id::right   ]);
					callbacks_digital[static_cast<size_t>(button_id::middle  )].emplace_mapping<utils::input_system::mapping::button::device_input>(device.digital[button_id::middle  ]);
					callbacks_digital[static_cast<size_t>(button_id::backward)].emplace_mapping<utils::input_system::mapping::button::device_input>(device.digital[button_id::backward]);
					callbacks_digital[static_cast<size_t>(button_id::forward )].emplace_mapping<utils::input_system::mapping::button::device_input>(device.digital[button_id::forward ]);
					
					auto& axes{callback_analog.emplace_mapping<utils::input_system::mapping::axis2d::from_axes>()};
					axes.emplace_x<utils::input_system::mapping::axis1d::device_input>(device.analog[axis_id::x]);
					axes.emplace_y<utils::input_system::mapping::axis1d::device_input>(device.analog[axis_id::y]);
					}

			private:
				std::array<utils::input_system::event<bool>, buttons_t::count> callbacks_digital;
				utils::input_system::event<utils::math::vec2f> callback_analog;
			};

		};

	using ⌨ = keyboard;
	}