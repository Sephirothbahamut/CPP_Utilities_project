#include "../geometry_sdf_and_normal_texture.h"

#include <utils/algorithm/for_each.h>
#include <vector>

#include "../log.h"

void geometry_sdf_and_normal_texture::dynamic_for_each_pixel() const noexcept
	{
	const auto _{logger.section("Dynamic for each pixel")};
	logger.flush();

	std::vector<utils::math::geometry::shape::aabb> bounding_boxes;
	for (const auto& dynamic_shape : shapes)
		{
		bounding_boxes.emplace_back(dynamic_shape->bounding_box() + shape_padding);
		}


	utils::matrix<utils::graphics::colour::rgba_f> image_lit (image_sizes);
	utils::matrix<utils::graphics::colour::rgba_f> image_dsdf(image_sizes);

	utils::clock<std::chrono::high_resolution_clock, float> clock;

	const auto for_each{utils::algorithm::for_each::in_sizes(image_sizes).scale(supersampling)};
	for_each.execute<true>([&](const decltype(for_each)::callback_params& params)
		{
		const utils::math::vec2f coords_f{params.global.floating};

		utils::math::geometry::sdf::direction_signed_distance directional_distance;

		for (size_t i = 0; i < 2; i++)
			{
			if (bounding_boxes[i].contains(coords_f))
				{
				const auto evaluated{shapes[i]->sdf(coords_f).direction_signed_distance()};
				directional_distance.merge_self_absolute(evaluated);
				}
			}
		for (size_t i = 2; i < shapes.size(); i++)
			{
			if (bounding_boxes[i].contains(coords_f))
				{
				const auto evaluated{shapes[i]->sdf(coords_f).direction_signed_distance()};
				directional_distance.merge_self(evaluated);
				}
			}
		
		const auto sample_gdist{utils::graphics::sdf::debug_sample_direction_sdf(directional_distance)};
		const auto sample_lit  {dsdf_helpers::apply_light(coords_f, directional_distance, light, 8.f)};

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

	utils::graphics::image::save_to_file(image_dsdf, "./output/dynamic_for_each_pixel_dsdf.png");
	utils::graphics::image::save_to_file(image_lit , "./output/dynamic_for_each_pixel_lit.png" );
	}
