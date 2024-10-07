
#include <utils/math/math.h>
#include <utils/math/vec3.h>
#include <utils/graphics/colour.h>
#include <utils/math/geometry/shape/sdf/return_types.h>

namespace gsdf_helpers
	{
	struct gsdf_sample_t
		{
		utils::math::geometry::sdf::gradient_signed_distance gsdf;
		
		gsdf_sample_t operator/(float count)
			{
			gsdf_sample_t ret{*this};
			ret.gsdf.distance.value /= count;
			ret.gsdf.gradient       /= count;
			return ret;
			}
		gsdf_sample_t operator+(gsdf_sample_t other)
			{
			gsdf_sample_t ret{*this};
			ret.gsdf.distance.value += other.gsdf.distance.value;
			ret.gsdf.gradient       += other.gsdf.gradient      ;
			return ret;
			}
		gsdf_sample_t& operator+=(gsdf_sample_t other)
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


	inline utils::graphics::colour::rgba_f gradient_sdf_from_gdist(utils::math::geometry::sdf::gradient_signed_distance gdist)
		{
		// Inigo Quilez fancy colors
		gdist.distance.value *= .006f;
		utils::math::vec3f colour = (gdist.distance.side().is_outside()) ? utils::math::vec3f{.9f, .6f, .3f} : utils::math::vec3f{.4f, .7f, .85f};
		colour = utils::math::vec3f{gdist.gradient.x() * .5f + .5f, gdist.gradient.y() * .5f + .5f, 1.f};
		colour *= 1.0f - 0.5f * std::exp(-16.0f * gdist.distance.absolute());
		colour *= 0.9f + 0.1f * std::cos(150.0f * gdist.distance.value);
		colour = utils::math::lerp(colour, utils::math::vec3f{1.f}, 1.f - smoothstep(0.f, .01f, gdist.distance.absolute()));
	
		if (gdist.distance.side().is_inside())
			{
			colour *= .5f;
			}

		return utils::graphics::colour::rgba_f
			{
			colour[0],
			colour[1],
			colour[2],
			1.f
			};
		}

	inline utils::graphics::colour::rgba_f apply_light(const utils::math::vec2f& coords_f, const utils::math::geometry::sdf::gradient_signed_distance& gdist, const simple_pointlight& simple_pointlight, const float edge_angled_area_thickness)
		{
		if (gdist.distance.side().is_outside())
			{
			return {0.f, 0.f, 0.f, 0.f};
			}
		const float z{std::clamp(gdist.distance.absolute() / edge_angled_area_thickness, 0.f, 1.f)};
		const float leftover_percent{1.f - z};
		utils::math::vec3f normal{gdist.gradient.x() * leftover_percent / 2.f, gdist.gradient.y() * leftover_percent / 2.f, z};
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