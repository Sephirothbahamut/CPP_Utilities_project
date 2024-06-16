#pragma once

#include "../details/vec/output.h"
#include "../console/colour.h"

namespace utils::output
	{
	namespace typeless
		{
		template <utils::details::vector::concepts::vector T>
		::std::ostream& operator<<(::std::ostream& os, const T& container)
			{
			return utils::output::typeless::operator<<(os, container.storage);
			}
		}

	template <utils::details::vector::concepts::vector T>
	::std::ostream& operator<<(::std::ostream& os, const T& container)
		{
		namespace ucc = utils::console::colour;
		os << ucc::type << T::static_name << T::extent << typeid(typename T::value_type).name();
		return utils::output::typeless::operator<<(os, container);
		}
	}