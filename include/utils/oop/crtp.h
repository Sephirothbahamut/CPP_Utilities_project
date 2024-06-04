#pragma once

#include <array>
#include <memory>
#include <cassert>
#include <iostream>
#include <stdexcept>

#include "../compilation/gpu.h"

namespace utils::oop
	{
	namespace details
		{
		template <typename crtp_t>
		struct crtp_concept_friend;
		}

	template <typename derived_T, typename unique_t = void>
	class crtp
		{
		template <typename crtp_t>
		friend struct details::crtp_concept_friend;
		protected:
			using derived_t = derived_T;
			utils_gpu_available constexpr const derived_t& derived() const noexcept { return static_cast<const derived_t&>(*this); }
			utils_gpu_available constexpr       derived_t& derived()       noexcept { return static_cast<      derived_t&>(*this); }

			//Thanks Alice from the #include discord
			utils_gpu_available constexpr void unimplemented_interface() const noexcept { static_assert(sizeof(derived_T) && false, "unimplemented CRTP interface"); }
		};

	namespace details
		{
		template <typename crtp_t>
		struct crtp_concept_friend { using derived_t = crtp_t::derived_t; };
		}

	namespace concepts
		{
		template <typename T>
		concept crtp = (std::derived_from<T, utils::oop::crtp<typename details::crtp_concept_friend<T>::derived_t>>);

		template <typename T, template <typename> class parent_crtp_T>
		concept derived_from_crtp = crtp<parent_crtp_T<T>> && std::derived_from<T, parent_crtp_T<T>>;
		}
	}

//////////////////////////////////////////// Usage
//	
//	template <typename derived_t>
//	struct mycrtp : utils::oop::crtp<derived_t>
//		{
//		using crtp = utils::oop::crtp<derived_t>;
//		int something() {  crtp::unimplemented_interface(); return 0; }
//		int stuff() { return crtp::derived().something() * 5; }
//		};
//	
//	struct myclass : mycrtp<myclass>
//		{
//		int something() { return 3; }
//		};
//	
//	struct myclass_wrong : mycrtp<myclass_wrong>
//		{
//		
//		};
//	
//	static_assert(utils::oop::concepts::crtp<mycrtp<int>>);
//	static_assert(utils::oop::concepts::crtp<myclass>);
//	static_assert(utils::oop::concepts::derived_from_crtp<myclass, mycrtp>);
//	
//	int main()
//		{
//		myclass inst;
//		std::cout << inst.stuff() << std::endl;
//	
//		myclass_wrong inst_wrong; //fail compilation with readable error message
//		std::cout << inst_wrong.stuff() << std::endl;
//		}

