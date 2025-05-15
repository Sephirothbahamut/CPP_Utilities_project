#include "../geometry_sdf_and_normal_texture.h"

#include <vector>

#include <utils/index_range.h>
#include <utils/algorithm/for_each.h>

#include "../log.h"

void geometry_sdf_and_normal_texture::dynamic_for_each_shape() const noexcept
	{
	const auto _{logger.section("Dynamic for each shape")};
	logger.flush();

	std::vector<utils::math::geometry::shape::aabb> bounding_boxes;
	for (const auto& dynamic_shape : shapes)
		{
		bounding_boxes.emplace_back(dynamic_shape->bounding_box() + shape_padding);
		}

	utils::matrix<utils::math::geometry::sdf::direction_signed_distance> dsdf(image_sizes);

	utils::clock<std::chrono::high_resolution_clock, float> clock;

	utils::logging::progress_bar progress_bar{.01f, 50};
	utils::logging::partial_progress partial_progress{progress_bar.partial_progress(shapes.size())};

	const auto per_shape_function{[&](size_t shape_index)
		{
		const auto& bounding_box{bounding_boxes[shape_index]};
		const auto& shape       {shapes        [shape_index]};

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
			const auto evaluated{shape->sdf(coords_f).direction_signed_distance()};
			directional_distance.merge_self(evaluated);
			});
		}};
	const auto per_shape_function_abs{[&](size_t shape_index)
		{
		const auto& bounding_box{bounding_boxes[shape_index]};
		const auto& shape       {shapes        [shape_index]};

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
			const auto evaluated{shape->sdf(coords_f).direction_signed_distance()};
			directional_distance.merge_self_absolute(evaluated);
			});
		}};

	const auto shapes_indices{utils::indices(shapes)};
	std::for_each(shapes_indices.begin(), shapes_indices.end() - 1, per_shape_function);
	per_shape_function_abs(shapes.size() - 1);
	
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

	utils::graphics::image::save_to_file(image_dsdf, "./output/dynamic_for_each_shape_dsdf.png");
	utils::graphics::image::save_to_file(image_lit , "./output/dynamic_for_each_shape_lit.png" );
	}
