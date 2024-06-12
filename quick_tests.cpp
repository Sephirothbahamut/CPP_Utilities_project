#include <string>
#include <iostream>

#include "include/utils/memory.h"
#include "include/utils/console/initializer.h"
#include "include/utils/console/colour.h"

#include "include/utils/math/vec.h"
#include "include/utils/math/vec2.h"
#include "include/utils/math/vec3.h"
#include "include/utils/math/angle.h"
#include "include/utils/math/rect.h"

#include "include/utils/output/std_containers.h"
#include "include/utils/graphics/colour.h"

//#include "include/utils/containers/matrix.h"
#include "include/utils/logging/logger.h"

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
#include "include/utils/beta/math/geometry/shape/point.h"
#include "include/utils/beta/math/geometry/shape/aabb.h"
//#include "include/utils/beta/math/geometry/shape/ab.h"

#include "include/utils/beta/math/geometry/shape/implementation/point.h"
#include "include/utils/beta/math/geometry/shape/implementation/aabb.h"
//#include "include/utils/beta/math/geometry/shape/implementation/ab.h"

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

	static_assert(sizeof(utils::math::vec2f) == (sizeof(float) * 2));

	utils::math::vec2f::storage_t base_a{1.f};
	utils::math::vec2f::storage_t base_b{1.f, 2.f};

	utils::math::vec2f    vec2f_a;
	vec2f_a.x() = vec2f_a.y() = 10.f;
	utils::math::vec2f    vec2f_b{1.f};
	utils::math::vec2f    vec2f_c{1.f, 2.f};
	utils::math::vec2f    vec2f_d{vec2f_a};
	utils::math::vec2f    vec2f_e{vec2f_a +  vec2f_b};
	utils::math::vec2f    vec2f_f{vec2f_a += vec2f_b};
	utils::math::vecref2f vec2f_g{vec2f_a};
	utils::math::vecref2f vec2f_h{vec2f_g *= 2.f};

	utils::math::vec2f::storage_t::inner_create(vec2f_a);
	utils::math::vec<float&, 2> vec2f_i{utils::math::vecref2f{vec2f_a}};
	utils::math::vec2f vec2f_j{vec2f_i};

