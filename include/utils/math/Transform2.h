#pragma once

#include "../cout_utilities.h"
#include "../custom_operators.h"
#include "angle.h"

#include "Vec2.h"

namespace utils::math
	{
	class transform2
		{
		private:

		public:
			//transform2() = default;
			//transform2(vec2f translation = {}, utils::math::angle::deg rotation = {}, float scaling = {1.f}) noexcept : translation(translation), rotation(rotation), scaling(scaling) {}
			constexpr static transform2 zero() noexcept { return {{}, {}, 0.f}; }

			vec2f translation{};
			utils::math::angle::rad	rotation{};
			float scaling{1.f};

			inline friend vec2f  operator* (const vec2f& v, const transform2& t) noexcept { return ((v * t.scaling) + t.rotation) + t.translation; }
			inline friend vec2f& operator*=(      vec2f& v, const transform2& t) noexcept { return v = (v * t); }

			transform2  operator+ (const transform2& oth) const noexcept 
				{
				return {translation + oth.translation, rotation + oth.rotation, scaling * oth.scaling}; 
				}
			transform2& operator+=(const transform2& oth)       noexcept { return *this = *this + oth; }
			transform2  operator- ()                      const noexcept
				{
				transform2 ret{ -translation, -rotation, 1.f / scaling };
				ret.translation = ((ret.translation * ret.scaling) + ret.rotation); //TODO not sure
				return ret;
				}
			transform2& operator--()                            noexcept { return *this = -(*this); }

			transform2  operator* (float delta_time) const noexcept { return  {translation * delta_time, rotation * delta_time, scaling}; }
			transform2& operator*=(float delta_time)       noexcept { *this = {translation * delta_time, rotation * delta_time, scaling}; return *this; }

			bool operator==(const transform2& oth) const noexcept { return translation == oth.translation && rotation == oth.rotation && scaling == oth.scaling; }
			bool operator!=(const transform2& oth) const noexcept { return !(*this == oth); }

			transform2  composite(const transform2& oth)  const noexcept { return *this +  oth; }//TODO fix, adding elements is not compose :(
			transform2& compose  (const transform2& oth)        noexcept { return *this += oth; }//TODO fix, adding elements is not compose :(
			transform2  inverse()                         const noexcept { return  -(*this); }
			transform2& invert ()                               noexcept { return --(*this); }

			
		};

	}

namespace utils
{
	inline math::transform2 lerp(math::transform2 a, math::transform2 b, float t) noexcept
	{
		return {	utils::lerp(a.translation, b.translation, t),
					utils::lerp(a.rotation, b.rotation, t),
					utils::lerp(a.scaling, b.scaling, t) };
	}
}

#ifdef COUT_CONTAINERS
namespace utils::cout
	{
	inline std::ostream& operator<<(std::ostream& os, const utils::math::transform2& transform) noexcept
		{
		namespace ccu = cout::support;
		return os << ccu::type << "Transform" << ccu::brace << "("
			<< ccu::type << "translation: " << ccu::value << transform.translation << ccu::separ << ", "
			<< ccu::type << "rotation: " << ccu::value << transform.rotation << ccu::separ << ", "
			<< ccu::type << "scaling: " << ccu::value << transform.scaling << ccu::brace << ")";
		}
	}
#endif