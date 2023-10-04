#pragma once
#include <chrono>
#include <string>
#include <string_view>
#include <iomanip>
#include <string_view>

#include "../string.h"
#include "../console/colour.h"

//TODO output readable date-time +GMT delta instead of timestamp
//Ideally: yyyy/mm/dd hh:mm:ss:millis

namespace utils::logging
	{
	enum class output_style_t { on_line, tag_as_separator };
	enum class msg_t { log, dgn, inf, wrn, err };

	template <output_style_t OUTPUT_STYLE>
	class message
		{
		public:
			inline static constexpr output_style_t output_style = OUTPUT_STYLE;

			constexpr message           (const message&  copy) = default;
			constexpr message& operator=(const message&  copy) = default;
			constexpr message           (      message&& move) = default;
			constexpr message& operator=(      message&& move) = default;

			static constexpr message log(const string::concepts::stringlike auto& string = "") noexcept { return {msg_t::log, string}; }
			static constexpr message dgn(const string::concepts::stringlike auto& string = "") noexcept { return {msg_t::dgn, string}; }
			static constexpr message inf(const string::concepts::stringlike auto& string = "") noexcept { return {msg_t::inf, string}; }
			static constexpr message wrn(const string::concepts::stringlike auto& string = "") noexcept { return {msg_t::wrn, string}; }
			static constexpr message err(const string::concepts::stringlike auto& string = "") noexcept { return {msg_t::err, string}; }

			constexpr std::string_view out_type() const noexcept
				{
				switch (type)
					{
					case msg_t::log: return "[LOG]";
					case msg_t::dgn: return "[DGN]";
					case msg_t::inf: return "[INF]";
					case msg_t::wrn: return "[WRN]";
					case msg_t::err: return "[ERR]";
					default: return "[This error code should be impossible to get]";
					}
				}
			constexpr std::string_view out_type_verbose() const noexcept
				{
				switch (type)
					{
					case msg_t::log: return "[LOG]"       ;
					case msg_t::dgn: return "[DIAGNOSTIC]";
					case msg_t::inf: return "[INFO]"      ;
					case msg_t::wrn: return "[WARNING]"   ;
					case msg_t::err: return "[ERROR]"     ;
					default: return "[This error code should be impossible to get]";
					}
				}
			constexpr utils::graphics::colour::base out_type_color() const noexcept
				{
				switch (type)
					{
					case msg_t::log: return utils::graphics::colour::base::white  ;
					case msg_t::dgn: return utils::graphics::colour::base::magenta;
					case msg_t::inf: return utils::graphics::colour::base::cyan   ;
					case msg_t::wrn: return utils::graphics::colour::base::yellow ;
					case msg_t::err: return utils::graphics::colour::base::red    ;
					default        : return utils::graphics::colour::base::red    ;
					}
				}

			constexpr std::chrono::time_point<std::chrono::system_clock> get_timestamp() const noexcept { return time; }

			constexpr std::strong_ordering operator<=>(const message<output_style>& other) const noexcept
				{
				return get_timestamp() <=> other.get_timestamp();
				}
			constexpr bool operator==(const message<output_style>& other) const noexcept
				{
				return get_timestamp() == other.get_timestamp();
				}

		private:
			constexpr message(msg_t type, const string::concepts::stringlike auto& string, std::chrono::time_point<std::chrono::system_clock> time) noexcept
				: type{type}, string{string}, time{time}
				{}
			constexpr message(msg_t type, const string::concepts::stringlike auto& string) noexcept
				: type{type}, string{string}, time{std::chrono::system_clock::now()}
				{}
			constexpr message(const string::concepts::stringlike auto& string) noexcept
				: type{msg_t::log}, string{string}, time{std::chrono::system_clock::now()}
				{}

			msg_t type = msg_t::log;
			std::string string{};
			std::chrono::time_point<std::chrono::system_clock> time;

			static constexpr std::string filter_last_newline(const std::string& string) noexcept
				{
				if (string.length() > 0 && string[string.length() - 1] == '\n') { return string.substr(0, string.length() - 1); }
				else { return string; }
				}

			friend std::ostream& operator<<(std::ostream& os, const message& m)
				{
				using namespace utils::output;

				constexpr size_t timestamp_digits{std::numeric_limits<decltype(m.time.time_since_epoch().count())>::digits10};

				//Data line
				if constexpr (output_style == output_style_t::tag_as_separator)
					{
					os << "_________________________________\n";
					os << ' ';

					os << utils::console::colour::background{utils::console::colour::colour_8::dark(m.out_type_color())};
					os << utils::console::colour::foreground{utils::console::colour::colour_8::bright(utils::graphics::colour::base::white)};
					os << std::left << m.out_type_verbose();
					os << utils::console::colour::restore_defaults;

					for (size_t i = 0; i < (12 - m.out_type_verbose().length()); i++) { os << ' '; }

					os << ' ' << std::right << std::setw(timestamp_digits) << m.time.time_since_epoch().count() << '\n';
					}

				//First line

				bool first_line{true};

				size_t index_beg = 0;

				//Lines
				while (index_beg < m.string.size())
					{
					size_t index_end = m.string.find_first_of('\n', index_beg);
					if (index_end == std::string::npos) { index_end = m.string.size(); }
					else { index_end++; }

					if constexpr (output_style == output_style_t::on_line)
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
							os << std::right << std::setw(timestamp_digits) << m.time.time_since_epoch().count();
							os << utils::console::colour::restore_defaults;

							first_line = false;
							}
						else
							{
							for (size_t i = 0; i < timestamp_digits - 1; i++) { os << ' '; }
							os << "|";
							}
						}

					os << ' ' << std::string_view(m.string).substr(index_beg, index_end - index_beg);

					index_beg = index_end;
					}

				return os << "\n";
				}
		};

	namespace concepts
		{
		template <typename T>
		concept message = std::same_as<T, utils::logging::message<T::output_style>>;
		}
	}