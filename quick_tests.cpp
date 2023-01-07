#include <string>
#include <iostream>

#include "include/utils/console/colour.h"

#include "include/utils/containers/memberwise_operators_array.h"
#include "include/utils/containers/memberwise_operators/seq.h"
#include "include/utils/containers/memberwise_operators/par.h"
#include "include/utils/math/vec.h"
#include "include/utils/output/std_containers.h"
#include "include/utils/math/angle.h"
#include "include/utils/graphics/colour.h"

#include "include/utils/containers/matrix.h"
#include "include/utils/logger.h"

#include "include/utils/containers/handled_container.h"
#include "include/utils/containers/multihandled.h"

#include "include/utils/containers/hive/next.h"
#include "include/utils/containers/linked_vector.h"
#include "include/utils/containers/segmented_vector.h"

#include "include/utils/containers/multithreading/multiqueue_consumer.h"

#include "include/utils/containers/object_pool.h"

#include "include/utils/oop/counting.h"

using civ = utils::oop::counting_invalidating_move;

struct test_struct : civ
	{
	test_struct(int v) : v{v} {}
	int v;
	};

#include "include/utils/math/rect.h"
int main()
	{
	using namespace utils::output;
	utils::math::rect<float> rf{.ll{0}, .up{0}, .rr{32}, .dw{32}};

	std::cout << "rect    : " << rf          << std::endl;
	std::cout << "position: " << static_cast<utils::math::vec2f>(rf.position()) << std::endl;
	std::cout << "size    : " << static_cast<utils::math::vec2f>(rf.size    ()) << std::endl;

	std::cout << "" << std::endl;
	
	std::cout << "rf.position.x += 5;" << std::endl;
	rf.position().x += 5;
	
	std::cout << "rect    : " << rf          << std::endl;
	std::cout << "position: " << static_cast<utils::math::vec2f>(rf.position()) << std::endl;
	std::cout << "size    : " << static_cast<utils::math::vec2f>(rf.size    ()) << std::endl;

	std::cout << "rf.position.y -= 5;" << std::endl;
	rf.position().y -= 5;
	
	std::cout << "rect    : " << rf          << std::endl;
	std::cout << "position: " << static_cast<utils::math::vec2f>(rf.position()) << std::endl;
	std::cout << "size    : " << static_cast<utils::math::vec2f>(rf.size    ()) << std::endl;

	std::cout << "rf.size.w += 5;" << std::endl;
	rf.size().w += 5;
	
	std::cout << "rect    : " << rf          << std::endl;
	std::cout << "position: " << static_cast<utils::math::vec2f>(rf.position()) << std::endl;
	std::cout << "size    : " << static_cast<utils::math::vec2f>(rf.size    ()) << std::endl;

	std::cout << "rf.size.h -= 5;" << std::endl;
	rf.size().h -= 5;

	std::cout << "rect    : " << rf          << std::endl;
	std::cout << "position: " << static_cast<utils::math::vec2f>(rf.position()) << std::endl;
	std::cout << "size    : " << static_cast<utils::math::vec2f>(rf.size    ()) << std::endl;

	std::cout << "rf.size.h = 24;" << std::endl;
	rf.size().h = 24;

	std::cout << "rect    : " << rf << std::endl;
	std::cout << "position: " << static_cast<utils::math::vec2f>(rf.position()) << std::endl;
	std::cout << "size    : " << static_cast<utils::math::vec2f>(rf.size    ()) << std::endl;

	std::cout << "rf.pos += utils::math::vec2f{-5, 5};" << std::endl;
	rf.pos() += utils::math::vec2<float>{-5, 5};

	std::cout << "rect    : " << rf << std::endl;
	std::cout << "position: " << static_cast<utils::math::vec2f>(rf.position()) << std::endl;
	std::cout << "size    : " << static_cast<utils::math::vec2f>(rf.size    ()) << std::endl;

	std::cout
		<< utils::console::colour::foreground::blue
		<< utils::console::colour::background::dark_white
		<< "Hello"
		<< utils::console::colour::background::white
		<< " "
		<< utils::console::colour::foreground::white
		<< utils::console::colour::background::yellow
		<< "world"
		<< utils::console::colour::foreground::white
		<< utils::console::colour::background::green
		<< "!"
		<< utils::console::color::restore_defaults;
	
	utils::math::vec<float, 4> a{ 5.f, 5.f, 5.f, 5.f };
	utils::math::vec<float, 3> b{ 0.f, 1.f, 2.f };
	
	a + 3.f;
	b + a;
	a + b;
	
	using namespace utils::containers::memberwise_operators::seq;
	std::vector<float> stdarr_a;
	std::vector<float> stdarr_out{ 1.f - stdarr_a + -stdarr_a };
	
	
	using namespace utils::output;
	std::cout << a << "\n";
	
	using namespace utils::math::angle::literals;
	auto deg{ 90_deg };
	auto rad{ 90_rad };
	auto sensicalrad{ 90_radpi };
	
	utils::math::angle::base<float, 1.f> other{ .5 };
	//std::cout << deg << ", " << rad << ", " << sensicalrad << ", " << other << "\n";
	
	utils::graphics::colour::rgb_u colour_a{ utils::graphics::colour::base::blue };
	utils::graphics::colour::rgb_u colour_b{ utils::graphics::colour::base::cyan   , 128, 100 };
	utils::graphics::colour::rgb_u colour_c{ utils::graphics::colour::base::yellow , 200 };
	utils::graphics::colour::rgb_u colour_d{ utils::graphics::colour::base::magenta, 127 };
	
	std::cout
		<< colour_a << "\n"
		<< colour_b << "\n"
		<< colour_c << "\n"
		<< colour_d << "\n";
	
	utils::containers::mat2x2f mat2x2f{ 1 };
	utils::containers::mat3x3f mat3x3f;
	utils::containers::mat4x4f mat4x4f;
	mat2x2f += mat2x2f;
	std::cout << mat2x2f << "\n" << mat4x4f << "\n";
	
	
	utils::globals::logger.err("hi");
	utils::globals::logger.wrn("how");
	utils::globals::logger.log("are");
	utils::globals::logger.dgn("you");
	utils::globals::logger.inf("?");
	
	utils::containers::handled_container<int> handled;
	
	auto handle_0{ handled.emplace(0) };
	auto handle_1{ handled.emplace(1) };
	auto handle_2{ handled.emplace(2) };
	
	handled.erase(handle_1);
	
	auto handle_3{ handled.emplace(3) };
	
	
	utils::containers::multithreading::consumption_delegating_queue<int  > queue_a{[](int  & i) { std::cout << "int   " << i << std::endl; }};
	utils::containers::multithreading::consumption_delegating_queue<float> queue_b{[](float& c) { std::cout << "float " << c << std::endl; }};
	
	//utils::containers::multithreading::multiqueue_consumer consumer<int, utils::containers::multithreading::operation_flag_bits::pre>{[]() {}, []() {}};
	utils::containers::multithreading::multiqueue_consumer consumer{ []() { std::cout << "pre " << std::endl; }, []() { std::cout << "post " << std::endl; } };
	consumer.bind(queue_a);
	consumer.bind(queue_b);
	
	for (size_t i = 0; true; i++)
		{
		queue_a.push(i);
		queue_b.push('a' + i);
	
		using namespace std::chrono_literals;
		std::this_thread::sleep_for(.1s);
		}
	}