#include "../geometry_sdf_and_normal_texture.h"

#include <utils/algorithm/for_each.h>
#include <vector>

#include "../log.h"

void geometry_sdf_and_normal_texture::static_for_each_shape() const noexcept
	{
	const auto _{logger.section("Static for each shape")};
	logger.flush();

	utils::matrix<utils::math::geometry::sdf::direction_signed_distance> dsdf(image_sizes);

	utils::clock<std::chrono::high_resolution_clock, float> clock;

	utils::logging::progress_bar progress_bar{.01f, 50};
	utils::logging::partial_progress partial_progress{progress_bar.partial_progress(shapes.size())};

	const auto per_shape_function{[&](const auto& shape)
		{
		const auto& bounding_box{shape.bounding_box() + shape_padding};

		const auto for_each
			{
			utils::algorithm::for_each::in_sizes(image_sizes)
			.scale(supersampling)
			.scaled_region(bounding_box)
			.partial_progress(partial_progress)
			};
		for_each.execute<true>([&](const decltype(for_each)::callback_params& params)
			{
			const utils::math::vec2f coords_f{params.global.floating};

			auto& directional_distance{dsdf[params.index]};
			const auto evaluated{shape.sdf(coords_f).direction_signed_distance()};
			directional_distance.merge_self(evaluated);
			});
		}};
	const auto per_shape_function_abs{[&](const auto& shape)
		{
		const auto& bounding_box{shape.bounding_box() + shape_padding};

		const auto for_each
			{
			utils::algorithm::for_each::in_sizes(image_sizes)
			.scale(supersampling)
			.scaled_region(bounding_box)
			.partial_progress(partial_progress)
			};
		for_each.execute<true>([&](const decltype(for_each)::callback_params& params)
			{
			const utils::math::vec2f coords_f{params.global.floating};

			auto& directional_distance{dsdf[params.index]};
			const auto evaluated{shape.sdf(coords_f).direction_signed_distance()};
			directional_distance.merge_self_absolute(evaluated);
			});
		}};

	per_shape_function(mixed                       );
	per_shape_function(mixed_cut_self_intersections);
	per_shape_function(world_point                 );
	per_shape_function(world_aabb                  );
	per_shape_function(circle                      );
	per_shape_function(capsule                     );
	per_shape_function(polyline                    );
	per_shape_function(triangle                    );
	per_shape_function(triangle_b                  );
	per_shape_function(bezier_3_pt                 );
	per_shape_function(bezier_4_pt                 );
	per_shape_function(bezier_loop                 );
	per_shape_function(segments[1]                 );
	per_shape_function(segments[2]                 );
	per_shape_function(quadratic_as_cubic          );

	per_shape_function_abs(mixed_inverse);


	
	utils::matrix<utils::graphics::colour::rgba_f> image_lit (image_sizes);
	utils::matrix<utils::graphics::colour::rgba_f> image_dsdf(image_sizes);

	const auto for_each{utils::algorithm::for_each::in_sizes(image_lit.sizes()).scale(supersampling)};
	for_each.execute<true>([&](const decltype(for_each)::callback_params& params)
		{
		const auto directional_distance{dsdf[params.index]};
		const auto sample_gdist{utils::graphics::sdf::debug_sample_direction_sdf(directional_distance)};
		const auto sample_lit  {dsdf_helpers::apply_light(params.global.floating, directional_distance, light, 8.f)};

		if (true)
			{
			image_dsdf[params.index] = sample_gdist;
			}
		if (true)
			{
			image_lit[params.index] = sample_lit;
			}
		});
	
	const auto elapsed{clock.get_elapsed()};
	logger.log("Time: " + std::to_string(elapsed.count()));

	utils::graphics::image::save_to_file(image_dsdf, "./output/static_for_each_shape_dsdf.png");
	utils::graphics::image::save_to_file(image_lit , "./output/static_for_each_shape_lit.png" );
	}
