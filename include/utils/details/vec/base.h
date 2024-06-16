#pragma once

#include <array>
#include <concepts>

#include "../../memory.h"
#include "../../storage.h"
#include "../../oop/crtp.h"
#include "../../compilation/gpu.h"
#include "../../compilation/compiler.h"

namespace utils::details::vector
	{
	/// <summary> This only exists to make the concept way simpler than having to repeat all the template parameters.</summary>
	struct concept_common_flag_type {};

	template<typename T, size_t size, template <typename, size_t> class unspecialized_derived_t, const char* name>
	struct base;

	template<typename T, size_t size, template <typename, size_t> class unspecialized_derived_t>
	struct definitions;
	template<typename definitions_t>
	struct memberwise_operators;
	template<const char* name>
	struct output;

	namespace concepts
		{
		template<typename T>
		concept vector = std::derived_from<T, concept_common_flag_type>;

		template <typename T, typename compatible_to>
		concept compatible_vector = vector<T> && vector<compatible_to> && std::convertible_to<typename T::value_type, typename compatible_to::value_type>;

		template <typename T, typename compatible_to>
		concept compatible_scalar = vector<compatible_to> && std::convertible_to<T, typename compatible_to::value_type>;
		}

	//template <concepts::array a_t, concepts::array b_t>
	template <typename a_t, typename b_t>
	struct get_larger
		{
		using type = std::conditional_t<(a_t::extent > b_t::extent), a_t, b_t>;
		};
	}