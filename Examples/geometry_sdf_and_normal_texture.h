#pragma once

#include <ranges>
#include <iostream>
#include <algorithm>
#include <execution>

#include <utils/clock.h>
#include <utils/math/vec.h>
#include <utils/math/geometry/shapes.h>
#include <utils/third_party/stb_image.h>
#include <utils/graphics/multisampling.h>
#include <utils/third_party/stb_image_write.h>

#include <utils/graphics/sdf.h>
#include <utils/graphics/image.h>

#include <utils/math/transform2.h>
#include <utils/math/geometry/sdf/all.h>
#include <utils/math/geometry/shape/all.h>
#include <utils/math/geometry/bounds/all.h>
#include <utils/math/geometry/transform/all.h>

#include "dsdf_helpers.h"

struct geometry_sdf_and_normal_texture
	{
	#pragma region shapes
	//The world prefix is to distinguish the objects in the world from the point and aabbs used inside the image generation algorithm
	utils::math::geometry::shape::point   world_point{40.f, 840.f};
	utils::math::geometry::shape::aabb    world_aabb{utils::math::geometry::shape::aabb::create::from_ul_dr(utils::math::vec2f{60.f, 400.f}, utils::math::vec2f{256.f, 475.f})};
	utils::math::geometry::shape::circle  circle{utils::math::vec2f{500.f, 400.f}, 64.f};
	utils::math::geometry::shape::capsule capsule{utils::math::geometry::shape::owner::segment{utils::math::vec2f{300.f, 330.f}, utils::math::vec2f{400.f, 330.f}}, 64.f};

	// Closed polygon with variable vertices count
	// More vertices added elsewhere (see constructor)
	utils::math::geometry::shape::polygon<std::dynamic_extent> triangle
		{
		utils::math::vec2f{  0.f,   0.f},
		utils::math::vec2f{100.f,   0.f}
		};
	
	// Plain vertices sequence in memory
	std::vector<utils::math::vec2f> vertices
		{
		utils::math::vec2f{  0.f,   0.f},
		utils::math::vec2f{100.f,   0.f},
		utils::math::vec2f{ 80.f,  50.f},
		utils::math::vec2f{100.f, 100.f},
		utils::math::vec2f{  0.f, 100.f},
		utils::math::vec2f{  0.f,   0.f},
		utils::math::vec2f{100.f,   0.f},
		utils::math::vec2f{ 50.f,  50.f}
		};

	// Mixed shapes can be initialized with their starting vertex
	// More vertices added elsewhere (see constructor)
	utils::math::geometry::shape::mixed<utils::math::geometry::ends::closeable::create::closed()> mixed        {utils::math::vec2f{130.f, 630.f}};
	utils::math::geometry::shape::mixed<utils::math::geometry::ends::closeable::create::closed()> mixed_inverse{utils::math::vec2f{150.f, 635.f}};
	utils::math::geometry::shape::mixed<utils::math::geometry::ends::closeable::create::open  ()> mixed_cut_self_intersections{utils::math::vec2f{630.f, 740.f}};

	// Open polyline defined on a statically sized span of vertices over a sequence in memory, with an infinite start and a finite end
	utils::math::geometry::shape::observer::polyline<utils::math::geometry::ends::closeable::create::open(true, false), 5> polyline{vertices.begin(), size_t{5}};

	// Closed polygon defined on a statically sized span of vertices over a sequence in memory
	utils::math::geometry::shape::observer::polygon<> triangle_b{vertices.begin() + 5, size_t{3}};

	inline static constexpr utils::math::geometry::ends::optional_ab bezier_ends
		{
		utils::math::geometry::ends::optional_ab::create::value
			(
			utils::math::geometry::ends::ab::create::default_(false, true)
			)
		};
	utils::math::geometry::shape::bezier<3, bezier_ends> bezier_3_pt
		{
		utils::math::vec2f{840.f,  80.f},
		utils::math::vec2f{440.f, 200.f},
		utils::math::vec2f{420.f,  50.f}
		};
	utils::math::geometry::shape::bezier<4, bezier_ends> bezier_4_pt
		{
		utils::math::vec2f{760.f, 670.f},
		utils::math::vec2f{860.f, 450.f},
		utils::math::vec2f{470.f,  90.f},
		utils::math::vec2f{800.f, 200.f}
		};
	utils::math::geometry::shape::bezier<4, bezier_ends> bezier_loop
		{
		utils::math::vec2f{760.f, 670.f},
		utils::math::vec2f{860.f, 450.f},
		utils::math::vec2f{470.f,  90.f},
		utils::math::vec2f{800.f, 200.f}
		};

	std::vector<utils::math::geometry::shape::segment> segments
		{
			{utils::math::vec2f{20.f, 18.f}, utils::math::vec2f{30.f, 18.f}},
			{utils::math::vec2f{32.f, 20.f}, utils::math::vec2f{32.f, 30.f}},
			{utils::math::vec2f{30.f, 32.f}, utils::math::vec2f{20.f, 32.f}},
			{utils::math::vec2f{18.f, 30.f}, utils::math::vec2f{18.f, 20.f}}
		};
	utils::math::geometry::shape::bezier<4> quadratic_as_cubic
		{
		utils::math::vec2f{2.5f     + 195.f, 107.5625f  },
		utils::math::vec2f{2.5f     + 195.f, 103.979172f},
		utils::math::vec2f{3.09375f + 195.f, 100.84375f },
		utils::math::vec2f{4.28125f + 195.f, 98.15625f  },
		};
	#pragma endregion shapes

	utils::math::geometry::shape::aabb shape_padding{-32.f, -32.f, 32.f, 32.f};
	const utils::math::vec2s output_resolution{size_t{900}, size_t{900}};
	utils::math::vec2f supersampling{2.f, 2.f};
	const utils::math::vec2s image_sizes{output_resolution * static_cast<utils::math::vec2s>(supersampling)};

	dsdf_helpers::simple_pointlight light
		{
		.position {200.f, 100.0f, 100.f},
		.colour   {  0.f,    .5f,   1.f},
		.intensity{2.f}
		};

	std::vector<utils::polymorphic_value<utils::math::geometry::shape::dynamic::base>> shapes;

	geometry_sdf_and_normal_texture() noexcept;

	void dynamic_for_each_pixel() const noexcept;
	void dynamic_for_each_shape() const noexcept;
	void static_for_each_pixel () const noexcept;
	void static_for_each_shape () const noexcept;
	};
