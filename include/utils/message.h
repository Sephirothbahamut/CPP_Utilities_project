#pragma once
#include <chrono>
#include <string>
#include <string_view>
#include <iomanip>

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

			static message log(std::string&& string = "") noexcept { return {msg_t::log, std::move(string), std::chrono::system_clock::now()}; }
			static message dgn(std::string&& string = "") noexcept { return {msg_t::dgn, std::move(string), std::chrono::system_clock::now()}; }
			static message inf(std::string&& string = "") noexcept { return {msg_t::inf, std::move(string), std::chrono::system_clock::now()}; }
			static message wrn(std::string&& string = "") noexcept { return {msg_t::wrn, std::move(string), std::chrono::system_clock::now()}; }
			static message err(std::string&& string = "") noexcept { return {msg_t::err, std::move(string), std::chrono::system_clock::now()}; }

			const char* out_type() const noexcept
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
			const char* out_type_verbose() const noexcept
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
			utils::console::colour::foreground_t out_type_color() const noexcept
				{
				switch (type)
					{
					case message::msg_t::log: return utils::console::colour::foreground::white;
					case message::msg_t::dgn: return utils::console::colour::foreground::magenta;
					case message::msg_t::inf: return utils::console::colour::foreground::cyan;
					case message::msg_t::wrn: return utils::console::colour::foreground::yellow;
					case message::msg_t::err: return utils::console::colour::foreground::red;
					default: return utils::console::colour::foreground::red;
					}
				}

		private:
			message(msg_t type, std::string&& string, std::chrono::time_point<std::chrono::system_clock> time) noexcept
				: type{type}, string{string}, time{time}
				{}
			message(msg_t type, std::string&& string) noexcept
				: type{type}, string{string}, time{std::chrono::system_clock::now()}
				{}
			message(std::string&& string) noexcept
				: type{msg_t::log}, string{string}, time{std::chrono::system_clock::now()}
				{}

			msg_t type = msg_t::log;
			std::string string{};
			std::chrono::time_point<std::chrono::system_clock> time;

			static std::string filter_last_newline(const std::string& string) noexcept
				{
				if (string.length() > 0 && string[string.length() - 1] == '\n') { return string.substr(0, string.length() - 1); }
				else { return string; }
				}

			friend std::ostream& operator<<(std::ostream& os, const message& m)
				{
				using namespace utils::output;

				if constexpr (message_output_style == message_output_style_t::on_line)
					{

					size_t beg = 0;
					size_t end = m.string.find_first_of('\n', beg);
					if (end == std::string::npos) { end = m.string.length(); }

					//First line
					os << utils::console::colour::foreground::dark_white << std::setw(18) << m.time.time_since_epoch().count() << ' ';

					os << m.out_type_color() << m.out_type();

					os << utils::console::colour::foreground::dark_white << ' ' << std::string_view(m.string).substr(beg, end - beg) << '\n';

					//Other lines
					while (true)
						{
						if (end == m.string.length()) { break; }
						else
							{
							beg = end + 1;
							end = m.string.find_first_of('\n', beg);
							if (end == std::string::npos) { end = m.string.length(); }
							}

						os << std::setw(24) << m.out_type_color() << '|';
						os << utils::console::colour::foreground::dark_white << ' ' << std::string_view(m.string).substr(beg, end - beg) << '\n';
						}

					return os;
					}
				else if constexpr (message_output_style == message_output_style_t::tag_as_separator)
					{
					size_t beg = 0;
					size_t end = m.string.find_first_of('\n', beg);
					if (end == std::string::npos) { end = m.string.length(); }

					//Data line
					os << "_________________________________\n";
					os << m.out_type_color() << ' ' << std::left << std::setw(12) << m.out_type_verbose() << ' ' << std::right << std::setw(18) << m.time.time_since_epoch().count() << '\n';
					//First line
					os << utils::console::colour::foreground::dark_white << ' ' << std::string_view(m.string).substr(beg, end - beg) << '\n';

					//Other lines
					while (true)
						{
						if (end == m.string.length()) { break; }
						else
							{
							beg = end + 1;
							end = m.string.find_first_of('\n', beg);
							if (end == std::string::npos) { end = m.string.length(); }
							}

						os << std::string_view(m.string).substr(beg, end - beg) << '\n';
						}

					return os;
					}
				}

		};
	}