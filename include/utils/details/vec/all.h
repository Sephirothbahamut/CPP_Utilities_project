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
		using utils::storage::multiple<T, size, false>::multiple;
		using utils::storage::multiple<T, size, false>::extent;
		using utils::storage::multiple<T, size, false>::storage_type;
		using memberwise_operators<definitions<T, size, unspecialized_derived_t>>::operator=;
		
		using typename definitions<T, size, unspecialized_derived_t>::self_t           ;
		using typename definitions<T, size, unspecialized_derived_t>::nonref_self_t    ;
		using typename definitions<T, size, unspecialized_derived_t>::storage_t        ;
		using typename utils::storage::multiple<T, size, false>::value_type            ;
		using typename utils::storage::multiple<T, size, false>::const_aware_value_type;
		using typename utils::storage::multiple<T, size, false>::iterator              ;
		using typename utils::storage::multiple<T, size, false>::const_iterator        ;
		using typename utils::storage::multiple<T, size, false>::reverse_iterator      ;
		using typename utils::storage::multiple<T, size, false>::const_reverse_iterator;
		};
	}