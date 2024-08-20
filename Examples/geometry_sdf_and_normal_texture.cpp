
#include <ranges>
#include <execution>

#include <utils/math/vec2.h>
#include <utils/math/vec3.h>
#include <utils/math/vec_s.h>
#include <utils/math/geometry/all.h>
#include <utils/third_party/stb_image.h>
#include <utils/third_party/stb_image_write.h>

#include <utils/clock.h>

#include "gsdf_helpers.h"

void geometry_sdf_and_normal_texture()
	{
	// Closed polygon with variable vertices count
	utils::math::geometry::shape::polygon<std::dynamic_extent> triangle
		{
		.vertices
			{
			utils::math::vec2f{  0.f,   0.f},
			utils::math::vec2f{100.f,   0.f}
			}
		};
	triangle.vertices.storage.emplace_back(50.f, 50.f);

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
	
	utils::math::geometry::shape::mixed<utils::math::geometry::ends::closeable::create::closed()> mixed{utils::math::vec2f{130.f, 630.f}};
	mixed.add_segments
		({
		utils::math::vec2f{180.f, 580.f},
		utils::math::vec2f{500.f, 600.f}
		});
	mixed.add_bezier
		({
		utils::math::vec2f{380.f, 670.f},
		utils::math::vec2f{470.f, 820.f}
		});
	mixed.add_segment({190.f, 790.f});

	auto mixed2{mixed};

	if (true)
		{
		auto mixed3{mixed};
		auto mixed4{mixed3};
		mixed2 = mixed3;
		}

	utils::math::geometry::shape::mixed mixed_inverse{utils::math::vec2f{150.f, 635.f}};
	mixed_inverse.add_segments
		({
		utils::math::vec2f{166.f, 684.f},
		utils::math::vec2f{197.f, 700.f}
		});
	mixed_inverse.add_bezier
		({
		utils::math::vec2f{160.f, 640.f},
		utils::math::vec2f{340.f, 730.f}
		});
	mixed_inverse.add_segment({183.f, 592.f});

	// Open polyline defined on a statically sized span of vertices over a sequence in memory, with an infinite start and a finite end
	utils::math::geometry::shape::observer::polyline<utils::math::geometry::ends::closeable::create::open(true, false), 5> poyline{.vertices{vertices.begin(), size_t{5}}};

	// Closed polygon defined on a statically sized span of vertices over a sequence in memory
	utils::math::geometry::shape::observer::polygon<> triangle_b{.vertices{vertices.begin() + 5, size_t{3}}};

	using bezier_t = utils::math::geometry::shape::bezier
		<
		3,
		utils::math::geometry::ends::optional_ab::create::value
			(
			utils::math::geometry::ends::ab::create::default_(false, true)
			)
		>;
	bezier_t bezier{.vertices{utils::math::vec2f{800.f, 400.f}, utils::math::vec2f{200.f, 600.f}, utils::math::vec2f{300.f, 300.f}}};

	utils::math::geometry::interactions::translate_self(poyline   , {50.f, 50.f});
	utils::math::geometry::interactions::scale_self    (triangle  , .5f);
	utils::math::geometry::interactions::translate_self(triangle  , {122.f, 143.f});
	utils::math::geometry::interactions::translate_self(triangle_b, {222.f, 143.f});
	
	const utils::math::vec3f light_source{100.f, 50.f, 10.f};

	utils::math::vec2s image_sizes{size_t{1024}, size_t{1024}};
	utils::storage::multiple<utils::graphics::colour::rgba_u> image{image_sizes.sizes_to_size()};
	std::ranges::iota_view indices(size_t{0}, image.size());

	//*
	std::for_each(std::execution::par, indices.begin(), indices.end(), [&](size_t index)
	/*/
	std::for_each(indices.begin(), indices.end(), [&](size_t index)
	/**/
		{
		const utils::math::vec2s coords_indices{image_sizes.index_to_coords(index)};
		const utils::math::vec2f coords_f
			{
			static_cast<float>(coords_indices.x()),
			static_cast<float>(coords_indices.y())
			};

		if (coords_indices == utils::math::vec2s{size_t{283}, size_t{580}})
			{
			std::cout << "a";
			}

		const auto cwsd_mixed_outer  {utils::math::geometry::interactions::closest_with_signed_distance(mixed        , coords_f)};
		const auto cwsd_mixed_inverse{utils::math::geometry::interactions::closest_with_signed_distance(mixed_inverse, coords_f)};
		const auto cwsd_mixed{utils::math::geometry::interactions::return_types::closest_point_with_signed_distance::pick_closest(cwsd_mixed_outer, cwsd_mixed_inverse)};

		std::array gdists
			{
			utils::math::geometry::interactions::gradient_signed_distance(poyline   , coords_f),
			utils::math::geometry::interactions::gradient_signed_distance(triangle  , coords_f),
			utils::math::geometry::interactions::gradient_signed_distance(triangle_b, coords_f),
			utils::math::geometry::interactions::gradient_signed_distance(bezier    , coords_f),
			utils::math::geometry::interactions::return_types::gradient_signed_distance::create(cwsd_mixed, coords_f),
			//utils::math::geometry::interactions::return_types::gradient_signed_distance
			//	{
			//	.distance{cwsd_mixed_inverse.distance},
			//	.gradient{0.f, 1.f}
			//	},
			};

		utils::math::geometry::interactions::return_types::gradient_signed_distance gdist;
		for (const auto& tmp : gdists)
			{
			gdist = utils::math::geometry::interactions::return_types::gradient_signed_distance::merge(gdist, tmp);
			}
		

		gdist.distance.value *= .006f;

		const auto colour_f{gsdf_helpers::gradient_sdf_from_gdist(gdist)};

		image[image_sizes.coords_to_index(coords_indices)] = colour_f;
		});
	
	stbi_write_png("geometry_output.png", static_cast<int>(image_sizes.x()), static_cast<int>(image_sizes.y()), 4, image.data(), static_cast<int>(image_sizes.x() * 4));
	}
