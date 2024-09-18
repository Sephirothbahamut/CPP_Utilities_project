
#include <ranges>
#include <execution>

#include <utils/clock.h>
#include <utils/math/vec2.h>
#include <utils/math/vec3.h>
#include <utils/math/vec_s.h>
#include <utils/math/geometry/all.h>
#include <utils/third_party/stb_image.h>
#include <utils/graphics/multisampling.h>
#include <utils/third_party/stb_image_write.h>


#include <utils/math/geometry/sdf/ab.h>

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
	mixed.add_bezier
		({
		utils::math::vec2f{470.f, 820.f} + (utils::math::vec2f{470.f, 820.f} - utils::math::vec2f{380.f, 670.f}),
		utils::math::vec2f{295.f, 750.f},
		utils::math::vec2f{190.f, 790.f}
		});

	auto mixed2{mixed};

	if (true)
		{
		auto mixed3{mixed };
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

	static constexpr auto bezier_ends
		{
		utils::math::geometry::ends::optional_ab::create::value
			(
			utils::math::geometry::ends::ab::create::default_(false, true)
			)
		};
	utils::math::geometry::shape::bezier<3, bezier_ends> bezier_3_pt{.vertices{utils::math::vec2f{840.f, 80.f}, utils::math::vec2f{440.f, 200.f}, utils::math::vec2f{420.f, 50.f}}};
	utils::math::geometry::shape::bezier<4, bezier_ends> bezier_4_pt{.vertices
		{
		utils::math::vec2f{760.f, 670.f},
		utils::math::vec2f{860.f, 450.f},
		utils::math::vec2f{470.f,  90.f},
		utils::math::vec2f{800.f, 200.f}
		}};
	utils::math::geometry::shape::bezier<4, bezier_ends> bezier_loop{.vertices
		{
		utils::math::vec2f{760.f, 670.f},
		utils::math::vec2f{860.f, 450.f},
		utils::math::vec2f{470.f,  90.f},
		utils::math::vec2f{800.f, 200.f}
		}};

	utils::math::geometry::interactions::translate_self(poyline   , {50.f, 50.f});
	utils::math::geometry::interactions::scale_self    (triangle  , .5f);
	utils::math::geometry::interactions::translate_self(triangle  , {122.f, 143.f});
	utils::math::geometry::interactions::translate_self(triangle_b, {222.f, 143.f});
	
	utils::math::vec2s image_sizes{size_t{900}, size_t{900}};

	utils::storage::multiple<utils::graphics::colour::rgba_u> image_lit {image_sizes.sizes_to_size()};
	utils::storage::multiple<utils::graphics::colour::rgba_u> image_gsdf{image_sizes.sizes_to_size()};
	std::ranges::iota_view indices(size_t{0}, image_lit.size());

	gsdf_helpers::simple_pointlight light
		{
		.position{200.f, 100.f, 100.f},
		.colour{0.f, .5f, 1.f},
		.intensity{2.f}
		};

	std::vector<utils::math::geometry::shape::segment> segments
		{
			{utils::math::vec2f{20.f, 18.f}, utils::math::vec2f{30.f, 18.f}},
			{utils::math::vec2f{32.f, 20.f}, utils::math::vec2f{32.f, 30.f}},
			{utils::math::vec2f{30.f, 32.f}, utils::math::vec2f{20.f, 32.f}},
			{utils::math::vec2f{18.f, 30.f}, utils::math::vec2f{18.f, 20.f}}
		};

	utils::clock<std::chrono::high_resolution_clock, float> clock;

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

		
		const auto sample{utils::graphics::multisample<gsdf_helpers::sample_t, 4>(coords_f, [&](utils::math::vec2f coords_f)
			{
			utils::math::geometry::interactions::return_types::gradient_signed_distance gdist;
			
			const auto cwsd_mixed_outer  {utils::math::geometry::interactions::closest_with_signed_distance(mixed        , coords_f)};
			const auto cwsd_mixed_inverse{utils::math::geometry::interactions::closest_with_signed_distance(mixed_inverse, coords_f)};
			const auto cwsd_mixed{utils::math::geometry::interactions::return_types::closest_point_with_signed_distance::pick_closest(cwsd_mixed_outer, cwsd_mixed_inverse)};

			std::array gdists
				{
				utils::math::geometry::interactions::gradient_signed_distance(poyline    , coords_f),
				utils::math::geometry::interactions::gradient_signed_distance(triangle   , coords_f),
				utils::math::geometry::interactions::gradient_signed_distance(triangle_b , coords_f),
				utils::math::geometry::interactions::gradient_signed_distance(bezier_3_pt, coords_f),
				utils::math::geometry::interactions::gradient_signed_distance(bezier_4_pt, coords_f),
				utils::math::geometry::interactions::gradient_signed_distance(bezier_loop, coords_f),
				utils::math::geometry::interactions::return_types::gradient_signed_distance::create(cwsd_mixed, coords_f),
				//segments[0].sdf(coords_f).gradient_signed_distance(),
				segments[1].sdf(coords_f).gradient_signed_distance(),
				segments[2].sdf(coords_f).gradient_signed_distance(),
				//segments[3].sdf(coords_f).gradient_signed_distance()
				};

			for (const auto& tmp : gdists)
				{
				gdist = utils::math::geometry::interactions::return_types::gradient_signed_distance::merge(gdist, tmp);
				}
			
			
			const auto sample_gdist{gsdf_helpers::gradient_sdf_from_gdist(          gdist            )};
			const auto sample_lit  {gsdf_helpers::apply_light            (coords_f, gdist, light, 8.f)};
			
			return gsdf_helpers::sample_t
				{
				.gdist{sample_gdist}, 
				.lit  {sample_lit  }
				};
			})};

		if (true)
			{
			//const auto colour_f{gsdf_helpers::apply_light(coords_f, gdist, light, 8.f)};
			const auto colour_u{gsdf_helpers::rgba_f_to_u(sample.lit)};
			image_lit[image_sizes.coords_to_index(coords_indices)] = colour_u;
			}
		if (true)
			{
			//const auto colour_f{gsdf_helpers::gradient_sdf_from_gdist(gdist)};
			const auto colour_u{gsdf_helpers::rgba_f_to_u(sample.gdist)};
			image_gsdf[image_sizes.coords_to_index(coords_indices)] = colour_u;
			}
		});
	
	const auto elapsed{clock.get_elapsed()};
	std::cout << elapsed.count() << std::endl;

	stbi_write_png("geometry_output_lit.png" , static_cast<int>(image_sizes.x()), static_cast<int>(image_sizes.y()), 4, image_lit .data(), static_cast<int>(image_sizes.x() * 4));
	stbi_write_png("geometry_output_gsdf.png", static_cast<int>(image_sizes.x()), static_cast<int>(image_sizes.y()), 4, image_gsdf.data(), static_cast<int>(image_sizes.x() * 4));
	}
