#pragma once

#include <string_view>

#include "common.h"
#include "../../output/std_containers.h"

namespace utils::details::vec
	{

	class concept_output_flag_type {};

	template<const char* NAME, typename DERIVED_T>
	struct
#ifdef utils_compiler_msvc
		//Other compilers make empty bases occupy 0, MSVC doesn't always do that without the following line:
		__declspec(empty_bases)
		#endif
	output : public concept_output_flag_type
		{
		using derived_t = DERIVED_T;
		inline static constexpr const char* name{NAME}; 
		};

	namespace concepts
		{
		template<typename T>
		concept output = std::derived_from<T, concept_output_flag_type>;
		}
	}

namespace utils::output
	{
	namespace typeless
		{
		template <utils::details::vec::concepts::output T>
		inline ::std::ostream& operator<<(::std::ostream& os, const T& container)
			{
			return utils::output::typeless::operator<<(os, container.array);
			}
		}

	template <utils::details::vec::concepts::output T>
	inline ::std::ostream& operator<<(::std::ostream& os, const T& container)
		{
		namespace ucc = utils::console::colour;
		os << ucc::type << T::name << T::static_size << typeid(typename T::value_type).name();
		return utils::output::typeless::operator<<(os, container);
		}
	}