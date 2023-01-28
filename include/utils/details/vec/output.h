#pragma once

#include <string_view>

#include "common.h"
#include "../../template_wrappers.h"
#include "../../console/colour.h"

namespace utils::details::vec
	{
	template<const char* NAME, typename DERIVED_T>
	struct output 
		{
		using derived_t = DERIVED_T;
		inline static constexpr const char* name{NAME}; 
		};

	namespace concepts
		{
		template<typename T>
		concept output = std::derived_from<T, vec::output<T::name, typename T::derived_t>>;
		}
	}

namespace utils::output
	{
	template <utils::details::vec::concepts::output T>
	inline ::std::ostream& operator<<(::std::ostream& os, const T& container)
		{
		namespace ucc = utils::console::colour;
		os << ucc::type << T::name << T::static_size << typeid(typename T::value_type).name();
		return utils::output::typeless::operator<<(os, container);
		}
	}