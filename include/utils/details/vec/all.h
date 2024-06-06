#pragma once

#include "base.h"
#include "definitions.h"
#include "storage.h"
#include "memberwise_operators.h"
#include "output.h"

namespace utils::details::vector
	{
	template<typename T, size_t size, template <typename, size_t> class unspecialized_derived_t, const char* name>
	struct utils_oop_empty_bases base :
		concept_common_flag_type,
		definitions<T, size, unspecialized_derived_t>,
		storage<definitions<T, size, unspecialized_derived_t>>,
		memberwise_operators<definitions<T, size, unspecialized_derived_t>>,
		output<name>
		{
		using storage<definitions<T, size, unspecialized_derived_t>>::storage;
		using storage<definitions<T, size, unspecialized_derived_t>>::operator=;
		
		using definitions<T, size, unspecialized_derived_t>::static_size              ;
		using definitions<T, size, unspecialized_derived_t>::static_value_is_reference;
		using typename definitions<T, size, unspecialized_derived_t>::self_t                ;
		using typename definitions<T, size, unspecialized_derived_t>::nonref_self_t         ;
		using typename definitions<T, size, unspecialized_derived_t>::array_t               ;
		using typename definitions<T, size, unspecialized_derived_t>::value_type            ;
		using typename definitions<T, size, unspecialized_derived_t>::nonref_value_type     ;
		using typename definitions<T, size, unspecialized_derived_t>::iterator              ;
		using typename definitions<T, size, unspecialized_derived_t>::const_iterator        ;
		using typename definitions<T, size, unspecialized_derived_t>::reverse_iterator      ;
		using typename definitions<T, size, unspecialized_derived_t>::const_reverse_iterator;
		};
	}