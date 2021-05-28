#pragma once
#include <chrono>
#include <string>

namespace utils
	{
	template <typename Clock_type = std::chrono::steady_clock>
	class Timer
		{
		public:
			using clock_t = Clock_type;

			Timer() noexcept : time_point{clock_t::now()} {}

			template <typename Time_unit = std::chrono::nanoseconds, typename Retval_t = double>
			Retval_t elapsed_time() const noexcept
				{
				auto now = Clock_type::now();
				return static_cast<Retval_t>(duration_cast<Time_unit>(now - time_point).count());
				}

			template <typename Time_unit = std::chrono::nanoseconds, typename Retval_t = double>
			Retval_t reset() noexcept { auto t = elapsed_time(); zero(); return t; }

			void zero() { time_point = clock_t::now(); }

		private:
			std::chrono::time_point<clock_t> time_point;
		};

	template <typename Clock_type = std::chrono::steady_clock>
	class Timer_printer : public Timer<Clock_type>
		{
		public:
			using clock_t = Clock_type;

			Timer_printer(const std::string& string) noexcept : message{string} {}
			~Timer_printer() noexcept
				{ 
				auto t = Timer<Clock_type>::elapsed_time(); 
				print(t, ""); 
				}

			template <typename Time_unit = std::chrono::nanoseconds, typename Retval_t = double>
			Retval_t elapsed_time(const std::string& next_string = "") noexcept
				{
				auto t = Timer<Clock_type>::template elapsed_time<Time_unit, Retval_t>();
				print(t, next_string);
				return t;
				}

			template <typename Time_unit = std::chrono::nanoseconds, typename Retval_t = double>
			Retval_t reset(const std::string& next_string = "") noexcept 
				{
				auto t = elapsed_time<Time_unit, Retval_t>();
				print(t, next_string);
				Timer<Clock_type>::zero();
				return t;
				}

		private:
			template <typename print_t = double>
			void print(print_t print, const std::string& new_string)
				{
				if (message != "") { std::cout << message << std::fixed << print << std::endl; }
				message = new_string;
				}

			std::string message;
		};
	}