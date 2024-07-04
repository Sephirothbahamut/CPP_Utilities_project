
#include <ranges>
#include <execution>

#include <utils/math/vec3.h>
#include <utils/math/vec_s.h>
#include <utils/math/geometry/all.h>
#include <utils/third_party/stb_image.h>
#include <utils/third_party/stb_image_write.h>

float smoothstep(float edge0, float edge1, float x) 
	{
	// Scale, bias and saturate x to 0..1 range
	x = std::clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
	// Evaluate polynomial
	return x * x * (3.f - 2.f * x);
	}

void geometry_sdf_and_normal_texture()
	{
	// Closed polygon with variable vertices count
	utils::math::geometry::shape::polygon<std::dynamic_extent> triangle
		{
		utils::math::vec2f{  0.f,   0.f},
		utils::math::vec2f{100.f,   0.f}
		};
	triangle.storage.emplace_back(50.f, 50.f);

	// Plain vertices sequence in memory
	std::vector<utils::math::vec2f> vertices
		{
		utils::math::vec2f{  0.f,   0.f},
		utils::math::vec2f{100.f,   0.f},
		utils::math::vec2f{120.f,  50.f},
		utils::math::vec2f{100.f, 100.f},
		utils::math::vec2f{  0.f, 100.f},
		utils::math::vec2f{  0.f,   0.f},
		utils::math::vec2f{100.f,   0.f},
		utils::math::vec2f{ 50.f,  50.f}
		};

	// Open polyline defined on a statically sized span of vertices over a sequence in memory, with an infinite start and a finite end
	utils::math::geometry::shape::observer::polyline<utils::math::geometry::ends::create::open(true, false), 5> poyline{vertices.begin(), size_t{5}};

	// Closed polygon defined on a statically sized span of vertices over a sequence in memory
	utils::math::geometry::shape::observer::polygon<> triangle_b{vertices.begin() + 5, size_t{3}};

	utils::math::geometry::shape::bezier<3> bezier{utils::math::vec2f{300.f, 300.f}, utils::math::vec2f{200.f, 600.f}, utils::math::vec2f{800.f, 400.f}};

	utils::math::geometry::interactions::translate_self(poyline   , {50.f, 50.f});
	utils::math::geometry::interactions::scale_self    (triangle  , .5f);
	utils::math::geometry::interactions::translate_self(triangle  , {122.f, 143.f});
	utils::math::geometry::interactions::translate_self(triangle_b, {222.f, 143.f});
	
	const utils::math::vec3f light_source{100.f, 50.f, 10.f};

	utils::math::vec2s image_sizes{size_t{1024}, size_t{1024}};
	utils::storage::multiple<utils::graphics::colour::rgba_u> image{image_sizes.sizes_to_size()};
	std::ranges::iota_view indices(size_t{0}, image.size());

	std::for_each(std::execution::par, indices.begin(), indices.end(), [&](size_t index)
		{
		const utils::math::vec2s coords_indices{image_sizes.index_to_coords(index)};
		const utils::math::vec2f coords_f
			{
			static_cast<float>(coords_indices.x()),
			static_cast<float>(coords_indices.y())
			};

		const auto gdist_a{utils::math::geometry::interactions::gradient_signed_distance(poyline   , coords_f)};
		const auto gdist_b{utils::math::geometry::interactions::gradient_signed_distance(triangle  , coords_f)};
		const auto gdist_c{utils::math::geometry::interactions::gradient_signed_distance(triangle_b, coords_f)};
		const auto gdist_d{utils::math::geometry::interactions::gradient_signed_distance(bezier    , coords_f)};

		auto gdist
			{
			utils::math::geometry::interactions::return_types::gradient_signed_distance::merge
				(
				utils::math::geometry::interactions::return_types::gradient_signed_distance::merge
					(
					utils::math::geometry::interactions::return_types::gradient_signed_distance::merge(gdist_a, gdist_b),
					gdist_c
					),
				gdist_d
				)
			};

		gdist.distance.value *= .006f;

		utils::math::vec3f col = (gdist.distance.side().is_outside()) ? utils::math::vec3f{.9f, .6f, .3f} : utils::math::vec3f{.4f, .7f, .85f};
		
		col = utils::math::vec3f{gdist.gradient.x() * .5f + .5f, gdist.gradient.y() * .5f + .5f, 1.f};
		col *= 1.0f - 0.5f * std::exp(-16.0f * gdist.distance.absolute());
		col *= 0.9f + 0.1f * std::cos(150.0f * gdist.distance.value);
		col = utils::math::lerp(col, utils::math::vec3f{1.f}, 1.f - smoothstep(0.f, .01f, gdist.distance.absolute()));

		if (gdist.distance.side().is_inside())
			{
			col *= .5f;
			}
		
		const utils::graphics::colour::rgba_u colour_8
			{
			static_cast<uint8_t>(col[0] * 255.f),
			static_cast<uint8_t>(col[1] * 255.f),
			static_cast<uint8_t>(col[2] * 255.f),
			uint8_t{255}
			};

		image[image_sizes.coords_to_index(coords_indices)] = colour_8;
		});

	stbi_write_png("output.png", static_cast<int>(image_sizes.x()), static_cast<int>(image_sizes.y()), 4, image.data(), static_cast<int>(image_sizes.x() * 4));
	}