//	using namespace utils::output;
//	using namespace utils::math::angle::literals;
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
//	utils::math::geometry::shape::point point{0.f, 1.f};
//	const auto result{point + 1.f};
//	const auto& result2{point += point};
//	const auto& result3{point += 3.f};
//	utils::math::transform2 transform{.translation{10.f, 2.f}, .rotation{12_rad}};
//	
//	const auto ret{point.transform(transform)};
//	
//	
//	utils::math::geometry::shape::point pa{3.f, 1.f};
//	const utils::math::geometry::shape::point pb{1.f, 3.f};
//	utils::math::geometry::shape::point pc{5.f, 2.f};
//	utils::math::geometry::shape::point pd{pa};
//	utils::math::geometry::shape::point pe{pb};
//	utils::math::geometry::shape::point pf{pa.x(), pa.y()};
//	utils::math::geometry::shape::point pg{pb.x(), pa.y()};
//	
//	//const auto aabb{utils::math::geometry::shape::aabb::create::from_vertices(pa, pb)};
//	float fa, fb;
//	utils::math::geometry::shape::observer::point po(point);
//	utils::math::geometry::shape::const_observer::point pco{point};
//	utils::math::geometry::shape::point pco_to_p{pco};
//	
//	std::cout << std::endl;

	//utils::math::geometry::shape::circle circle{utils::math::vec2f{0.f, 0.f}, 5.f};
	//utils::math::geometry::shape::point point{0.f, 1.f};
	//utils::math::geometry::shape::segment segment{point, point};
	//
	//using variant_t = std::variant
	//	<
	//	utils::math::geometry::shape::point,
	//	utils::math::geometry::shape::circle,
	//	utils::math::geometry::shape::segment
	//	>;
	//std::vector<variant_t> shapes;
	//shapes.emplace_back(utils::math::geometry::shape::point {0.f, 0.1f});
	//shapes.emplace_back(utils::math::geometry::shape::circle{{0.f, 0.f}, 5.f});
	//shapes.emplace_back(utils::math::geometry::shape::segment{{.5f, -1.f}, {.5f, 1.f}});
	//
	//for (const auto& shape_a : shapes)
	//	{
	//	for (const auto& shape_b : shapes)
	//		{
	//		bool collides{false};
	//
	//		std::visit([&](const auto& shape_a)
	//			{
	//			std::visit([&](const auto& shape_b)
	//				{
	//				using a_shape_t = std::remove_reference<decltype(shape_a)>::type;
	//				using b_shape_t = std::remove_reference<decltype(shape_b)>::type;
	//				
	//				//shape_a.contains             (shape_b);
	//				//shape_a.distance             (shape_b);
	//				//shape_a.side_of              (shape_b);
	//				//shape_a.closest_pair         (shape_b);
	//				//shape_a.closest_point_to     (shape_b);
	//				//shape_a.closest_with_distance(shape_b);
	//				//
	//				//shape_a.intersects           (shape_b);
	//				//shape_a.intersection_with    (shape_b);
	//				//shape_a.collides_with        (shape_b);
	//				}, shape_b);
	//			}, shape_a);
	//		}
	//	}
	//
	//for (const auto& shape : shapes)
	//	{
	//	std::visit([&](const auto& shape)
	//		{
	//		std::cout << point.distance_signed(shape).value << std::endl;
	//		}, shape);
	//	}

	//std::cout << "utils::math::rect<float>: " << sizeof(utils::math::rect<float>) << std::endl;
	//std::cout << "expected:                 " << (sizeof(float) * 4) << std::endl;
	//std::cout << "utils::math::vec2f:       " << sizeof(utils::math::vec2f) << std::endl;
	//std::cout << "expected:                 " << (sizeof(float) * 2) << std::endl;
	//std::cout << "circle:                   " << sizeof(utils::math::geometry::shape::circle) << std::endl;
	//std::cout << "expected:                 " << (sizeof(float) * 3) << std::endl;
	//std::cout << "segment:                  " << sizeof(utils::math::geometry::shape::segment) << std::endl;
	//std::cout << "expected:                 " << (sizeof(float) * 4) << std::endl;
	//std::cout << "bezier:                   " << sizeof(utils::math::geometry::shape::bezier<4>) << std::endl;
	//std::cout << "expected:                 " << (sizeof(float) * 8) << std::endl;
	//
	//const utils::math::geometry::shape::point testpt{10.f, 0.f};
	//const utils::math::geometry::shape::segment testsg{{15.f, 0.f}, {20.f, 0.f}};
	//const utils::math::geometry::shape::polygon testpoly{{15.f, -5.f}, {20.f, 0.f}, {15.f, 5.f}};
	//const utils::math::geometry::shape::view::polygon<true> testpolyview{std::span(testpoly.vertices.begin(), testpoly.vertices.size())};
	//const utils::math::geometry::shape::bezier<4> testcurve{utils::math::vec2f{10.f, 10.f}, utils::math::vec2f{200.f, 10.f}, utils::math::vec2f{300.f, 100.f}, utils::math::vec2f{50.f, 150.f}};
	//
	//utils::math::geometry::shape::mixed testglyph;
	//testglyph.add_first_vertex({ 32.f,  32.f});
	//testglyph.add_segment     ({128.f,  32.f});
	//testglyph.add_segment     ({256.f, 128.f});
	//testglyph.add_segment     ({128.f, 224.f});
	//testglyph.add_segment     ({ 32.f, 224.f});
	//
	//utils::math::geometry::group<false> group;
	//group.add(testglyph);
	//group.add(testsg);
	//
	//group.for_each([](const auto& shape)
	//	{
	//	if constexpr (utils::math::geometry::shape::concepts::segment<std::remove_cvref_t<decltype(shape)>>)
	//		{
	//		std::cout << "segment\n\t" << shape.a << ", " << shape.b << std::endl;
	//		}
	//	else if constexpr (utils::math::geometry::shape::concepts::mixed<std::remove_cvref_t<decltype(shape)>>)
	//		{
	//		std::cout << "mixed\n\tvertices count: " << shape.vertices.size() << std::endl;
	//		}
	//	});
	//
	//static_assert(utils::math::geometry::shape::concepts::any<decltype(testpt)>);
	//static_assert(utils::math::geometry::shape::concepts::point<decltype(testpt)>);
	//
	//static_assert(utils::math::geometry::shape::concepts::any<decltype(testsg)>);
	//static_assert(utils::math::geometry::shape::concepts::ab<decltype(testsg)>);
	//static_assert(utils::math::geometry::shape::concepts::segment<decltype(testsg)>);
	//
	//static_assert(utils::math::geometry::shape::concepts::any<decltype(testpoly)>);
	//static_assert(utils::math::geometry::shape::concepts::any<decltype(testpolyview)>);
	//
	//static_assert(utils::math::geometry::shape::concepts::polyline<decltype(testpoly)>);
	//static_assert(utils::math::geometry::shape::concepts::polyline<decltype(testpolyview)>);
	//
	//static_assert(utils::math::geometry::shape::concepts::polygon<decltype(testpoly)>);
	//static_assert(utils::math::geometry::shape::concepts::polygon<decltype(testpolyview)>);
	//
	//static_assert(utils::math::geometry::shape::concepts::bezier<decltype(testcurve)>);
	//
	//std::cout << testpt.distance_signed(testpoly    ).value << std::endl;
	//std::cout << testpt.distance_signed(testpolyview).value << std::endl;
	//std::cout << testpt.distance_signed(testcurve   ).value << std::endl;
	//
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
