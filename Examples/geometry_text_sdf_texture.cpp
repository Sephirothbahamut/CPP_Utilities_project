#include <vector>
#include <ranges>
#include <iostream>
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
#include <utils/math/geometry/bounds/all.h>
#include <utils/math/geometry/transform/all.h>

#include <utils/MS/graphics/dx.h>
#include <utils/MS/graphics/conversions.h>
#include <utils/MS/graphics/text/format.h>
#include <utils/MS/graphics/text/renderer.h>

#include <utils/logging/logger.h>
#include <utils/logging/progress_bar.h>

#include "dsdf_helpers.h"

void geometry_text_sdf_texture()
	{
	const utils::math::vec2f output_size_mm{300.f, 300.f};
	const utils::math::vec2f dpi{100.f, 100.f};

	const utils::math::vec2f output_resolution_f{utils::math::ceil(utils::MS::graphics::conversions::mm_to_px(output_size_mm, dpi))};

	const utils::math::vec2s output_resolution{static_cast<size_t>(output_resolution_f.x()), static_cast<size_t>(output_resolution_f.y())};
	float supersampling{1.f};
	utils::math::transform2 camera_transform{};
	const utils::math::vec2s supersampled_resolution{output_resolution * static_cast<size_t>(supersampling)};



	utils::MS::graphics::dx::initializer dx_initializer;
	utils::MS::graphics::dx::context     dx_context{dx_initializer};



	utils::MS::graphics::text::format text_format
		{
		.font{"Gabriola"},
		.size{utils::MS::graphics::conversions::mm_to_dips(80.f)},
		.alignment{.horizontal_alignment{utils::alignment::horizontal::centre}}
		};

	//std::string string{(const char*)u8"c"};
	//std::string string{(const char*)u8"Freya"};
	//std::string string{(const char*)u8"\n"};
	std::string string{(const char*)u8"Hello, world!\nFreya\n\n"};
	//std::string string{(const char*)u8"de"}; //"d" in MagicMedieval has self-intersecting curves that my code doesn't handle
	//std::string string{(const char*)u8"Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum."};
	

	const auto dips_size{utils::MS::graphics::conversions::px_to_dips(output_resolution_f, dpi)};
	utils::MS::graphics::text::formatted_string formatted_string{string, text_format, dips_size};

	formatted_string.properties_regions.format.font.add("Mana", {10, 30});
	formatted_string.properties_regions.format.fill.colour.add({1.f, 0.f, 0.f, 1.f}, { 5, 5});
	formatted_string.properties_regions.format.fill.colour.add({1.f, 1.f, 0.f, 1.f}, {10, 5});
	formatted_string.properties_regions.format.fill.colour.add({0.f, 1.f, 0.f, 1.f}, {15, 5});
	formatted_string.properties_regions.format.fill.colour.add({0.f, 1.f, 1.f, 1.f}, {20, 5});
	formatted_string.properties_regions.format.fill.colour.add({0.f, 0.f, 1.f, 1.f}, {25, 5});
	formatted_string.properties_regions.format.fill.enabled.set_full_range(true);
	formatted_string.properties_regions.format.outline.enabled.add(true, {4, 2});
	formatted_string.properties_regions.format.outline.colour .add({1.f, 1.f, 1.f, 1.f}, {4, 2});

	//formatted_string.properties_regions.formatting.font.add("MagicMedieval", {0, 1});
	//formatted_string.properties_regions.rendering.text.colour.add({1.f, 1.f, 1.f, 1.f}, utils::containers::region::create::full_range());
	
	const auto renderable{formatted_string.shrink_to_fit(dx_initializer)};
	//const auto renderable{formatted_string.finalize(dx_initializer)};

	

	const utils::graphics::colour::rgba_f background_colour{0.f, .1f, .2f, 1.f};
	utils::MS::graphics::text::renderer text_renderer{dx_initializer, utils::MS::graphics::text::renderer::create_info
		{
		.resolution{output_resolution}, 
		.clear_colour{background_colour},
		.dpi{dpi}
		}};

	auto& default_rendering_properties{text_renderer.get_default_rendering_properties()};
	default_rendering_properties.render.outline.to_shapes = true;
	default_rendering_properties.render.outline.to_image  = true;
	default_rendering_properties.render.outline.to_shapes = true;
	text_renderer.draw_text(renderable, {0.f, 0.f});

	auto renderer_output_image{text_renderer.get_output_image()};
	auto renderer_output_shapes{text_renderer.get_output_shapes()};

	std::filesystem::create_directories("./output");
	utils::graphics::image::save_to_file(renderer_output_image, "output/text_directwrite.png");

	auto& glyphs{renderer_output_shapes.glyphs};

	utils::math::geometry::shape::aabb shape_padding{-32.f, -32.f, 32.f, 32.f};

	
	std::vector<utils::math::geometry::shape::aabb> bb_glyphs;
	for (auto& region_and_glyphs_group : glyphs)
		{
		auto& glyphs_group{region_and_glyphs_group.value};

		for (auto& glyph : glyphs_group)
			{
			auto aabb{utils::math::geometry::shape::aabb::create::inverse_infinite()};
			for (auto& outline : glyph)
				{
				outline.scale_self(utils::MS::graphics::conversions::multipliers::dips_to_px(dpi));

				const auto tmp{outline.bounding_box()};
				aabb.merge_self(tmp);
				}
			aabb = aabb + shape_padding;
			bb_glyphs.emplace_back(aabb);
			}
		}
	
	dsdf_helpers::simple_pointlight light
		{
		.position{200.f, 200.f, 200.f},
		.colour{0.f, .5f, 1.f},
		.intensity{2.f}
		};

	utils::matrix<utils::graphics::colour::rgba_f> image_lit (supersampled_resolution);
	utils::matrix<utils::graphics::colour::rgba_f> image_dsdf(supersampled_resolution);

	std::ranges::iota_view indices(size_t{0}, image_lit.size());

	utils::clock<std::chrono::high_resolution_clock, float> clock;

	utils::logging::progress_bar progress_bar{.01f, 50};
	std::atomic_size_t completed{0};
	const auto indices_count{image_lit.size()};

	//*
	std::for_each(std::execution::par, indices.begin(), indices.end(), [&](size_t index)
	/*/
	std::for_each(indices.begin(), indices.end(), [&](size_t index)
	/**/
		{
		const utils::math::vec2s coords_indices{supersampled_resolution.index_to_coords(index)};
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

		utils::math::geometry::sdf::direction_signed_distance gdist;



		size_t bb_index{0};
		for (auto& region_and_glyphs_group : glyphs)
			{
			auto& glyphs_group{region_and_glyphs_group.value};

			for (auto& glyph : glyphs_group)
				{
				const auto& aabb{bb_glyphs[bb_index]};
				if (aabb.contains(coords_f))
					{
					const auto evaluated{utils::math::geometry::sdf::composite{glyph, coords_f}.direction_signed_distance()};
					gdist = utils::math::geometry::sdf::direction_signed_distance::merge_absolute(gdist, evaluated);
					}
				bb_index++;
				}
			}
			
		const auto sample_gdist{utils::graphics::sdf::debug_sample_direction_sdf(gdist            )};
		const auto sample_lit  {dsdf_helpers::apply_light            (coords_f, gdist, light, 8.f)};
		
		image_dsdf[index] = sample_gdist;
		image_lit [index] = sample_lit;

		completed++;
		const float percent{completed / static_cast<float>(indices_count)};
		progress_bar.advance(percent);
		});
	progress_bar.complete();
	
	const auto elapsed{clock.get_elapsed()};
	std::cout << "text: " << elapsed.count() << std::endl;

	utils::graphics::image::save_to_file(image_dsdf, "./output/text.png"    );
	utils::graphics::image::save_to_file(image_lit , "./output/text_lit.png");
	}
