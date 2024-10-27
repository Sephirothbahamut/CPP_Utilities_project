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

#include <utils/graphics/text.h>

#include "gsdf_helpers.h"

void geometry_text_sdf_texture()
	{
	utils::math::geometry::shape::aabb shape_padding{-32.f, -32.f, 32.f, 32.f};

	//std::string string{(const char*)u8"c"};
	std::string string{(const char*)u8"Freya"};
	//std::string string{(const char*)u8"\n"};

	std::vector<utils::graphics::text::glyph_t> glyphs{utils::graphics::text::glyphs_from_string(string, L"Arial")};
	//std::vector<utils::graphics::text::glyph_t> glyphs{utils::graphics::text::glyphs_from_string(string, L"Gabriola")};
	//std::vector<utils::graphics::text::glyph_t> glyphs{utils::graphics::text::glyphs_from_string(string, L"Mana")};
	
	std::vector<utils::graphics::sdf::shape_bounding_box_wrapper<utils::graphics::text::glyph_t>> bb_glyphs;
	for (const auto& glyph : glyphs)
		{
		bb_glyphs.emplace_back(glyph, shape_padding);
		}
	
	utils::math::geometry::shape::bezier<4> curve
		{
		utils::math::vec2f{2.5f, 107.5625f},
		utils::math::vec2f{2.5f, 103.979172f},
		utils::math::vec2f{3.09375f, 100.84375f},
		utils::math::vec2f{4.28125f, 98.15625f},
		};









	const utils::math::vec2s image_sizes{size_t{512}, size_t{256}};
	const float supersampling{1.f};
	
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

	/*
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
			//.transform(camera_transform)
			.scale    (1.f / supersampling)
			};


		utils::math::geometry::sdf::gradient_signed_distance gdist;
		for (const auto& bb_glyph : bb_glyphs)
			{
			const auto evaluated{bb_glyph.evaluate_gradient_signed_distance(coords_f)};
			gdist = utils::math::geometry::sdf::gradient_signed_distance::merge_absolute(gdist, evaluated);
			}
		
		//utils::math::geometry::sdf::gradient_signed_distance gdist{curve.sdf(coords_f).gradient_signed_distance()};
			
		const auto sample_gdist{gsdf_helpers::gradient_sdf_from_gdist(          gdist            )};
		const auto sample_lit  {gsdf_helpers::apply_light            (coords_f, gdist, light, 8.f)};

		image_gsdf[index] = sample_gdist;
		image_lit [index] = sample_lit;
		});
	
	const auto elapsed{clock.get_elapsed()};
	std::cout << "text: " << elapsed.count() << std::endl;

	utils::graphics::image::save_to_file(image_gsdf    , "./output/text.png");
	utils::graphics::image::save_to_file(image_lit     , "./output/text_lit.png" );


	}
