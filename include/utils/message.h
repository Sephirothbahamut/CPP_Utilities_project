#pragma once
#include <chrono>
#include <string>
#include <string_view>
#include <iomanip>
#include <string_view>

#include "console/colour.h"

//TODO output readable date-time +GMT delta instead of timestamp
//Ideally: yyyy/mm/dd hh:mm:ss:millis

namespace utils
	{
	enum class message_output_style_t { on_line, tag_as_separator };

	inline constexpr message_output_style_t message_output_style = message_output_style_t::on_line;

	class message
		{
		enum class msg_t { log, dgn, inf, wrn, err };

		public:
			message(const message& copy) = default;
			message& operator=(const message& copy) = default;
			message(message&& move) = default;
			message& operator=(message&& move) = default;

			inline static message log(std::string&& string = "") noexcept { return {msg_t::log, std::move(string), std::chrono::system_clock::now()}; }
			inline static message dgn(std::string&& string = "") noexcept { return {msg_t::dgn, std::move(string), std::chrono::system_clock::now()}; }
			inline static message inf(std::string&& string = "") noexcept { return {msg_t::inf, std::move(string), std::chrono::system_clock::now()}; }
			inline static message wrn(std::string&& string = "") noexcept { return {msg_t::wrn, std::move(string), std::chrono::system_clock::now()}; }
			inline static message err(std::string&& string = "") noexcept { return {msg_t::err, std::move(string), std::chrono::system_clock::now()}; }

			inline constexpr std::string_view out_type() const noexcept
				{
				switch (type)
					{
					case message::msg_t::log: return "[LOG]";
					case message::msg_t::dgn: return "[DGN]";
					case message::msg_t::inf: return "[INF]";
					case message::msg_t::wrn: return "[WRN]";
					case message::msg_t::err: return "[ERR]";
					default: return "[This error code should be impossible to get]";
					}
				}
			inline constexpr std::string_view out_type_verbose() const noexcept
				{
				switch (type)
					{
					case message::msg_t::log: return "[LOG]";
					case message::msg_t::dgn: return "[DIAGNOSTIC]";
					case message::msg_t::inf: return "[INFO]";
					case message::msg_t::wrn: return "[WARNING]";
					case message::msg_t::err: return "[ERROR]";
					default: return "[This error code should be impossible to get]";
					}
				}
			inline constexpr utils::graphics::colour::base out_type_color() const noexcept
				{
				switch (type)
					{
					case message::msg_t::log: return utils::graphics::colour::base::white;
					case message::msg_t::dgn: return utils::graphics::colour::base::magenta;
					case message::msg_t::inf: return utils::graphics::colour::base::cyan;
					case message::msg_t::wrn: return utils::graphics::colour::base::yellow;
					case message::msg_t::err: return utils::graphics::colour::base::red;
					default: return utils::graphics::colour::base::red;
					}
				}

			inline constexpr std::chrono::time_point<std::chrono::system_clock> get_timestamp() const noexcept { return time; }

		private:
			inline message(msg_t type, std::string&& string, std::chrono::time_point<std::chrono::system_clock> time) noexcept
				: type{type}, string{string}, time{time}
				{}
			inline message(msg_t type, std::string&& string) noexcept
				: type{type}, string{string}, time{std::chrono::system_clock::now()}
				{}
			inline message(std::string&& string) noexcept
				: type{msg_t::log}, string{string}, time{std::chrono::system_clock::now()}
				{}

			msg_t type = msg_t::log;
			std::string string{};
			std::chrono::time_point<std::chrono::system_clock> time;

			inline static constexpr std::string filter_last_newline(const std::string& string) noexcept
				{
				if (string.length() > 0 && string[string.length() - 1] == '\n') { return string.substr(0, string.length() - 1); }
				else { return string; }
				}

			inline friend std::ostream& operator<<(std::ostream& os, const message& m)
				{
				using namespace utils::output;

				//Data line
				if constexpr (message_output_style == message_output_style_t::tag_as_separator)
					{
					os << "_________________________________\n";
					os << ' ';

					os << utils::console::colour::background{utils::console::colour::colour_8::dark  (m.out_type_color())};
					os << utils::console::colour::foreground{utils::console::colour::colour_8::bright(utils::graphics::colour::base::white)};
					os << std::left << m.out_type_verbose();
					os << utils::console::colour::restore_defaults;

					for (size_t i = 0; i < (12 - m.out_type_verbose().length()); i++) { os << ' '; }

					os << ' ' << std::right << std::setw(18) << m.time.time_since_epoch().count() << '\n';
					}

				//First line

				bool first_line{true};

				size_t beg = 0;
				size_t end = m.string.find_first_of('\n', beg);
				if (end == std::string::npos) { end = m.string.length(); }

				os << ' ' << std::string_view(m.string).substr(beg, end - beg) << '\n';

				//Lines
				while (end < m.string.length())
					{
					if constexpr (message_output_style == message_output_style_t::on_line)
						{
						os << " ";
						os << utils::console::colour::background{utils::console::colour::colour_8::dark(m.out_type_color())};
						os << utils::console::colour::foreground{utils::console::colour::colour_8::bright(utils::graphics::colour::base::white)};
						os << std::left << m.out_type();
						os << utils::console::colour::restore_defaults;
						os << " ";

						if (first_line)
							{
							os << utils::console::colour::foreground{utils::console::colour::colour_8::dark(utils::graphics::colour::base::white)};
							os << std::setw(18) << m.time.time_since_epoch().count() << ' ';
							os << utils::console::colour::restore_defaults;
							first_line = false;
							}
						else 
							{
							for (size_t i = 0; i < 18 - 2; i++) { os << ' '; }
							os << "| ";
							}
						}

					os << std::string_view(m.string).substr(beg, end - beg) << '\n';

					beg = end + 1;
					end = m.string.find_first_of('\n', beg);
					if (end == std::string::npos) { end = m.string.length(); }
					}

				return os;
				}

		};
	}