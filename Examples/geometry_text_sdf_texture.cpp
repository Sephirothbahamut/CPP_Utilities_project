#include <vector>
#include <ranges>
#include <execution>

#include <utils/math/vec2.h>
#include <utils/math/vec3.h>
#include <utils/math/vec_s.h>
#include <utils/graphics/text.h>
#include <utils/math/geometry/all.h>
#include <utils/third_party/stb_image.h>
#include <utils/third_party/stb_image_write.h>

#include <utils/clock.h>

#include "gsdf_helpers.h"

void geometry_text_sdf_texture()
	{
	// Testing outside of COM to make sure I'm not going crazy and my classes are fine
	for (size_t i = 0; i < 200; i++)
		{
		std::vector<utils::graphics::text::glyph_t> vec;

		for (size_t i = 0; i < 200; i++)
			{
			utils::graphics::text::glyph_t glyph{utils::math::vec2f{10.f, 10.f}};
			glyph.add_segments({utils::math::vec2f{20.f, 20.f}, utils::math::vec2f{30.f, 30.f}, utils::math::vec2f{40.f, 40.f}});
			glyph.add_bezier_3pt({utils::math::vec2f{50.f, 50.f}, utils::math::vec2f{60.f, 60.f}, utils::math::vec2f{70.f, 70.f}, utils::math::vec2f{80.f, 80.f}});

			const utils::math::transform2 transform
				{
				.translation{1.f, 1.f}
				};
			utils::math::geometry::interactions::transform_self(glyph, transform);

			if (glyph.vertices[0].x() != 11.f) { throw std::runtime_error{"Corrupted memory"}; }

			vec.emplace_back(std::move(glyph));
			}
		}

	//std::string string{(const char*)u8"Laelina\n\"£€$%&/()=?!^"};
	std::string string{"Iqwerty"};
	std::vector<utils::graphics::text::glyph_t> glyphs{utils::graphics::text::glyphs_from_string(string, L"Arial")};
	std::vector<utils::math::geometry::shape::aabb> aabbs(glyphs.size());

	const utils::math::transform2 transform
		{
		.translation{64.f, 64.f}
		};

	const float maximum_sdf_distance{32.f};

	std::ranges::iota_view glyphs_indices(size_t{0}, glyphs.size());
	std::for_each(std::execution::par, glyphs_indices.begin(), glyphs_indices.end(), [&](size_t index)
		{
		auto& glyph{glyphs[index]};
		auto& aabb {aabbs [index]};

		utils::math::geometry::interactions::transform_self(glyph, transform);
		aabb = utils::math::geometry::interactions::bounding_box(glyph);
		aabb.size().resize
			(
			utils::alignment{.horizontal_alignment{utils::alignment::horizontal::centre}, .vertical_alignment{utils::alignment::vertical::middle}}, 
			aabb.size() + utils::math::vec2f{maximum_sdf_distance * 2.f, maximum_sdf_distance * 2.f}
			);
		});


	utils::math::vec2s image_sizes{size_t{/*1024 +*/ 512}, size_t{512}};

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

		if (coords_indices == utils::math::vec2s{size_t{75}, size_t{134}})
			{
			std::cout << "a";
			}

		
		utils::math::geometry::interactions::return_types::gradient_signed_distance gdist;
		
		for (size_t glyphs_index{0}; glyphs_index < glyphs.size(); glyphs_index++)
			{
			const auto& aabb{aabbs[glyphs_index]};
			if (!aabb.contains(coords_f)) { continue; }

			const auto& glyph{glyphs[glyphs_index]};

			const auto tmp{utils::math::geometry::interactions::gradient_signed_distance(glyph, coords_f)};
			gdist = utils::math::geometry::interactions::return_types::gradient_signed_distance::merge_absolute(gdist, tmp);
			}

		const auto colour_f{gsdf_helpers::gradient_sdf_from_gdist(gdist)};
		const auto colour_u{gsdf_helpers::rgba_f_to_u(colour_f)};


		image[image_sizes.coords_to_index(coords_indices)] = colour_u;
		});
	
	stbi_write_png("text_output.png", static_cast<int>(image_sizes.x()), static_cast<int>(image_sizes.y()), 4, image.data(), static_cast<int>(image_sizes.x() * 4));
	}
