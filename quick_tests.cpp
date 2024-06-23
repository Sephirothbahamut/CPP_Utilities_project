#include <string>
#include <iostream>

#include "include/utils/memory.h"

#include "include/utils/console/io.h"
#include "include/utils/console/colour.h"
#include "include/utils/console/initializer.h"

#include "include/utils/math/vec.h"
#include "include/utils/math/vec2.h"
#include "include/utils/math/vec3.h"
#include "include/utils/math/angle.h"
#include "include/utils/math/rect.h"

#include "include/utils/output/std_containers.h"
#include "include/utils/graphics/colour.h"

#include "include/utils/matrix_interface.h"
#include "include/utils/logging/logger.h"

#include "include/utils/clock.h"
//#include "include/utils/containers/handled_container.h"
//#include "include/utils/containers/multihandled.h"

//#include "include/utils/containers/hive/next.h"
//#include "include/utils/containers/linked_vector.h"
//#include "include/utils/containers/segmented_vector.h"
//
//#include "include/utils/containers/multithreading/multiqueue_consumer.h"
//
#include "include/utils/containers/object_pool.h"
//
//#include "include/utils/oop/counting.h"
//
//#include "include/utils/thread_pool.h"
//
#include "include/utils/beta/containers/multihandled_default.h"
//
#include "include/utils/name.h"
//
#include "include/utils/tracking.h"
//#include "include/utils/containers/resource_manager.h"

#include "include/utils/storage.h"

//#include "include/utils/beta/math/geometry/all.h"
//#include "include/utils/beta/math/geometry/group.h"
//#include "include/utils/math/transform2.h"
#include "include/utils/math/geometry/shape/point.h"
#include "include/utils/math/geometry/shape/aabb.h"
#include "include/utils/math/geometry/shape/ab.h"
#include "include/utils/math/geometry/shape/polyline.h"

#include "include/utils/math/geometry/interactions.h"

#include "include/utils/output/vec.h"

//using civ = utils::oop::counting_invalidating_move;
//
//struct test_struct : civ
//	{
//	test_struct(int v) : v{v} {}
//	int v;
//	};



struct angry_type
	{
	angry_type(int i, float) : i{i} {}
	int i;
	};

struct child_type : angry_type
	{
	child_type(int i) : angry_type{i, 0.f}, another_i{i * 2} {}
	int another_i;
	};


