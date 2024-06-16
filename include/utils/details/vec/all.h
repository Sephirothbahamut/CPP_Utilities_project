#pragma once

#include "base.h"
#include "output.h"
#include "../../storage.h"
#include "definitions.h"
#include "memberwise_operators.h"

namespace utils::details::vector
	{
	template<typename T, size_t size, template <typename, size_t> class unspecialized_derived_t, const char* name>
	struct utils_oop_empty_bases base :
		concept_common_flag_type,
		definitions<T, size, unspecialized_derived_t>,
		utils::storage::multiple<T, size, false>,
		memberwise_operators<definitions<T, size, unspecialized_derived_t>>,
		output<name>
		{
		using typename definitions<T, size, unspecialized_derived_t>::self_t       ;
		using typename definitions<T, size, unspecialized_derived_t>::nonref_self_t;
		using typename definitions<T, size, unspecialized_derived_t>::storage_t    ;

		using storage_t::multiple;
		using storage_t::extent;
		using storage_t::storage_type;
		using memberwise_operators<definitions<T, size, unspecialized_derived_t>>::operator=;
		
		using typename storage_t::value_type            ;
		using typename storage_t::const_aware_value_type;
		using typename storage_t::iterator              ;
		using typename storage_t::const_iterator        ;
		using typename storage_t::reverse_iterator      ;
		using typename storage_t::const_reverse_iterator;
		};
	}