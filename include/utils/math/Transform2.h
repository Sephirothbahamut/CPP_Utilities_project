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
			//transform2(vec2f translation = {}, utils::math::angle::deg rotation = {}, float scaling = {1.f}) noexcept : position(translation), orientation(rotation), size(scaling) {}
			constexpr static transform2 zero() noexcept { return {{}, {}, 0.f}; }

			vec2f                   position{};
			utils::math::angle::deg orientation{};
			float                   size{1.f};

			inline friend vec2f  operator* (const vec2f& v, const transform2& t) noexcept { return ((v * t.size) + t.orientation) + t.position; }
			inline friend vec2f& operator*=(      vec2f& v, const transform2& t) noexcept { return v = (v * t); }

			transform2  operator+ (const transform2& oth) const noexcept 
				{
				return {position + oth.position, orientation + oth.orientation, size + oth.size}; 
				}
			transform2& operator+=(const transform2& oth)       noexcept { return *this = *this + oth; }
			transform2  operator- ()                      const noexcept
				{
				transform2 ret{ -position, -orientation, 1.f / size };
				ret.position = ((ret.position * ret.size) + ret.orientation); //TODO not sure
				return ret;
				}
			transform2& operator--()                            noexcept { return *this = -(*this); }

			transform2  operator* (float delta_time) const noexcept { return  {position * delta_time, orientation * delta_time, size * delta_time}; }
			transform2& operator*=(float delta_time)       noexcept { *this = {position * delta_time, orientation * delta_time, size * delta_time}; return *this; }

			bool operator==(const transform2& oth) const noexcept { return position == oth.position && orientation == oth.orientation && size == oth.size; }
			bool operator!=(const transform2& oth) const noexcept { return !(*this == oth); }

			transform2  composite(const transform2& oth)  const noexcept { return *this +  oth; }
			transform2& compose  (const transform2& oth)        noexcept { return *this += oth; }
			transform2  inverse()                         const noexcept { return  -(*this); }
			transform2& invert ()                               noexcept { return --(*this); }

			static transform2 lerp(transform2 a, transform2 b, float position)
				{
				return { vec2f::lerp(a.position, b.position, position),
					utils::lerp(a.orientation.value, b.orientation.value, position),
					utils::lerp(a.size, b.size, position) };
				}
		};

	}

#ifdef COUT_CONTAINERS
namespace utils::cout
	{
	inline std::ostream& operator<<(std::ostream& os, const utils::math::transform2& transform) noexcept
		{
		namespace ccu = cout::support;
		return os << ccu::type << "Transform" << ccu::brace << "("
			<< ccu::type << "Position: " << ccu::value << transform.position << ccu::separ << ", "
			<< ccu::type << "Orientation: " << ccu::value << transform.orientation << ccu::separ << ", "
			<< ccu::type << "Size: " << ccu::value << transform.size << ccu::brace << ")";
		}
	}
#endif