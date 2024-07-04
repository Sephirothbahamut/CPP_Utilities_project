#include <utils/logging/logger.h>
#include <utils/console/initializer.h>

void logger()
	{
	using namespace std::literals;

	utils::console::initializer console_initializer;
	std::cout << utils::console::colour::restore_defaults << std::endl;
	for (size_t i = 0; i < 256; i++)
		{
		std::cout << utils::console::colour::foreground{utils::graphics::colour::rgb_u{static_cast<uint8_t>(0), static_cast<uint8_t>(i      ), static_cast<uint8_t>(255 - i)}};
		std::cout << utils::console::colour::background{utils::graphics::colour::rgb_u{static_cast<uint8_t>(0), static_cast<uint8_t>(255 - i), static_cast<uint8_t>(i      )}};
		std::cout << "#";
		}
	std::cout << utils::console::colour::restore_defaults << std::endl;
	
	utils::logging::logger<utils::logging::message<utils::logging::output_style_t::tag_as_separator>> logger;
	
	logger.log("log Hello wolrd\nhow are you\ni'm alright thanks");
	logger.err("err Hello wolrd\nhow are you\ni'm alright thanks");
	logger.wrn("wrn Hello wolrd\nhow are you\ni'm alright thanks");
	logger.dgn("dgn Hello wolrd\nhow are you\ni'm alright thanks");
	logger.inf("inf Hello wolrd\nhow are you\ni'm alright thanks");
	logger.flush();
	
	std::cout << utils::console::colour::restore_defaults << std::endl;
	utils::logging::logger<utils::logging::message<utils::logging::output_style_t::on_line>> logger2;
	
	logger2.log("log Hello wolrd\nhow are you\ni'm alright thanks");
	logger2.err("err Hello wolrd\nhow are you\ni'm alright thanks");
	logger2.wrn("wrn Hello wolrd\nhow are you\ni'm alright thanks");
	logger2.dgn("dgn Hello wolrd\nhow are you\ni'm alright thanks");
	logger2.inf("inf Hello wolrd\nhow are you\ni'm alright thanks");
	
	std::cout << "Terminating program" << std::endl;
	}