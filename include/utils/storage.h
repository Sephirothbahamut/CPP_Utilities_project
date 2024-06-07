#pragma once

#include <span>
#include <array>
#include <vector>
#include <memory>
#include "math/math.h"

namespace utils::storage
	{
	enum class type
		{
		owner, observer, const_observer
		};

	template <type storage, typename T>
	using single = std::conditional_t
		<
		storage == type::owner,
		T,
		std::conditional_t
		/**/<
		/**/storage == type::observer,
		/**/std::reference_wrapper<T>,
		/**/std::reference_wrapper<const T>
		/**/>
		>;

	template <type storage, typename T, size_t extent = std::dynamic_extent>
	using multiple = std::conditional_t
		<
		storage == type::owner,
		std::conditional_t
			<
			extent == std::dynamic_extent,
			std::vector<T>,
			std::array <T, extent>
			>,
		std::conditional_t
		/**/<
		/**/storage == type::observer,
		/**/std::span<      T, extent>,
		/**/std::span<const T, extent>
		/**/>
		>;

	template <typename T>
	inline static consteval type get_type()
		{
		if constexpr (concepts::const_reference<T>) { return type::const_observer; }
		else if constexpr (concepts::reference<T>) { return type::observer; }
		else return type::owner;
		}
	}