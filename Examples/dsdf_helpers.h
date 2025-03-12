#pragma once

#include <utils/math/math.h>
#include <utils/math/vec.h>
#include <utils/graphics/colour.h>
#include <utils/math/geometry/sdf/common.h>

namespace dsdf_helpers
	{
	struct dsdf_sample_t
		{
		utils::math::geometry::sdf::direction_signed_distance dsdf;
		
		dsdf_sample_t operator/(float count)
			{
			dsdf_sample_t ret{*this};
			ret.dsdf.distance.value /= count;
			ret.dsdf.direction       /= count;
			return ret;
			}
		dsdf_sample_t operator+(dsdf_sample_t other)
			{
			dsdf_sample_t ret{*this};
			ret.dsdf.distance.value += other.dsdf.distance.value;
			ret.dsdf.direction       += other.dsdf.direction      ;
			return ret;
			}
		dsdf_sample_t& operator+=(dsdf_sample_t other)
			{
			return *this = *this + other;
			}
		};

	struct sample_t
		{
		utils::graphics::colour::rgba_f gdist{0.f, 0.f, 0.f, 0.f};
		utils::graphics::colour::rgba_f lit{0.f, 0.f, 0.f, 0.f};

		sample_t operator/(float count)
			{
			return {gdist / count, lit / count};
			}
		sample_t operator+(sample_t other)
			{
			return {gdist + other.gdist, lit + other.lit};
			}
		sample_t& operator+=(sample_t other)
			{
			return *this = *this + other;
			}
		};

	struct simple_pointlight
		{
		utils::math::vec3f position{200.f, 50.f, 50.f};
		utils::graphics::colour::rgb_f colour{.0f, 0.3f, .1f};
		float intensity{2.f};
		};

	inline float smoothstep(float edge0, float edge1, float x)
		{
		// Scale, bias and saturate x to 0..1 range
		x = std::clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
		// Evaluate polynomial
		return x * x * (3.f - 2.f * x);
		}

	inline utils::graphics::colour::rgba_f apply_light(const utils::math::vec2f& coords_f, const utils::math::geometry::sdf::direction_signed_distance& gdist, const simple_pointlight& simple_pointlight, const float edge_angled_area_thickness)
		{
		if (gdist.distance.side().is_outside())
			{
			return {0.f, 0.f, 0.f, 0.f};
			}
		const float z{std::clamp(gdist.distance.absolute() / edge_angled_area_thickness, 0.f, 1.f)};
		const float leftover_percent{1.f - z};
		utils::math::vec3f normal{gdist.direction.x() * leftover_percent / 2.f, gdist.direction.y() * leftover_percent / 2.f, z};
		normal.normalize_self();

		//const auto light_direction{utils::math::vec3f{coords_f.x(), coords_f.y(), 0.f} - simple_pointlight.position};
		const auto light_direction{-(utils::math::vec3f{coords_f.x(), coords_f.y(), 0.f} - simple_pointlight.position).normalize()};

		const float lightmap{utils::math::vec3f::dot(light_direction, normal)};

		return utils::math::clamp(simple_pointlight.colour * simple_pointlight.intensity * lightmap, 0.f, 1.f);
		}

	inline utils::graphics::colour::rgba_u rgba_f_to_u(const utils::graphics::colour::rgba_f& rgba_f)
		{
		return utils::graphics::colour::rgba_u
			{
			utils::math::cast_clamp<uint8_t>(rgba_f[0] * 255.f),
			utils::math::cast_clamp<uint8_t>(rgba_f[1] * 255.f),
			utils::math::cast_clamp<uint8_t>(rgba_f[2] * 255.f),
			utils::math::cast_clamp<uint8_t>(rgba_f[3] * 255.f)
			};
		}
	}