//#include <ranges>
//#include <algorithm>
//#include <execution>
//
//#include <utils/clock.h>
//#include <utils/math/vec.h>
//#include <utils/math/geometry/shapes.h>
//#include <utils/third_party/stb_image.h>
//#include <utils/graphics/multisampling.h>
//#include <utils/third_party/stb_image_write.h>
//
//#include <utils/graphics/sdf.h>
//#include <utils/graphics/image.h>
//
//#include <utils/math/transform2.h>
//#include <utils/math/geometry/shape/sdf/all.h>
//#include <utils/math/geometry/shape/bounds/all.h>
//#include <utils/math/geometry/shape/transform/all.h>
//
//#include "dsdf_helpers.h"
//
//void geometry_sdf_and_normal_texture()
//	{
//	#pragma region shapes
//	//The world prefix is to distinguish the objects in the world from the point and aabbs used inside the image generation algorithm
//	utils::math::geometry::shape::point world_point{40.f, 840.f};
//	auto world_aabb{utils::math::geometry::shape::aabb::create::from_ul_dr(utils::math::vec2f{60.f, 400.f}, utils::math::vec2f{256.f, 475.f})};
//	utils::math::geometry::shape::circle circle{utils::math::vec2f{500.f, 400.f}, 64.f};
//
//	// Closed polygon with variable vertices count
//	utils::math::geometry::shape::polygon<std::dynamic_extent> triangle
//		{
//		utils::math::vec2f{  0.f,   0.f},
//		utils::math::vec2f{100.f,   0.f}
//		};
//	triangle.vertices.storage.emplace_back(50.f, 50.f);
//
//	// Plain vertices sequence in memory
//	std::vector<utils::math::vec2f> vertices
//		{
//		utils::math::vec2f{  0.f,   0.f},
//		utils::math::vec2f{100.f,   0.f},
//		utils::math::vec2f{ 80.f,  50.f},
//		utils::math::vec2f{100.f, 100.f},
//		utils::math::vec2f{  0.f, 100.f},
//		utils::math::vec2f{  0.f,   0.f},
//		utils::math::vec2f{100.f,   0.f},
//		utils::math::vec2f{ 50.f,  50.f}
//		};
//	
//	utils::math::geometry::shape::mixed<utils::math::geometry::ends::closeable::create::closed()> mixed{utils::math::vec2f{130.f, 630.f}};
//	mixed.add_segments
//		({
//		utils::math::vec2f{180.f, 580.f},
//		utils::math::vec2f{500.f, 600.f}
//		});
//	mixed.add_bezier
//		({
//		utils::math::vec2f{380.f, 670.f},
//		utils::math::vec2f{470.f, 820.f}
//		});
//	mixed.add_bezier
//		({
//		utils::math::vec2f{470.f, 820.f} + (utils::math::vec2f{470.f, 820.f} - utils::math::vec2f{380.f, 670.f}),
//		utils::math::vec2f{295.f, 750.f},
//		utils::math::vec2f{190.f, 790.f}
//		});
//
//	utils::math::geometry::shape::mixed mixed_inverse{utils::math::vec2f{150.f, 635.f}};
//	mixed_inverse.add_segments
//		({
//		utils::math::vec2f{166.f, 684.f},
//		utils::math::vec2f{197.f, 700.f}
//		});
//	mixed_inverse.add_bezier
//		({
//		utils::math::vec2f{160.f, 640.f},
//		utils::math::vec2f{340.f, 730.f}
//		});
//	mixed_inverse.add_segment({183.f, 592.f});
//
//	// Open polyline defined on a statically sized span of vertices over a sequence in memory, with an infinite start and a finite end
//	utils::math::geometry::shape::observer::polyline<utils::math::geometry::ends::closeable::create::open(true, false), 5> polyline{vertices.begin(), size_t{5}};
//
//	// Closed polygon defined on a statically sized span of vertices over a sequence in memory
//	utils::math::geometry::shape::observer::polygon<> triangle_b{vertices.begin() + 5, size_t{3}};
//
//	static constexpr auto bezier_ends
//		{
//		utils::math::geometry::ends::optional_ab::create::value
//			(
//			utils::math::geometry::ends::ab::create::default_(false, true)
//			)
//		};
//	utils::math::geometry::shape::bezier<3, bezier_ends> bezier_3_pt
//		{
//		utils::math::vec2f{840.f,  80.f},
//		utils::math::vec2f{440.f, 200.f},
//		utils::math::vec2f{420.f,  50.f}
//		};
//	utils::math::geometry::shape::bezier<4, bezier_ends> bezier_4_pt
//		{
//		utils::math::vec2f{760.f, 670.f},
//		utils::math::vec2f{860.f, 450.f},
//		utils::math::vec2f{470.f,  90.f},
//		utils::math::vec2f{800.f, 200.f}
//		};
//	utils::math::geometry::shape::bezier<4, bezier_ends> bezier_loop
//		{
//		utils::math::vec2f{760.f, 670.f},
//		utils::math::vec2f{860.f, 450.f},
//		utils::math::vec2f{470.f,  90.f},
//		utils::math::vec2f{800.f, 200.f}
//		};
//
//	polyline  .translate_self({50.f, 50.f});
//	triangle  .scale_self    (.5f);
//	triangle  .translate_self({122.f, 143.f});
//	triangle_b.translate_self({222.f, 143.f});
//	
//	std::vector<utils::math::geometry::shape::segment> segments
//		{
//			{utils::math::vec2f{20.f, 18.f}, utils::math::vec2f{30.f, 18.f}},
//			{utils::math::vec2f{32.f, 20.f}, utils::math::vec2f{32.f, 30.f}},
//			{utils::math::vec2f{30.f, 32.f}, utils::math::vec2f{20.f, 32.f}},
//			{utils::math::vec2f{18.f, 30.f}, utils::math::vec2f{18.f, 20.f}}
//		};
//	#pragma endregion shapes
//
//	const utils::math::vec2s output_resolution{size_t{900}, size_t{900}};
//	float supersampling{2.f};
//	utils::math::transform2 camera_transform;
//	const utils::math::vec2s image_sizes{output_resolution * static_cast<size_t>(supersampling)};
//
//	dsdf_helpers::simple_pointlight light
//		{
//		.position {200.f, 100.0f, 100.f},
//		.colour   {  0.f,    .5f,   1.f},
//		.intensity{2.f}
//		};
//
//
//	utils::graphics::sdf::debug debug_renderer;
//
//	utils::matrix<utils::math::geometry::sdf::direction_signed_distance> direction_signed_distance_field(image_sizes);
//
//	utils::math::geometry::shape::aabb shape_padding{-32.f, -32.f, 32.f, 32.f};
//	//utils::math::geometry::shape::aabb shape_padding{utils::math::geometry::shape::aabb::create::infinite()};
//	utils::graphics::sdf::shape_bounding_box_wrapper bb_wrapper_mixed        {mixed        , shape_padding};
//	utils::graphics::sdf::shape_bounding_box_wrapper bb_wrapper_mixed_inverse{mixed_inverse, shape_padding};
//	utils::graphics::sdf::shape_bounding_box_wrapper bb_wrapper_world_point  {world_point  , shape_padding};
//	utils::graphics::sdf::shape_bounding_box_wrapper bb_wrapper_world_aabb   {world_aabb   , shape_padding};
//	utils::graphics::sdf::shape_bounding_box_wrapper bb_wrapper_circle       {circle       , shape_padding};
//	utils::graphics::sdf::shape_bounding_box_wrapper bb_wrapper_polyline     {polyline     , shape_padding};
//	utils::graphics::sdf::shape_bounding_box_wrapper bb_wrapper_triangle     {triangle     , shape_padding};
//	utils::graphics::sdf::shape_bounding_box_wrapper bb_wrapper_triangle_b   {triangle_b   , shape_padding};
//	utils::graphics::sdf::shape_bounding_box_wrapper bb_wrapper_bezier_3_pt  {bezier_3_pt  , shape_padding};
//	utils::graphics::sdf::shape_bounding_box_wrapper bb_wrapper_bezier_4_pt  {bezier_4_pt  , shape_padding};
//	utils::graphics::sdf::shape_bounding_box_wrapper bb_wrapper_bezier_loop  {bezier_loop  , shape_padding};
//	utils::graphics::sdf::shape_bounding_box_wrapper bb_wrapper_segments_1   {segments[1]  , shape_padding};
//	utils::graphics::sdf::shape_bounding_box_wrapper bb_wrapper_segments_2   {segments[2]  , shape_padding};
//
//
//	utils::clock<std::chrono::high_resolution_clock, float> clock;
//	
//	bb_wrapper_mixed        .evaluate_dsdf(camera_transform, &utils::math::geometry::sdf::direction_signed_distance::merge_absolute, direction_signed_distance_field, supersampling);
//	bb_wrapper_mixed_inverse.evaluate_dsdf(camera_transform, &utils::math::geometry::sdf::direction_signed_distance::merge_absolute, direction_signed_distance_field, supersampling);
//	bb_wrapper_world_point  .evaluate_dsdf(camera_transform, &utils::math::geometry::sdf::direction_signed_distance::merge         , direction_signed_distance_field, supersampling);
//	bb_wrapper_world_aabb   .evaluate_dsdf(camera_transform, &utils::math::geometry::sdf::direction_signed_distance::merge         , direction_signed_distance_field, supersampling);
//	bb_wrapper_circle       .evaluate_dsdf(camera_transform, &utils::math::geometry::sdf::direction_signed_distance::merge         , direction_signed_distance_field, supersampling);
//	bb_wrapper_polyline     .evaluate_dsdf(camera_transform, &utils::math::geometry::sdf::direction_signed_distance::merge         , direction_signed_distance_field, supersampling);
//	bb_wrapper_triangle     .evaluate_dsdf(camera_transform, &utils::math::geometry::sdf::direction_signed_distance::merge         , direction_signed_distance_field, supersampling);
//	bb_wrapper_triangle_b   .evaluate_dsdf(camera_transform, &utils::math::geometry::sdf::direction_signed_distance::merge         , direction_signed_distance_field, supersampling);
//	bb_wrapper_bezier_3_pt  .evaluate_dsdf(camera_transform, &utils::math::geometry::sdf::direction_signed_distance::merge         , direction_signed_distance_field, supersampling);
//	bb_wrapper_bezier_4_pt  .evaluate_dsdf(camera_transform, &utils::math::geometry::sdf::direction_signed_distance::merge         , direction_signed_distance_field, supersampling);
//	bb_wrapper_bezier_loop  .evaluate_dsdf(camera_transform, &utils::math::geometry::sdf::direction_signed_distance::merge         , direction_signed_distance_field, supersampling);
//	bb_wrapper_segments_1   .evaluate_dsdf(camera_transform, &utils::math::geometry::sdf::direction_signed_distance::merge         , direction_signed_distance_field, supersampling);
//	bb_wrapper_segments_2   .evaluate_dsdf(camera_transform, &utils::math::geometry::sdf::direction_signed_distance::merge         , direction_signed_distance_field, supersampling);
//	
//	const auto rendered_image{debug_renderer.render({}, direction_signed_distance_field, supersampling)};
//	
//	auto elapsed{clock.get_elapsed()};
//	std::cout << elapsed.count() << std::endl;
//	
//	utils::graphics::image::save_to_file(rendered_image, "geometry_output_test.png");
//	
//	clock.restart();
//
//	const auto rendered_image_2{debug_renderer.render<true>(camera_transform, image_sizes, [&](const utils::math::vec2f coords_f)
//		{
//		if (coords_f.x() == 50.f && coords_f.y() == 1650.f)
//			{
//			std::cout << "a";
//			}
//		
//		const auto evaluated_mixed        {bb_wrapper_mixed        .evaluate_direction_signed_distance(coords_f)};
//		const auto evaluated_mixed_inverse{bb_wrapper_mixed_inverse.evaluate_direction_signed_distance(coords_f)};
//		const auto evaluated_world_point  {bb_wrapper_world_point  .evaluate_direction_signed_distance(coords_f)};
//		const auto evaluated_world_aabb   {bb_wrapper_world_aabb   .evaluate_direction_signed_distance(coords_f)};
//		const auto evaluated_circle       {bb_wrapper_circle       .evaluate_direction_signed_distance(coords_f)};
//		const auto evaluated_polyline     {bb_wrapper_polyline     .evaluate_direction_signed_distance(coords_f)};
//		const auto evaluated_triangle     {bb_wrapper_triangle     .evaluate_direction_signed_distance(coords_f)};
//		const auto evaluated_triangle_b   {bb_wrapper_triangle_b   .evaluate_direction_signed_distance(coords_f)};
//		const auto evaluated_bezier_3_pt  {bb_wrapper_bezier_3_pt  .evaluate_direction_signed_distance(coords_f)};
//		const auto evaluated_bezier_4_pt  {bb_wrapper_bezier_4_pt  .evaluate_direction_signed_distance(coords_f)};
//		const auto evaluated_bezier_loop  {bb_wrapper_bezier_loop  .evaluate_direction_signed_distance(coords_f)};
//		const auto evaluated_segments_1   {bb_wrapper_segments_1   .evaluate_direction_signed_distance(coords_f)};
//		const auto evaluated_segments_2   {bb_wrapper_segments_2   .evaluate_direction_signed_distance(coords_f)};
//
//		utils::math::geometry::sdf::direction_signed_distance gdist;
//		gdist = utils::math::geometry::sdf::direction_signed_distance::merge_absolute(gdist, evaluated_mixed        );
//		gdist = utils::math::geometry::sdf::direction_signed_distance::merge_absolute(gdist, evaluated_mixed_inverse);
//		gdist = utils::math::geometry::sdf::direction_signed_distance::merge         (gdist, evaluated_world_point  );
//		gdist = utils::math::geometry::sdf::direction_signed_distance::merge         (gdist, evaluated_world_aabb   );
//		gdist = utils::math::geometry::sdf::direction_signed_distance::merge         (gdist, evaluated_circle       );
//		gdist = utils::math::geometry::sdf::direction_signed_distance::merge         (gdist, evaluated_polyline     );
//		gdist = utils::math::geometry::sdf::direction_signed_distance::merge         (gdist, evaluated_triangle     );
//		gdist = utils::math::geometry::sdf::direction_signed_distance::merge         (gdist, evaluated_triangle_b   );
//		gdist = utils::math::geometry::sdf::direction_signed_distance::merge         (gdist, evaluated_bezier_3_pt  );
//		gdist = utils::math::geometry::sdf::direction_signed_distance::merge         (gdist, evaluated_bezier_4_pt  );
//		gdist = utils::math::geometry::sdf::direction_signed_distance::merge         (gdist, evaluated_bezier_loop  );
//		gdist = utils::math::geometry::sdf::direction_signed_distance::merge         (gdist, evaluated_segments_1   );
//		gdist = utils::math::geometry::sdf::direction_signed_distance::merge         (gdist, evaluated_segments_2   );
//
//		return gdist;
//		}, supersampling)};
//
//	elapsed = clock.get_elapsed();
//	std::cout << elapsed.count() << std::endl;
//
//	utils::graphics::image::save_to_file(rendered_image_2, "geometry_output_test_2.png");
//
//	clock.restart();
//
//	utils::matrix<utils::graphics::colour::rgba_f> image_lit (image_sizes);
//	utils::matrix<utils::graphics::colour::rgba_f> image_dsdf(image_sizes);
//	std::ranges::iota_view indices(size_t{0}, image_lit.size());
//
//	//*
//	std::for_each(std::execution::par, indices.begin(), indices.end(), [&](size_t index)
//	/*/
//	std::for_each(indices.begin(), indices.end(), [&](size_t index)
//	/**/
//		{
//		const utils::math::vec2s coords_indices{image_sizes.index_to_coords(index)};
//		const utils::math::vec2f coords_f
//			{
//			utils::math::vec2f
//				{
//				static_cast<float>(coords_indices.x()),
//				static_cast<float>(coords_indices.y())
//				}
//			.transform(camera_transform)
//			.scale    (1.f / supersampling)
//			};
//
//
//		const auto sample{utils::graphics::multisample<dsdf_helpers::sample_t, 1>(coords_f, [&](utils::math::vec2f coords_f)
//			{
//			utils::math::geometry::sdf::direction_signed_distance gdist;
//			
//			const auto cwsd_mixed_outer  {mixed        .sdf(coords_f).closest_with_signed_distance()};
//			const auto cwsd_mixed_inverse{mixed_inverse.sdf(coords_f).closest_with_signed_distance()};
//			const auto cwsd_mixed{utils::math::geometry::sdf::closest_point_with_signed_distance::pick_closest(cwsd_mixed_outer, cwsd_mixed_inverse)};
//			
//			std::array gdists
//				{
//				world_point.sdf(coords_f).direction_signed_distance(),
//				world_aabb .sdf(coords_f).direction_signed_distance(),
//				circle     .sdf(coords_f).direction_signed_distance(),
//				polyline   .sdf(coords_f).direction_signed_distance(),
//				triangle   .sdf(coords_f).direction_signed_distance(),
//				triangle_b .sdf(coords_f).direction_signed_distance(),
//				bezier_3_pt.sdf(coords_f).direction_signed_distance(),
//				bezier_4_pt.sdf(coords_f).direction_signed_distance(),
//				bezier_loop.sdf(coords_f).direction_signed_distance(),
//				utils::math::geometry::sdf::direction_signed_distance::create(cwsd_mixed, coords_f),
//				segments[1].sdf(coords_f).direction_signed_distance(),
//				segments[2].sdf(coords_f).direction_signed_distance()
//				};
//
//			for (const auto& tmp : gdists)
//				{
//				gdist = utils::math::geometry::sdf::direction_signed_distance::merge(gdist, tmp);
//				}
//			
//			
//			const auto sample_gdist{dsdf_helpers::direction_sdf_from_gdist(          gdist            )};
//			const auto sample_lit  {dsdf_helpers::apply_light            (coords_f, gdist, light, 8.f)};
//			
//			return dsdf_helpers::sample_t
//				{
//				.gdist{sample_gdist}, 
//				.lit  {sample_lit  }
//				};
//			})};
//
//		if (true)
//			{
//			image_lit[index] = sample.lit;
//			}
//		if (true)
//			{
//			image_dsdf[index] = sample.gdist;
//			}
//		});
//	
//	elapsed = clock.get_elapsed();
//	std::cout << elapsed.count() << std::endl;
//
//	utils::graphics::image::save_to_file(image_lit     , "geometry_output_lit.png" );
//	utils::graphics::image::save_to_file(image_dsdf    , "geometry_output_dsdf.png");
//	}
