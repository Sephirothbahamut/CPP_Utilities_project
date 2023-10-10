#pragma once

#include <iomanip>
#include <iostream>

#include "../common.h"

namespace utils::input_system::device
	{
	namespace details
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

	struct mouse : device::base<details::buttons_t, details::axes_t>
		{
		using button_id = details::button_id;
		using axis_id   = details::axis_id  ;

		using buttons_t = details::buttons_t;
		using axes_t    = details::axes_t   ;

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

				debug_callbacks(mouse& device) :
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

	using maus = mouse;
	using 🐭  = mouse;
	}