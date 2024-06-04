#pragma once

#include <iostream>

#include "base.h"

namespace utils::details::vector
	{
	template<const char* name>
	struct utils_oop_empty_bases output
		{
		inline static constexpr const char* static_name{name}; 
		};
	}

namespace utils::output
	{
	namespace typeless
		{
		template <utils::details::vector::concepts::vector T>
		::std::ostream& operator<<(::std::ostream& os, const T& container);
		//	{
		//	return utils::output::typeless::operator<<(os, container.array);
		//	}
		}

	template <utils::details::vector::concepts::vector T>
	::std::ostream& operator<<(::std::ostream& os, const T& container);
	//	{
	//	namespace ucc = utils::console::colour;
	//	os << ucc::type << T::static_name << T::static_size << typeid(typename T::value_type).name();
	//	return utils::output::typeless::operator<<(os, container);
	//	}
	}