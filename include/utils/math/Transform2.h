#pragma once

#include "../cout_utilities.h"

#include "angle.h"

#include "Vec2.h"

namespace engine::core
	{
	class Transform2
		{
		private:

		public:
			Transform2(Vec2f translation = {}, utils::angle::deg rotation = {}, float scaling = {}) noexcept : position(translation), orientation(rotation), size(scaling) {}
			static Transform2 zero() { return {}; }

			Vec2f             position{};
			utils::angle::deg orientation{};
			float             size{};
			
			Vec2f&    translate  (      Vec2f& v)    const noexcept { return v += position; }
			Vec2f&    rotate     (      Vec2f& v)    const noexcept { return v += orientation; }
			Vec2f&    scale      (      Vec2f& v)    const noexcept { return v *= size; }
			Vec2f     translation(const Vec2f& v)    const noexcept { return v +  position; }
			Vec2f     rotation   (const Vec2f& v)    const noexcept { return v +  orientation; }
			Vec2f     scaling    (const Vec2f& v)    const noexcept { return v *  size; }
			
			Vec2f&    transform  (      Vec2f& v)    const noexcept { return translate(rotate(scale(v))); }
			Vec2f     transformed(const Vec2f& v)    const noexcept { Vec2f ret{v}; return transform(ret); }

			Transform2  operator+ (const Transform2& oth) const noexcept 
				{
				return {position + oth.position, orientation + oth.orientation, size * oth.size}; 
				}
			Transform2& operator+=(const Transform2& oth)       noexcept { return *this = *this + oth; }
			Transform2  operator- ()                      const noexcept
				{
				Transform2 ret{ -position, -orientation, 1.f / size };
				ret.rotate(ret.scale(ret.position)); //TODO not sure
				return ret;
				}
			Transform2& operator--()                            noexcept { return *this = -(*this); }

			bool     operator==(const Transform2& oth) const noexcept { return position == oth.position && orientation == oth.orientation && size == oth.size; }
			bool     operator!=(const Transform2& oth) const noexcept { return !(*this == oth); }

			Transform2  composite(const Transform2& oth)  const noexcept { return *this +  oth; }
			Transform2& compose(const Transform2& oth)          noexcept { return *this += oth; }
			Transform2  inverse()                         const noexcept { return  -(*this); }
			Transform2  invert()                                noexcept { return --(*this); }

			static Transform2 lerp(Transform2 a, Transform2 b, float position)
				{
				return { Vec2f::lerp(a.position, b.position, position),
					utils::lerp(a.orientation.value, b.orientation.value, position),
					utils::lerp(a.size, b.size, position) };
				}
		};
	}

namespace utils::cout
	{
	inline std::ostream& operator<<(std::ostream& os, const engine::core::Transform2& transform) noexcept
		{
		namespace ccu = cout::support;
		return os << ccu::type << "Transform" << ccu::brace << "("
			<< ccu::type << "Position: " << ccu::value << transform.position << ccu::separ << ", "
			<< ccu::type << "Orientation: " << ccu::value << transform.orientation << ccu::separ << ", "
			<< ccu::type << "Size: " << ccu::value << transform.size << ccu::brace << ")";
		}
	}
