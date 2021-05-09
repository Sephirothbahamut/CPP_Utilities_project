#pragma once
#include <chrono>
#include <string>
#include <string_view>
#include <iomanip>

#include "cout_utilities.h"

namespace utils
	{
	class message
		{
		enum class Type { log, dgn, inf, wrn, err };

		public:
			message(const message& copy) = default;
			message& operator=(const message& copy) = default;
			message(message&& move) = default;
			message& operator=(message&& move) = default;

			static message log(std::string&& string = "") noexcept { return {Type::log, std::move(string), std::chrono::system_clock::now()}; }
			static message dgn(std::string&& string = "") noexcept { return {Type::dgn, std::move(string), std::chrono::system_clock::now()}; }
			static message inf(std::string&& string = "") noexcept { return {Type::inf, std::move(string), std::chrono::system_clock::now()}; }
			static message wrn(std::string&& string = "") noexcept { return {Type::wrn, std::move(string), std::chrono::system_clock::now()}; }
			static message err(std::string&& string = "") noexcept { return {Type::err, std::move(string), std::chrono::system_clock::now()}; }

			const char* out_type() const noexcept
				{
				switch (type)
					{
					case message::Type::log: return "[LOG]";
					case message::Type::dgn: return "[DGN]";
					case message::Type::inf: return "[INF]";
					case message::Type::wrn: return "[WRN]";
					case message::Type::err: return "[ERR]";
					default: return "[This error code should be impossible to get]";
					}
				}
			const char* out_type_verbose() const noexcept
				{
				switch (type)
					{
					case message::Type::log: return "[LOG]";
					case message::Type::dgn: return "[DIAGNOSTIC]";
					case message::Type::inf: return "[INFO]";
					case message::Type::wrn: return "[WARNING]";
					case message::Type::err: return "[ERROR]";
					default: return "[This error code should be impossible to get]";
					}
				}
			utils::cout::color out_type_color() const noexcept
				{
				switch (type)
					{
					case message::Type::log: return utils::cout::color::white;
					case message::Type::dgn: return utils::cout::color::magenta;
					case message::Type::inf: return utils::cout::color::cyan;
					case message::Type::wrn: return utils::cout::color::yellow;
					case message::Type::err: return utils::cout::color::red;
					default: return utils::cout::color::red;
					}
				}

		private:
			message(Type type, std::string&& string, std::chrono::time_point<std::chrono::system_clock> time) noexcept
				: type{type}, string{string}, time{time}
				{}
			message(Type type, std::string&& string) noexcept
				: type{type}, string{string}, time{std::chrono::system_clock::now()}
				{}
			message(std::string&& string) noexcept
				: type{Type::log}, string{string}, time{std::chrono::system_clock::now()}
				{}

			Type type = Type::log;
			std::string string{};
			std::chrono::time_point<std::chrono::system_clock> time;

			static std::string filter_last_newline(const std::string& string) noexcept
				{
				if (string.length() > 0 && string[string.length() - 1] == '\n') { return string.substr(0, string.length() - 1); }
				else { return string; }
				}


			//ON LINE VERSION
			friend std::ostream& operator<<(std::ostream& os, const message& m)
				{
				size_t beg = 0;
				size_t end = m.string.find_first_of('\n', beg);
				if (end == std::string::npos) { end = m.string.length(); }

				//First line
				os << utils::cout::color::dw << std::setw(18) << m.time.time_since_epoch().count() << ' ';

				os << m.out_type_color() << m.out_type();

				os << utils::cout::color::dw << ' ' << std::string_view(m.string).substr(beg, end - beg) << '\n';

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
					os << utils::cout::color::dw << ' ' << std::string_view(m.string).substr(beg, end - beg) << '\n';
					}

				return os;
				}

			//TAG AS SEPARATOR VERSION
			/*friend std::ostream& operator<<(std::ostream& os, const Message& m)
				{
				size_t beg = 0;
				size_t end = m.string.find_first_of('\n', beg);
				if (end == std::string::npos) { end = string.length(); }

				//Data line
				os << "_________________________________\n";
				os << m.out_type_color() << ' ' << std::left << std::setw(12) << m.out_type_verbose() << ' ' << std::right << std::setw(18) << m.time.time_since_epoch().count() << '\n';
				//First line
				os << utils::cout::color::dw << ' ' << std::string_view(string).substr(beg, end - beg) << '\n';

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

					os << std::string_view(string).substr(beg, end - beg) << '\n';
					}

				return os;
				}*/
		};
	}