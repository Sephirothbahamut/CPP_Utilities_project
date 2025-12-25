
#include <print>
#include <random>
#include <iostream>
#include <algorithm>
#include <execution>

#include <utils/math/vec.h>
#include <utils/logging/logger.h>
#include <utils/console/initializer.h>
#include <utils/logging/progress_bar.h>


std::random_device dev;
std::mt19937 rng(dev());

auto random_milliseconds()
	{
	std::uniform_int_distribution<std::mt19937::result_type> distribution(5, 100);
	return distribution(rng);
	}

void logger()
	{
	using namespace std::literals;

	utils::console::initializer console_initializer;
	std::cout << utils::console::colour::restore_defaults << std::endl;
	for (size_t i = 0; i < 256; i++)
		{
		std::cout << utils::console::colour::foreground{utils::graphics::colour::rgb_u{static_cast<uint8_t>(0), static_cast<uint8_t>(i), static_cast<uint8_t>(255 - i)}};
		std::cout << utils::console::colour::background{utils::graphics::colour::rgb_u{static_cast<uint8_t>(0), static_cast<uint8_t>(255 - i), static_cast<uint8_t>(i)}};
		std::cout << "#";
		}
	std::print(utils::console::colour::to_string(utils::console::colour::restore_defaults));

	utils::logging::logger<utils::logging::message<utils::logging::output_style_t::tag_as_separator>> logger;


	if (const auto _{logger.section("Unicode test")})
		{
		logger.log("Expected: 😄 \\u{1F604} (\\u1F604)");

		const char32_t codepoint{utils::string::parse_codepoint<char>("1F604")};
		const std::string string_with_codepoint{utils::string::codepoint_to_string<char>(codepoint)};
		logger.log("Output: \"" + string_with_codepoint + "\"");
		}

	logger.log("log Hello wolrd\nhow are you\ni'm alright thanks");
	logger.err("err Hello wolrd\nhow are you\ni'm alright thanks");
	logger.wrn("wrn Hello wolrd\nhow are you\ni'm alright thanks");
	logger.dgn("dgn Hello wolrd\nhow are you\ni'm alright thanks");
	logger.inf("inf Hello wolrd\nhow are you\ni'm alright thanks");

	if (true)
		{
		const auto _{logger.section("hello world")};
		logger.log("a");
		logger.log("b");
		logger.log("c");
		}


	



	
	if(true)
		{
		const auto _{logger.section("Progress bar testing")};

		utils::logging::progress_bar<> pb{utils::logging::progress_bar<>::create_info
			{
			.logger_ptr{&logger},
			.label{"Main"},
			.steps_count{3}
			}};
		for (size_t i = 0; i < 3; i++)
			{
			const auto step{pb.step()};
			std::this_thread::sleep_for(std::chrono::milliseconds{20});

			auto sub{pb.split(20)};
		
			try
				{
				for (size_t j = 0; j < 5; j++)
					{
					const auto step{sub.step()};
					std::this_thread::sleep_for(std::chrono::milliseconds{5});
		
					auto subsub{sub.split(10, "Subsub")};
					for (size_t k = 0; k < 10; k++)
						{
						const auto step{subsub.step()};
						std::this_thread::sleep_for(std::chrono::milliseconds{10});
		
						if (i == 1 && j == 2 && k == 3)
							{
							throw std::runtime_error{"Error at index " + std::to_string(i) + ", " + std::to_string(j) + ", " + std::to_string(k)};
							}
						}
					}
		
				for (size_t j = 0; j < 10; j++)
					{
					const auto step{sub.step()};
					std::this_thread::sleep_for(std::chrono::milliseconds{80});
					}
		
				const utils::math::vec2s matrix_size{size_t{20}, size_t{20}};
				const auto indices{matrix_size.indices_range()};
		
				auto subsub{sub.split(indices.size(), "Matrix iteration")};
				std::mutex matrix_mutex;
				std::for_each(std::execution::par_unseq, indices.begin(), indices.end(), [&i, &subsub, &matrix_mutex, &matrix_size](size_t index)
					{
					const auto sleep_milliseconds{std::chrono::milliseconds(1000)};
					std::this_thread::sleep_for(std::chrono::milliseconds{sleep_milliseconds});

					const std::unique_lock lock{matrix_mutex};
					subsub.advance();
					});
				}
			catch (const std::exception& e)
				{
				std::cout << "\n" << e.what() << "\n\n";
				sub.draw_new_lines();
				}
			}
		pb.draw_new_lines();
		}








	std::cout << utils::console::colour::restore_defaults << std::endl;
	utils::logging::logger<utils::logging::message<utils::logging::output_style_t::on_line>> logger2;

	logger2.log("log Hello wolrd\nhow are you\ni'm alright thanks");
	logger2.err("err Hello wolrd\nhow are you\ni'm alright thanks");
	logger2.wrn("wrn Hello wolrd\nhow are you\ni'm alright thanks");
	logger2.dgn("dgn Hello wolrd\nhow are you\ni'm alright thanks");
	logger2.inf("inf Hello wolrd\nhow are you\ni'm alright thanks");

	if (true)
		{
		const auto _{logger2.section("hello world")};
		logger2.log("a");
		logger2.log("b");
		logger2.log("c");
		}
	std::cout << "Terminating program" << std::endl;
	}