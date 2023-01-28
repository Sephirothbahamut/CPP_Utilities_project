#include <string>
#include <iostream>

#include "include/utils/console/colour.h"

#include "include/utils/math/vec.h"
#include "include/utils/math/vec2.h"
#include "include/utils/math/vec3.h"
#include "include/utils/output/std_containers.h"
#include "include/utils/math/angle.h"
#include "include/utils/graphics/colour.h"

//#include "include/utils/containers/matrix.h"
//#include "include/utils/logger.h"
//
//#include "include/utils/containers/handled_container.h"
//#include "include/utils/containers/multihandled.h"
//
//#include "include/utils/containers/hive/next.h"
//#include "include/utils/containers/linked_vector.h"
//#include "include/utils/containers/segmented_vector.h"
//
//#include "include/utils/containers/multithreading/multiqueue_consumer.h"
//
//#include "include/utils/containers/object_pool.h"
//
//#include "include/utils/oop/counting.h"

//using civ = utils::oop::counting_invalidating_move;

//struct test_struct : civ
//	{
//	test_struct(int v) : v{v} {}
//	int v;
//	};

//#include "include/utils/math/rect.h"
int main()
	{
	using namespace utils::output;
	//utils::math::rect<float> rf{.ll{0}, .up{0}, .rr{32}, .dw{32}};
	//
	//std::cout << "rect    : " << rf          << std::endl;
	//std::cout << "position: " << static_cast<utils::math::vec2f>(rf.position()) << std::endl;
	//std::cout << "size    : " << static_cast<utils::math::vec2f>(rf.size    ()) << std::endl;
	//
	//std::cout << "" << std::endl;
	//
	//std::cout << "rf.position.x += 5;" << std::endl;
	//rf.position().x += 5;
	//
	//std::cout << "rect    : " << rf          << std::endl;
	//std::cout << "position: " << static_cast<utils::math::vec2f>(rf.position()) << std::endl;
	//std::cout << "size    : " << static_cast<utils::math::vec2f>(rf.size    ()) << std::endl;
	//
	//std::cout << "rf.position.y -= 5;" << std::endl;
	//rf.position().y -= 5;
	//
	//std::cout << "rect    : " << rf          << std::endl;
	//std::cout << "position: " << static_cast<utils::math::vec2f>(rf.position()) << std::endl;
	//std::cout << "size    : " << static_cast<utils::math::vec2f>(rf.size    ()) << std::endl;
	//
	//std::cout << "rf.size.w += 5;" << std::endl;
	//rf.size().w += 5;
	//
	//std::cout << "rect    : " << rf          << std::endl;
	//std::cout << "position: " << static_cast<utils::math::vec2f>(rf.position()) << std::endl;
	//std::cout << "size    : " << static_cast<utils::math::vec2f>(rf.size    ()) << std::endl;
	//
	//std::cout << "rf.size.h -= 5;" << std::endl;
	//rf.size().h -= 5;
	//
	//std::cout << "rect    : " << rf          << std::endl;
	//std::cout << "position: " << static_cast<utils::math::vec2f>(rf.position()) << std::endl;
	//std::cout << "size    : " << static_cast<utils::math::vec2f>(rf.size    ()) << std::endl;
	//
	//std::cout << "rf.size.h = 24;" << std::endl;
	//rf.size().h = 24;
	//
	//std::cout << "rect    : " << rf << std::endl;
	//std::cout << "position: " << static_cast<utils::math::vec2f>(rf.position()) << std::endl;
	//std::cout << "size    : " << static_cast<utils::math::vec2f>(rf.size    ()) << std::endl;
	//
	//std::cout << "rf.pos += utils::math::vec2f{-5, 5};" << std::endl;
	//rf.pos() += utils::math::vec2<float>{-5, 5};
	//
	//std::cout << "rect    : " << rf << std::endl;
	//std::cout << "position: " << static_cast<utils::math::vec2f>(rf.position()) << std::endl;
	//std::cout << "size    : " << static_cast<utils::math::vec2f>(rf.size    ()) << std::endl;

	std::cout
		<< utils::console::colour::foreground::blue
		<< utils::console::colour::background::dark_white
		<< "Hello"
		<< utils::console::colour::background::white
		<< " "
		<< utils::console::colour::foreground::black
		<< utils::console::colour::background::yellow
		<< "world"
		<< utils::console::colour::foreground::white
		<< utils::console::colour::background::dark_green
		<< "!"
		<< utils::console::color::restore_defaults
		<< std::endl;
	
	utils::math::vec<float, 4> vec_a{.array{5.f, 0.f, 0.f, 5.f}};
	auto vec_b{utils::math::vec<float, 3>::from(vec_a)};
	auto vec_c{utils::math::vec<float, 2>::from(3.f, 2.f)};
	auto vec_d{utils::math::vec<float, 8>::from(vec_c, 1.f)};
	auto vec_e{!vec_a};

	std::cout << vec_a << "\n" << vec_b << "\n" << vec_c << "\n" << vec_d << "\n" << vec_e << std::endl;

	using namespace utils::details::vec;
	vec_a + 3.f;
	vec_b + vec_a;
	vec_a + vec_b;
	
	using namespace utils::output;
	std::cout << vec_a << "\n";
	
	using namespace utils::math::angle::literals;
	auto angle_deg{90_deg};
	auto angle_rad{90_rad};
	auto angle_sensicalrad{90_radpi};

	auto vec_f{utils::math::vec2f::from(angle_deg, 1.f)}; //TODO check, wrong result

	utils::math::angle::base<float, 1.f> other{ .5 };
	std::cout << angle_deg << "\n" << angle_rad << "\n" << angle_sensicalrad << "\n" << other << "\n" << vec_f << "\n";

	angle_deg + angle_rad;

	vec_c - angle_deg;

	auto colour_a{utils::graphics::colour::rgb_u::from(utils::graphics::colour::base::blue          )};
	auto colour_b{utils::graphics::colour::rgb_u::from(utils::graphics::colour::base::cyan, 128, 100)};
	utils::graphics::colour::rgb_u colour_c{utils::graphics::colour::rgb_u::from(vec_d)};
	
	std::cout
		<< colour_a << "\n"
		<< colour_b << "\n"
		<< colour_c << "\n";
	
	//utils::containers::mat2x2f mat2x2f{ 1 };
	//utils::containers::mat3x3f mat3x3f;
	//utils::containers::mat4x4f mat4x4f;
	//mat2x2f += mat2x2f;
	//std::cout << mat2x2f << "\n" << mat4x4f << "\n";
	//
	//
	//utils::globals::logger.err("hi");
	//utils::globals::logger.wrn("how");
	//utils::globals::logger.log("are");
	//utils::globals::logger.dgn("you");
	//utils::globals::logger.inf("?");
	//
	//utils::containers::handled_container<int> handled;
	//
	//auto handle_0{ handled.emplace(0) };
	//auto handle_1{ handled.emplace(1) };
	//auto handle_2{ handled.emplace(2) };
	//
	//handled.erase(handle_1);
	//
	//auto handle_3{ handled.emplace(3) };
	//
	//
	//utils::containers::multithreading::consumption_delegating_queue<int  > queue_a{[](int  & i) { std::cout << "int   " << i << std::endl; }};
	//utils::containers::multithreading::consumption_delegating_queue<float> queue_b{[](float& c) { std::cout << "float " << c << std::endl; }};
	//
	////utils::containers::multithreading::multiqueue_consumer consumer<int, utils::containers::multithreading::operation_flag_bits::pre>{[]() {}, []() {}};
	//utils::containers::multithreading::multiqueue_consumer consumer{ []() { std::cout << "pre " << std::endl; }, []() { std::cout << "post " << std::endl; } };
	//consumer.bind(queue_a);
	//consumer.bind(queue_b);
	//
	//for (size_t i = 0; true; i++)
	//	{
	//	queue_a.push(i);
	//	queue_b.push('a' + i);
	//
	//	using namespace std::chrono_literals;
	//	std::this_thread::sleep_for(.1s);
	//	}
	}