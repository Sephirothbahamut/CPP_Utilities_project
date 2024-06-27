#include <string>
#include <iostream>
#include <execution>

#include "include/utils/memory.h"

#include "include/utils/console/io.h"
#include "include/utils/console/colour.h"
#include "include/utils/console/initializer.h"

#include "include/utils/math/vec.h"
#include "include/utils/math/vec2.h"
#include "include/utils/math/vec3.h"
#include "include/utils/math/vec_s.h"
#include "include/utils/math/angle.h"
#include "include/utils/math/rect.h"

#include "include/utils/output/std_containers.h"
#include "include/utils/graphics/colour.h"

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
#include "include/utils/matrix_interface.h"
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

#include "include/utils/third_party/stb_image.h"
#include "include/utils/third_party/stb_image_write.h"


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
float smoothstep(float edge0, float edge1, float x) {
	// Scale, bias and saturate x to 0..1 range
	x = std::clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
	// Evaluate polynomial
	return x * x * (3 - 2 * x);
	}

utils::math::vec3f sdgCircle(utils::math::vec2f p, float r)
	{
	const float l = p.get_length();
	const auto div{p / l};
	return utils::math::vec3f(l - r, div.x(), div.y());
	}

int main()
	{


	utils::console::initializer utils_console_initializer;
	using namespace utils::output;
	using namespace utils::math::angle::literals;

	std::vector<utils::math::vec2f> vertices
		{
		utils::math::vec2f{  0.f,   0.f},
		utils::math::vec2f{100.f,   0.f},
		utils::math::vec2f{100.f, 100.f},
		utils::math::vec2f{  0.f, 100.f},
		utils::math::vec2f{  0.f,   0.f},
		utils::math::vec2f{100.f,   0.f},
		utils::math::vec2f{ 50.f,  50.f}
		};

	utils::math::vec2f asdqwe{0.f, 0.f};
	asdqwe = -asdqwe;

	using span_t = utils::math::geometry::shape::observer::polygon<3>::storage_t::inner_storage_t;
	
	utils::math::geometry::shape::polygon<std::dynamic_extent> polyline{utils::math::vec2f{0.f, 0.f}, utils::math::vec2f{100.f, 0.f}, utils::math::vec2f{50.f, 50.f}};
	utils::math::geometry::shape::observer::polygon<4> triangle_a{vertices.begin(), size_t{4}};
	utils::math::geometry::shape::observer::polygon<3> triangle_b{vertices.begin() + 4, size_t{3}};

	utils::math::geometry::interactions::translate_self(polyline, {50.f, 50.f});
	
	utils::math::geometry::interactions::translate_self(triangle_a, {50.f, 50.f});
	utils::math::geometry::interactions::scale_self    (triangle_b, .5f);
	utils::math::geometry::interactions::translate_self(triangle_b, {120.f, 145.f});

	const utils::math::vec3f light_source{100.f, 50.f, 10.f};

	utils::math::vec2s image_sizes{size_t{1024}, size_t{1024}};
	utils::storage::multiple<utils::graphics::colour::rgba_u> image{image_sizes.sizes_to_size()};
	std::ranges::iota_view indices(size_t{0}, image.size());

	std::for_each(std::execution::par, indices.begin(), indices.end(), [&](size_t index)
		{
		const utils::math::vec2s coords_indices{image_sizes.index_to_coords(index)};
		const utils::math::vec2f coords_f
			{
			static_cast<float>(coords_indices.x()),
			static_cast<float>(coords_indices.y())
			};
		
		const auto shape{polyline};
		const auto sdist_a{utils::math::geometry::interactions::signed_distance(triangle_a, coords_f)};
		const auto sdist_b{utils::math::geometry::interactions::signed_distance(triangle_b, coords_f)};

		const auto gdist_a{utils::math::geometry::interactions::gradient_signed_distance(triangle_a, coords_f)};
		const auto gdist_b{utils::math::geometry::interactions::gradient_signed_distance(triangle_b, coords_f)};
		auto gdist  {utils::math::geometry::interactions::return_types::gradient_signed_distance::merge(gdist_a, gdist_b)};
		//auto gdist{utils::math::geometry::interactions::gradient_signed_distance(triangle_a.get_edges()[0], coords_f)};

		//const auto tmp{sdgCircle(coords_f - utils::math::vec2f{256.f, 256.f}, 32.f)};
		//utils::math::geometry::interactions::return_types::gradient_signed_distance gdist
		//	{
		//	.distance{tmp.x()},
		//	.gradient
		//		{
		//		tmp.y(),
		//		tmp.z()
		//		}
		//	};
		//utils::graphics::colour::rgb_f colour{1.f};
		//colour -= (utils::graphics::colour::rgb_f{.1f, .4f, .7f} * gdist.distance.side().sign());
		//
		//colour *= 1.0 - std::exp(-4.0 * (gdist.distance.absolute() * .001f));
		//colour *= 0.8 + 0.2 * std::cos(280.0 * gdist.distance.value * .001f);
		//utils::math::vec3f normal{tmp.x(), normal_dir.y(), missing_to_max};
		//normal.normalize();

		gdist.distance.value *= .006f;

		utils::math::vec3f col = (gdist.distance.side().is_outside()) ? utils::math::vec3f{.9f, .6f, .3f} : utils::math::vec3f{.4f, .7f, .85f};
		
		col = utils::math::vec3f{gdist.gradient.x() * .5f + .5f, gdist.gradient.y() * .5f + .5f, 1.f};
		col *= 1.0 - 0.5 * std::exp(-16.0 * gdist.distance.absolute());
		col *= 0.9 + 0.1 * std::cos(150.0 * gdist.distance.value);
		col = utils::math::lerp(col, utils::math::vec3f{1.f}, 1.f - smoothstep(0.f, .01f, gdist.distance.absolute()));

		if (gdist.distance.side().is_inside())
			{
			col *= .5f;
			}

			
		// Sdist colour

		//utils::graphics::colour::rgb_f colour
		//	{
		//	std::fmod(gdist.distance.value, 64.f),
		//	std::fmod(gdist.gradient.x()  , 64.f),
		//	std::fmod(gdist.gradient.y()  , 64.f)
		//	};
		//colour /= 64.f;
		
		const utils::graphics::colour::rgba_u colour_8
			{
			static_cast<uint8_t>(col[0] * 255.f),
			static_cast<uint8_t>(col[1] * 255.f),
			static_cast<uint8_t>(col[2] * 255.f),
			uint8_t{255}
			};

		image[image_sizes.coords_to_index(coords_indices)] = colour_8;
		});

	stbi_write_png("output.png", static_cast<int>(image_sizes.x()), static_cast<int>(image_sizes.y()), 4, image.data(), static_cast<int>(image_sizes.x() * 4));



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
