#pragma once

#include "../rect.h"

namespace utils::math::geometry
	{
	using aabb                      = utils::math::rect<float>;
	using axis_aligned_bounding_box = utils::math::rect<float>;
	}



namespace utils::output
	{
	namespace typeless
		{
		inline ::std::ostream& operator<<(::std::ostream& os, const utils::math::geometry::aabb& aabb)
			{
			namespace ucc = utils::console::colour;

			os  << ucc::brace << "(" 
				<< ucc::type << "left"  << ucc::separ << ": " << ucc::value << aabb.ll << ucc::separ << ", "
				<< ucc::type << "up"    << ucc::separ << ": " << ucc::value << aabb.up << ucc::separ << ", "
				<< ucc::type << "right" << ucc::separ << ": " << ucc::value << aabb.rr << ucc::separ << ", "
				<< ucc::type << "down"  << ucc::separ << ": " << ucc::value << aabb.dw << ucc::separ
				<< ucc::brace << ")";
			return os;
			}
		}

	inline ::std::ostream& operator<<(::std::ostream& os, const utils::math::geometry::aabb& aabb)
		{
		namespace ucc = utils::console::colour;
		os << ucc::type << "aabb";
		return utils::output::typeless::operator<<(os, aabb);
		}
	}