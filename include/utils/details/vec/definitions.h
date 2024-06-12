#pragma once

#include "base.h"

namespace utils::details::vector
	{
	template<typename T, size_t size, template <typename, size_t> class unspecialized_derived_T>
	struct utils_oop_empty_bases definitions
		{
		using storage_t = utils::storage::multiple<T, size, false>;

		template <typename T, size_t size>
		using unspecialized_derived_t = unspecialized_derived_T<T, size>;

		using self_t        = unspecialized_derived_t<T, size>;
		using nonref_self_t = unspecialized_derived_t<typename storage_t::value_type, size>;
		};
	}