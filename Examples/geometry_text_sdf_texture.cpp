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

namespace
	{
	float smoothstep(float edge0, float edge1, float x)
		{
		// Scale, bias and saturate x to 0..1 range
		x = std::clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
		// Evaluate polynomial
		return x * x * (3.f - 2.f * x);
		}
	}

void geometry_text_sdf_texture()
	{
	//std::string string{"Laelina"};
	std::string string{"L"};
	std::vector<utils::graphics::text::glyph_t> glyphs{utils::graphics::text::glyphs_from_string(string)};

	const utils::math::transform2 transform
		{
		.translation{64.f, 256.f + 32.f},
		.scaling{8.f}
		};

	for (auto& glyph : glyphs)
		{
		utils::math::geometry::interactions::transform_self(glyph, transform);
		}

	const utils::math::vec3f light_source{100.f, 50.f, 10.f};

	//utils::math::vec2s image_sizes{size_t{1024 + 512}, size_t{512}};
	utils::math::vec2s image_sizes{size_t{512}, size_t{512}};

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

		if (coords_indices == utils::math::vec2s{size_t{283}, size_t{580}})
			{
			std::cout << "a";
			}

		
		utils::math::geometry::interactions::return_types::gradient_signed_distance gdist;
		for (const auto& glyph : glyphs)
			{
			const auto tmp{utils::math::geometry::interactions::gradient_signed_distance(glyph, coords_f)};
		
			gdist = utils::math::geometry::interactions::return_types::gradient_signed_distance::merge_absolute(gdist, tmp);
			}

		// Inigo Quilez fancy colors
		//gdist.distance.value *= .006f;
		//utils::math::vec3f colour = (gdist.distance.side().is_outside()) ? utils::math::vec3f{.9f, .6f, .3f} : utils::math::vec3f{.4f, .7f, .85f};
		//colour = utils::math::vec3f{gdist.gradient.x() * .5f + .5f, gdist.gradient.y() * .5f + .5f, 1.f};
		//colour *= 1.0f - 0.5f * std::exp(-16.0f * gdist.distance.absolute());
		//colour *= 0.9f + 0.1f * std::cos(150.0f * gdist.distance.value);
		//colour = utils::math::lerp(colour, utils::math::vec3f{1.f}, 1.f - smoothstep(0.f, .01f, gdist.distance.absolute()));
		//
		//if (gdist.distance.side().is_inside())
		//	{
		//	colour *= .5f;
		//	}

		utils::math::vec3f colour;

		if (gdist.distance.side().is_outside())
			{
			const float background_intensity{.5f};
			const float background_shadow_mask{1.f - utils::math::vec2f::dot(gdist.gradient, light_source.xy())};
			const float background{background_intensity * background_shadow_mask};

			colour[0] = background;
			colour[1] = background;
			colour[2] = background;
			}
		else
			{
			const float edge_angled_area_thickness{4.f};
			const float z{std::clamp(gdist.distance.absolute() / edge_angled_area_thickness, 0.f, 1.f)};
			const float remainder{1.f - z};
			utils::math::vec3f normal{gdist.gradient.x() * remainder, gdist.gradient.y() * remainder, z};

			const float lightmap{utils::math::vec3f::dot(normal, light_source)};

			colour[0] = 0.f;
			colour[1] = lightmap;
			colour[2] = lightmap;
			}
		
		const utils::graphics::colour::rgba_u colour_8
			{
			static_cast<uint8_t>(colour[0] * 255.f),
			static_cast<uint8_t>(colour[1] * 255.f),
			static_cast<uint8_t>(colour[2] * 255.f),
			uint8_t{255}
			};

		image[image_sizes.coords_to_index(coords_indices)] = colour_8;
		});
	
	stbi_write_png("text_output.png", static_cast<int>(image_sizes.x()), static_cast<int>(image_sizes.y()), 4, image.data(), static_cast<int>(image_sizes.x() * 4));
	}