int main()
	{
	utils::console::initializer utils_console_initializer;
	using namespace utils::output;
	using namespace utils::math::angle::literals;

	std::vector<utils::math::vec2f> vertices
		{
		utils::math::vec2f{0.f, 0.f},
		utils::math::vec2f{100.f, 0.f},
		utils::math::vec2f{100.f, 100.f},
		utils::math::vec2f{0.f, 100.f},
		utils::math::vec2f{0.f, 0.f},
		utils::math::vec2f{100.f, 0.f},
		utils::math::vec2f{50.f, 50.f}
		};

	using span_t = utils::math::geometry::shape::observer::polygon<3>::storage_t::inner_storage_t;
	
	utils::math::geometry::shape::polygon<std::dynamic_extent> polyline{utils::math::vec2f{0.f, 0.f}, utils::math::vec2f{100.f, 0.f}, utils::math::vec2f{50.f, 50.f}};
	utils::math::geometry::shape::observer::polygon<4> triangle_a{vertices.begin(), size_t{4}};
	utils::math::geometry::shape::observer::polygon<3> triangle_b{vertices.begin() + 4, size_t{3}};

	utils::math::geometry::interactions::translate_self(polyline, {5.f, 5.f});
	
	utils::math::geometry::interactions::translate_self(triangle_a, {5.f, 5.f});
	utils::math::geometry::interactions::scale_self    (triangle_b, .5f);
	utils::math::geometry::interactions::translate_self(triangle_b, {20.f, 20.f});


	const auto console_size{utils::console::size()};
	std::stringstream console_stream;

	for (size_t y{0}; y < console_size.y(); y++)
		{
		//console_stream << "\n";
		for (size_t x{0}; x < console_size.x(); x++)
			{
			const utils::math::vec2s coords_indices{x, y};
			const utils::math::vec2f coords_f
				{
				static_cast<float>(coords_indices.x()),
				static_cast<float>(coords_indices.y() * 2.f)
				};
	
			const auto shape{polyline};
			const auto sdist_a{utils::math::geometry::interactions::signed_distance(triangle_a, coords_f)};
			const auto sdist_b{utils::math::geometry::interactions::signed_distance(triangle_b, coords_f)};

			const auto gdist_a{utils::math::geometry::interactions::gradient_signed_distance(triangle_a, coords_f)};
			const auto gdist_b{utils::math::geometry::interactions::gradient_signed_distance(triangle_b, coords_f)};
			const auto gdist  {utils::math::geometry::interactions::return_types::gradient_signed_distance::merge(gdist_a, gdist_b)};

			const auto min_dist{std::min(sdist_a.absolute(), sdist_b.absolute())};
			const auto sdist{utils::math::geometry::interactions::return_types::signed_distance::subtract(sdist_a, sdist_b)};
			
			const float value{sdist.absolute() / 16.f};
	
			const utils::graphics::colour::rgb_f colour
				{
				sdist.side().is_inside() ? value : 0.f, 
				value, 
				sdist.side().is_outside() ? value : 0.f
				};

			const auto normalized_gradient{gdist.gradient.normalize()};
			const auto positive_gradient{(normalized_gradient / 2.f) + 1.f};

			const utils::graphics::colour::rgb_f colour_gradient
				{
				value,
				normalized_gradient.x() * value,
				normalized_gradient.y() * value,
				};

			const utils::graphics::colour::rgb_u colour_8
				{
				static_cast<uint8_t>(colour_gradient[0] * 255.f),
				static_cast<uint8_t>(colour_gradient[1] * 255.f),
				static_cast<uint8_t>(colour_gradient[2] * 255.f)
				};
			
			console_stream << utils::console::colour::foreground{colour_8} << utils::console::colour::background{colour_8} << "#";
			}
		}
	
	std::cout << console_stream.str() << utils::console::colour::restore_defaults << std::endl;

//	
//	utils::trackable_wrapper<child_type> child1{1};
//	utils::trackable_wrapper<angry_type> parent1{1, 2.f};
//	
//	utils::tracking_ptr<child_type> child_tracker{child1};
//	utils::tracking_ptr<angry_type> child_as_parent_tracker{child1};
//	utils::tracking_ptr<angry_type> parent_tracker{parent1};
//	
//	auto child2{std::move(child1)};
//	auto parent2{std::move(parent1)};
//	
//	child1 ->another_i = -1;
//	child1 ->        i = -1;
//	parent1->        i = -1;
//	
//	child1 = utils::trackable_wrapper<child_type>{2};
//	
//	std::cout << child_tracker          ->another_i << std::endl;
//	std::cout << child_as_parent_tracker->        i << std::endl;
//	std::cout <<          parent_tracker->        i << std::endl;
//	
//	enum class example_enum
//		{
//		a = 0b00000001,
//		b = 0b00000010,
//		c = 0b00000100,
//		d = 0b00001000
//		};
//	auto f{[](const utils::flags<example_enum>& flags)
//		{
//		std::cout << flags.test(example_enum::a) << std::endl;
//		}};
//	
//	utils::flags<example_enum> flags{example_enum::a, example_enum::b};
//	flags |= example_enum::d;
//	flags.flip(example_enum::a);
//	flags.reset(example_enum::b);
//	flags = example_enum::a | flags;
//	
//	std::cout << flags.test(example_enum::a) << std::endl;
//	std::cout << flags.test(example_enum::b) << std::endl;
//	std::cout << flags.test(example_enum::c) << std::endl;
//	std::cout << flags.test(example_enum::d) << std::endl;
//	
//	f(flags.flip(example_enum::a));
//	
//	utils::name hello{"hello"};
//	
//	std::unordered_map<utils::name, int> map;
//	
//	map[hello] = 5;
//	map["hello"] = 1;
//	map["world"] = 2;
//	
//	std::cout << map[hello];
//	std::cout << map["world"];
//	std::cout << ((hello == "halo") ? "true" : "false");
//	std::cout << std::endl;
//	
//	
//	//utils::containers::object_pool<angry_type> md;
//	//auto u{md.make_unique(3, 2.f)};
//	//auto s{md.make_shared(3, 2.f)};
//	//auto s2{s};
//	//auto r{md.emplace<utils::containers::object_pool_handle_version::raw>(3, 2.f)};
//	//auto u2{md.emplace<utils::containers::object_pool_handle_version::unique>(3, 2.f)};
//	
//	utils::containers::multihandled_default<angry_type> md{5, 2.f};
//	
//	utils::containers::object_pool
//		<
//		angry_type, 8,
//		utils::flags<utils::containers::object_pool_handle_version>::full()
//		> op;
//	
//	auto u{md.make_unique(3, 2.f)};
//	
//	try
//		{
//		md.emplace_in_handle(u, 3, 2.f);
//		}
//	catch (const std::exception& e)
//		{
//		std::cout << e.what() << std::endl;
//		}
//	
//	utils::containers::multihandled_default<angry_type>::handle_shared s{md.clone_default<utils::containers::object_pool_handle_version::shared>()};
//	
//	md.emplace_in_handle(s, 55, 32.f);
//	
//	try
//		{
//		md.emplace_in_handle(s, 55, 32.f);
//		}
//	catch (const std::exception& e)
//		{
//		std::cout << e.what() << std::endl;
//		}
//	
//	utils::console::initializer console_initializer;
//	
	//std::cout << utils::console::colour::restore_defaults << std::endl;
	//for (size_t i = 0; i < 256; i++)
	//	{
	//	std::cout << utils::console::colour::foreground{utils::graphics::colour::rgb_u{0, i, 255 - i}};
	//	std::cout << utils::console::colour::background{utils::graphics::colour::rgb_u{0, 255 - i, i}};
	//	std::cout << "#";
	//	}
	//std::cout << utils::console::colour::restore_defaults << std::endl;
	
	//utils::logging::logger<utils::logging::message<utils::logging::output_style_t::tag_as_separator>> logger;
	//
	//logger.log("log Hello wolrd\nhow are you\ni'm alright thanks");
	//logger.err("err Hello wolrd\nhow are you\ni'm alright thanks");
	//logger.wrn("wrn Hello wolrd\nhow are you\ni'm alright thanks");
	//logger.dgn("dgn Hello wolrd\nhow are you\ni'm alright thanks");
	//logger.inf("inf Hello wolrd\nhow are you\ni'm alright thanks");
	//logger.flush();
	//
	//std::cout << utils::console::colour::restore_defaults << std::endl;
	//utils::logging::logger<utils::logging::message<utils::logging::output_style_t::on_line>> logger2;
	//
	//logger2.log("log Hello wolrd\nhow are you\ni'm alright thanks");
	//logger2.err("err Hello wolrd\nhow are you\ni'm alright thanks");
	//logger2.wrn("wrn Hello wolrd\nhow are you\ni'm alright thanks");
	//logger2.dgn("dgn Hello wolrd\nhow are you\ni'm alright thanks");
	//logger2.inf("inf Hello wolrd\nhow are you\ni'm alright thanks");
	//
	//std::cout << "Terminating program" << std::endl;

	return 0;
	}
