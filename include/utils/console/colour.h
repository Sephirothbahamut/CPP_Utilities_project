#pragma once

#include <iostream>
#include <stdexcept>

namespace utils::console::colour
	{
	enum class base { black, red, green, yellow, blue, magenta, cyan, white };

	struct root_t
		{
		base base{base::white};
		bool dark{false};
		};

	using  foreground_t = root_t;
	struct background_t : root_t {};
	struct restore_defaults_t {};

	inline constexpr restore_defaults_t restore_defaults;

	namespace foreground
		{
		static constexpr foreground_t black       {base::black  , false};
		static constexpr foreground_t red         {base::red    , false};
		static constexpr foreground_t green       {base::green  , false};
		static constexpr foreground_t yellow      {base::yellow , false};
		static constexpr foreground_t blue        {base::blue   , false};
		static constexpr foreground_t magenta     {base::magenta, false};
		static constexpr foreground_t cyan        {base::cyan   , false};
		static constexpr foreground_t white       {base::white  , false};
		static constexpr foreground_t dark_black  {base::black  , true };
		static constexpr foreground_t dark_red    {base::red    , true };
		static constexpr foreground_t dark_green  {base::green  , true };
		static constexpr foreground_t dark_yellow {base::yellow , true };
		static constexpr foreground_t dark_blue   {base::blue   , true };
		static constexpr foreground_t dark_magenta{base::magenta, true };
		static constexpr foreground_t dark_cyan   {base::cyan   , true };
		static constexpr foreground_t dark_white  {base::white  , true };
		static constexpr foreground_t bright_black     = black  ;
		static constexpr foreground_t bright_red       = red    ;
		static constexpr foreground_t bright_green     = green  ;
		static constexpr foreground_t bright_yellow    = yellow ;
		static constexpr foreground_t bright_blue      = blue   ;
		static constexpr foreground_t bright_magenta   = magenta;
		static constexpr foreground_t bright_cyan      = cyan   ;
		static constexpr foreground_t bright_white     = white  ;
		}
	namespace background
		{
		inline constexpr background_t black       {base::black  , false};
		inline constexpr background_t red         {base::red    , false};
		inline constexpr background_t green       {base::green  , false};
		inline constexpr background_t yellow      {base::yellow , false};
		inline constexpr background_t blue        {base::blue   , false};
		inline constexpr background_t magenta     {base::magenta, false};
		inline constexpr background_t cyan        {base::cyan   , false};
		inline constexpr background_t white       {base::white  , false};
		inline constexpr background_t dark_black  {base::black  , true };
		inline constexpr background_t dark_red    {base::red    , true };
		inline constexpr background_t dark_green  {base::green  , true };
		inline constexpr background_t dark_yellow {base::yellow , true };
		inline constexpr background_t dark_blue   {base::blue   , true };
		inline constexpr background_t dark_magenta{base::magenta, true };
		inline constexpr background_t dark_cyan   {base::cyan   , true };
		inline constexpr background_t dark_white  {base::white  , true };
		inline constexpr background_t bright_black     = black  ;
		inline constexpr background_t bright_red       = red    ;
		inline constexpr background_t bright_green     = green  ;
		inline constexpr background_t bright_yellow    = yellow ;
		inline constexpr background_t bright_blue      = blue   ;
		inline constexpr background_t bright_magenta   = magenta;
		inline constexpr background_t bright_cyan      = cyan   ;
		inline constexpr background_t bright_white     = white  ;
		}

	inline constexpr auto brace{foreground::bright_black}; 
	inline constexpr auto separ{foreground::dark_white  };
	inline constexpr auto value{foreground::bright_white };
	inline constexpr auto type {foreground::dark_green   };
	}

namespace utils::console
	{
	namespace color = colour; //for americans
	}

namespace utils::output
	{
	inline std::ostream& operator<< (std::ostream& stream, const utils::console::colour::foreground_t& colour) noexcept
		{
		if (stream.rdbuf() != std::cout.rdbuf()) { return stream; }
		switch (colour.base)
			{
			case utils::console::colour::base::black  : return stream << "\x1B[" << (colour.dark ? "30" : "90") << "m";
			case utils::console::colour::base::red    : return stream << "\x1B[" << (colour.dark ? "31" : "91") << "m";
			case utils::console::colour::base::green  : return stream << "\x1B[" << (colour.dark ? "32" : "92") << "m";
			case utils::console::colour::base::yellow : return stream << "\x1B[" << (colour.dark ? "33" : "93") << "m";
			case utils::console::colour::base::blue   : return stream << "\x1B[" << (colour.dark ? "34" : "94") << "m";
			case utils::console::colour::base::magenta: return stream << "\x1B[" << (colour.dark ? "35" : "95") << "m";
			case utils::console::colour::base::cyan   : return stream << "\x1B[" << (colour.dark ? "36" : "96") << "m";
			case utils::console::colour::base::white  : return stream << "\x1B[" << (colour.dark ? "37" : "97") << "m";
			}
		return stream;
		}
	inline std::ostream& operator<< (std::ostream& stream, const utils::console::colour::background_t& colour) noexcept
		{
		if (stream.rdbuf() != std::cout.rdbuf()) { return stream; }
		switch (colour.base)
			{
			case utils::console::colour::base::black  : return stream << "\x1B[" << (colour.dark ? "40" : "100") << "m";
			case utils::console::colour::base::red    : return stream << "\x1B[" << (colour.dark ? "41" : "101") << "m";
			case utils::console::colour::base::green  : return stream << "\x1B[" << (colour.dark ? "42" : "102") << "m";
			case utils::console::colour::base::yellow : return stream << "\x1B[" << (colour.dark ? "43" : "103") << "m";
			case utils::console::colour::base::blue   : return stream << "\x1B[" << (colour.dark ? "44" : "104") << "m";
			case utils::console::colour::base::magenta: return stream << "\x1B[" << (colour.dark ? "45" : "105") << "m";
			case utils::console::colour::base::cyan   : return stream << "\x1B[" << (colour.dark ? "46" : "106") << "m";
			case utils::console::colour::base::white  : return stream << "\x1B[" << (colour.dark ? "47" : "107") << "m";
			}
		return stream;
		}
	inline std::ostream& operator<< (std::ostream& stream, const utils::console::colour::restore_defaults_t&) noexcept
		{
		if (stream.rdbuf() != std::cout.rdbuf()) { return stream; }
		return stream << "\033[0m";
		}
	}