#pragma once

#include "../cout_utilities.h"
#include "../custom_operators.h"
#include "angle.h"

#include "Vec2.h"

namespace utils::math
	{

	class Transform2
		{
		private:

		public:
			//Transform2() = default;
			Transform2(vec2f translation = {}, utils::angle::deg rotation = {}, float scaling = {1.f}) noexcept : position(translation), orientation(rotation), size(scaling) {}
			static Transform2 zero() { return {}; }

			vec2f             position{};
			utils::angle::deg orientation{};
			float             size{1.f};

			inline friend vec2f  operator* (const vec2f& v, const Transform2& t) noexcept { return ((v * t.size) + t.orientation) + t.position; }
			inline friend vec2f& operator*=(      vec2f& v, const Transform2& t) noexcept { return v = (v * t); }

			Transform2  operator+ (const Transform2& oth) const noexcept 
				{
				return {position + oth.position, orientation + oth.orientation, size * oth.size}; 
				}
			Transform2& operator+=(const Transform2& oth)       noexcept { return *this = *this + oth; }
			Transform2  operator- ()                      const noexcept
				{
				Transform2 ret{ -position, -orientation, 1.f / size };
				ret.position = ((ret.position * ret.size) + ret.orientation); //TODO not sure
				return ret;
				}
			Transform2& operator--()                            noexcept { return *this = -(*this); }

			bool     operator==(const Transform2& oth) const noexcept { return position == oth.position && orientation == oth.orientation && size == oth.size; }
			bool     operator!=(const Transform2& oth) const noexcept { return !(*this == oth); }

			Transform2  composite(const Transform2& oth)  const noexcept { return *this +  oth; }
			Transform2& compose  (const Transform2& oth)        noexcept { return *this += oth; }
			Transform2  inverse()                         const noexcept { return  -(*this); }
			Transform2& invert ()                               noexcept { return --(*this); }

			static Transform2 lerp(Transform2 a, Transform2 b, float position)
				{
				return { vec2f::lerp(a.position, b.position, position),
					utils::lerp(a.orientation.value, b.orientation.value, position),
					utils::lerp(a.size, b.size, position) };
				}
		};

	}

namespace utils::cout
	{
	inline std::ostream& operator<<(std::ostream& os, const utils::math::Transform2& transform) noexcept
		{
		namespace ccu = cout::support;
		return os << ccu::type << "Transform" << ccu::brace << "("
			<< ccu::type << "Position: " << ccu::value << transform.position << ccu::separ << ", "
			<< ccu::type << "Orientation: " << ccu::value << transform.orientation << ccu::separ << ", "
			<< ccu::type << "Size: " << ccu::value << transform.size << ccu::brace << ")";
		}
	}
