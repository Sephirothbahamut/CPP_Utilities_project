#include <vector>
#include <optional>
#include <iostream>
#include <chrono>

#include "Library/include/utils/math/math.h"
#include "Library/include/utils/aggregate.h"
#include "Library/include/utils/containers/aggregate_regions.h"
#include "Library/include/utils/console/initializer.h"
#include "Library/include/utils/console/colour.h"
#include "Library/include/utils/console/io.h"
#include "Library/include/utils/logging/logger.h"
#include "Library/include/utils/logging/progress_bar.h"

#include "Library/include/utils/math/rect.h"

#include <mutex>
#include <thread>
#include <random>
#include <algorithm>
#include <execution>


int main()
	{
	using logger_t = utils::logging::logger<utils::logging::message<utils::logging::output_style_t::tag_as_separator>>;
	logger_t logger;
	
	for (size_t i = 0; i < 100; i++)
		{
		logger.log("asd " + std::to_string(i));
		}


	const utils::math::rect<float> ra{1.f, 2.f, 3.f, 4.f};
	utils::math::rect<float> rb{ra};
	}






