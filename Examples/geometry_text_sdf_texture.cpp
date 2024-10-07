#include <vector>
#include <ranges>
#include <execution>

#include <utils/clock.h>
#include <utils/math/vec2.h>
#include <utils/math/vec3.h>
#include <utils/math/vec_s.h>
#include <utils/graphics/text.h>
#include <utils/math/geometry/all.h>
#include <utils/third_party/stb_image.h>
#include <utils/graphics/multisampling.h>
#include <utils/third_party/stb_image_write.h>

#include "gsdf_helpers.h"

void geometry_text_sdf_texture()
	{
	//std::string string{(const char*)u8"Freya"};
	std::string string{(const char*)u8"\n"};

	//std::vector<utils::graphics::text::glyph_t> glyphs{utils::graphics::text::glyphs_from_string(string, L"Gabriola")};
	std::vector<utils::graphics::text::glyph_t> glyphs{utils::graphics::text::glyphs_from_string(string, L"Mana")};

	std::vector<utils::math::geometry::shape::aabb> aabbs(glyphs.size());

	const utils::math::transform2 transform
		{
		.translation{0.f, 64.f},
		//.scaling{1.f}
		};

	const float maximum_sdf_distance{32.f};

	std::ranges::iota_view glyphs_indices(size_t{0}, glyphs.size());
	std::for_each(std::execution::par, glyphs_indices.begin(), glyphs_indices.end(), [&](size_t index)
		{
		auto& glyph{glyphs[index]};
		auto& aabb {aabbs [index]};

		glyph.transform_self(transform);
		aabb = glyph.bounding_box();
		aabb.proxy_size().resize
			(
			utils::alignment{.horizontal_alignment{utils::alignment::horizontal::centre}, .vertical_alignment{utils::alignment::vertical::middle}},
			aabb.size() + utils::math::vec2f{maximum_sdf_distance * 2.f, maximum_sdf_distance * 2.f}
			);
		});

	auto bounding_box{utils::math::geometry::shape::aabb::create::inverse_infinite()};
	for(const auto& aabb : aabbs)
		{
		bounding_box.merge_self(aabb);
		}

	std::for_each(std::execution::par, glyphs_indices.begin(), glyphs_indices.end(), [&](size_t index)
		{
		auto& glyph{glyphs[index]};
		auto& aabb {aabbs [index]};
		glyph.translate_self(-bounding_box.up_left());
		aabb .translate_self(-bounding_box.up_left());
		});

	const utils::math::vec2s image_sizes
		{
		static_cast<size_t>(bounding_box.width ()),
		static_cast<size_t>(bounding_box.height()),
		};


	utils::storage::multiple<utils::graphics::colour::rgba_u> image_lit {image_sizes.sizes_to_size()};
	utils::storage::multiple<utils::graphics::colour::rgba_u> image_gsdf{image_sizes.sizes_to_size()};
	std::ranges::iota_view indices(size_t{0}, image_lit.size());

	gsdf_helpers::simple_pointlight light
		{
		.position{200.f, 200.f, 200.f},
		.colour{0.f, .5f, 1.f},
		.intensity{2.f}
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
	
		//if (coords_indices != utils::math::vec2s{size_t{173}, size_t{93}})
		//	{
		//	return;
		//	}
	
		const auto sample{utils::graphics::multisample<gsdf_helpers::sample_t, 4>(coords_f, [&](utils::math::vec2f coords_f)
			{
			utils::math::geometry::sdf::gradient_signed_distance gdist;
		
			for (size_t glyphs_index{0}; glyphs_index < glyphs.size(); glyphs_index++)
				{
				const auto& aabb{aabbs[glyphs_index]};
				if (!aabb.contains(coords_f)) { continue; }
		
				const auto& glyph{glyphs[glyphs_index]};
			
				const auto tmp{glyph.sdf(coords_f).gradient_signed_distance()};
				gdist = utils::math::geometry::sdf::gradient_signed_distance::merge_absolute(gdist, tmp);
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
			const auto colour_u{gsdf_helpers::rgba_f_to_u(sample.lit)};
			image_lit[image_sizes.coords_to_index(coords_indices)] = colour_u;
			}
		if (true)
			{
			const auto colour_u{gsdf_helpers::rgba_f_to_u(sample.gdist)};
			image_gsdf[image_sizes.coords_to_index(coords_indices)] = colour_u;
			}
		});

	const auto elapsed{clock.get_elapsed()};
	std::cout << elapsed.count() << std::endl;

	stbi_write_png("text_output_lit.png" , static_cast<int>(image_sizes.x()), static_cast<int>(image_sizes.y()), 4, image_lit .data(), static_cast<int>(image_sizes.x() * 4));
	stbi_write_png("text_output_gsdf.png", static_cast<int>(image_sizes.x()), static_cast<int>(image_sizes.y()), 4, image_gsdf.data(), static_cast<int>(image_sizes.x() * 4));
	}
