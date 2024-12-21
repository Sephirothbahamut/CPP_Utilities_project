#include <vector>
#include <ranges>
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
#include <utils/math/geometry/shape/sdf/all.h>
#include <utils/math/geometry/shape/bounds/all.h>
#include <utils/math/geometry/shape/transform/all.h>

#include <utils/MS/graphics/dx.h>
#include <utils/MS/graphics/text/format.h>
#include <utils/MS/graphics/text/renderer.h>

#include "gsdf_helpers.h"

void geometry_text_sdf_texture()
	{
	const utils::math::vec2s output_resolution{size_t{512}, size_t{512}};
	float supersampling{2.f};
	utils::math::transform2 camera_transform{};
	const utils::math::vec2s image_sizes{output_resolution * static_cast<size_t>(supersampling)};



	utils::MS::graphics::dx::initializer dx_initializer;
	utils::MS::graphics::dx::context     dx_context{dx_initializer};

	const utils::math::vec2f output_resolution_f{static_cast<float>(output_resolution.x()), static_cast<float>(output_resolution.y())};

	utils::MS::graphics::text::format text_format
		{
		.font{"Gabriola"},
		.size{64.f},
		.alignment{.horizontal{utils::alignment::horizontal::centre}}
		};

	const utils::graphics::colour::rgba_f background_colour{0.f, .1f, .2f, 1.f};
	utils::MS::graphics::text::renderer text_renderer{dx_initializer, output_resolution, background_colour};
	auto& default_rendering_properties{text_renderer.get_default_rendering_properties()};
	default_rendering_properties.outline   .to_shapes = true;
	default_rendering_properties.decorators.to_image  = false;
	default_rendering_properties.decorators.to_shapes = true;

	//std::string string{(const char*)u8"c"};
	//std::string string{(const char*)u8"Freya"};
	//std::string string{(const char*)u8"\n"};
	std::string string{(const char*)u8"Hello, world!\nFreya\n\n"};
	//std::string string{(const char*)u8"d"}; //"d" in MagicMedieval has self-intersecting curves that my code doesn't handle

	utils::MS::graphics::text::formatted_string formatted_string{dx_initializer, string, text_format, output_resolution_f};

	formatted_string.properties_regions.formatting.font.add("Mana", {10, 30});
	formatted_string.properties_regions.rendering.text.colour.add({1.f, 0.f, 0.f, 1.f}, { 5, 5});
	formatted_string.properties_regions.rendering.text.colour.add({1.f, 1.f, 0.f, 1.f}, {10, 5});
	formatted_string.properties_regions.rendering.text.colour.add({0.f, 1.f, 0.f, 1.f}, {15, 5});
	formatted_string.properties_regions.rendering.text.colour.add({0.f, 1.f, 1.f, 1.f}, {20, 5});
	formatted_string.properties_regions.rendering.text.colour.add({0.f, 0.f, 1.f, 1.f}, {25, 5});
	//formatted_string.properties_regions.formatting.font.add("MagicMedieval", {0, 1});
	//formatted_string.shrink_to_fit();
	formatted_string.update();

	text_renderer.draw_text(formatted_string, {0.f, 0.f});

	const auto renderer_output{text_renderer.get_output()};
	const auto renderer_dx_geometries{text_renderer.get_dx_geometry_output()};

	std::filesystem::create_directories("./output");
	utils::graphics::image::save_to_file(renderer_output.image, "output/text_directwrite.png");

	const auto& glyphs{renderer_output.outlines};

	utils::math::geometry::shape::aabb shape_padding{-32.f, -32.f, 32.f, 32.f};

	
	std::vector<utils::graphics::sdf::shape_bounding_box_wrapper<utils::MS::graphics::text::shape_outline>> bb_outlines;
	for (const auto& glyph : glyphs)
		{
		bb_outlines.emplace_back(glyph, shape_padding);
		}
	
	gsdf_helpers::simple_pointlight light
		{
		.position{200.f, 200.f, 200.f},
		.colour{0.f, .5f, 1.f},
		.intensity{2.f}
		};

	utils::matrix<utils::graphics::colour::rgba_f> image_lit (image_sizes);
	utils::matrix<utils::graphics::colour::rgba_f> image_gsdf(image_sizes);

	std::ranges::iota_view indices(size_t{0}, image_lit.size());

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
			utils::math::vec2f
				{
				static_cast<float>(coords_indices.x()),
				static_cast<float>(coords_indices.y())
				}
			.transform(camera_transform)
			.scale    (1.f / supersampling)
			};

		if (coords_indices == utils::math::vec2s{size_t{522}, size_t{51}})
			{
			std::cout << "break";
			}


		utils::math::geometry::sdf::gradient_signed_distance gdist;
		for (const auto& bb_outline : bb_outlines)
			{
			const auto evaluated{bb_outline.evaluate_gradient_signed_distance(coords_f)};
			gdist = utils::math::geometry::sdf::gradient_signed_distance::merge_absolute(gdist, evaluated);
			}

		gdist.distance.value = std::abs(gdist.distance.value) * (renderer_dx_geometries.is_inside(coords_f) ? -1.f : 1.f);
			
		const auto sample_gdist{utils::graphics::sdf::debug_sample_gradient_sdf(gdist            )};
		const auto sample_lit  {gsdf_helpers::apply_light            (coords_f, gdist, light, 8.f)};
		
		image_gsdf[index] = sample_gdist;
		image_lit [index] = sample_lit;
		});
	
	const auto elapsed{clock.get_elapsed()};
	std::cout << "text: " << elapsed.count() << std::endl;

	utils::graphics::image::save_to_file(image_gsdf, "./output/text.png"    );
	utils::graphics::image::save_to_file(image_lit , "./output/text_lit.png");
	}
