#pragma once

#include <array>
#include <concepts>
#include "../../compilation/CUDA.h"
#include "../../memory.h"

namespace utils::details::vec
	{
	// ...yes I could write all the methods twice, one in the vec base class and the other in the colour base class
	// but why do that when you can overcomplicate your life and waste your time in a funny mess of CRTPs with multiple inheritance? :)

	template<typename T, size_t SIZE, typename DERIVED_T, typename NONREF_DERIVED_T>
	class common;

	namespace concepts
		{
		template<typename T>
		concept array = std::derived_from<T, vec::common<typename T::value_type, T::static_size, T, typename T::nonref_derived_t>>;

		template <typename T1, typename T2>
		concept compatible_array = array<T1> && array<T2> && std::convertible_to<typename T1::value_type, typename T2::value_type>;
		}

	template <concepts::array a_t, concepts::array b_t>
	struct get_larger
		{
		using type = std::conditional_t<(a_t::static_size > b_t::static_size), a_t, b_t>;
		};

	template<typename T, size_t SIZE, typename DERIVED_T, typename NONREF_DERIVED_T>
	class common
		{
		public:
			inline static constexpr const size_t static_size{SIZE};
			using derived_t = DERIVED_T;
			using arr_t                   = std::array<T, static_size>;
			using value_type              = typename arr_t::value_type            ;
			using size_type               = typename arr_t::size_type             ;
			using difference_type         = typename arr_t::difference_type       ;
			using reference               = typename arr_t::reference             ;
			using const_reference         = typename arr_t::const_reference       ;
			using pointer                 = typename arr_t::pointer               ;
			using const_pointer           = typename arr_t::const_pointer         ;
			using iterator                = typename arr_t::iterator              ;
			using const_iterator          = typename arr_t::const_iterator        ;
			using reverse_iterator        = typename arr_t::reverse_iterator      ;
			using const_reverse_iterator  = typename arr_t::const_reverse_iterator;
			using nonref_value_type       = typename utils::remove_cvref_t<value_type>;
			using nonref_derived_t        = NONREF_DERIVED_T;
	
		private:
			utils_cuda_available constexpr const derived_t& derived() const noexcept { return static_cast<const derived_t&>(*this); }
			utils_cuda_available constexpr       derived_t& derived()       noexcept { return static_cast<      derived_t&>(*this); }
			utils_cuda_available constexpr const auto     & get_arr() const noexcept { return derived().array; }
			utils_cuda_available constexpr       auto     & get_arr()       noexcept { return derived().array; }
			
		public:
			utils_cuda_available constexpr size_t size() const noexcept { return get_arr().size(); }

			utils_cuda_available constexpr const auto data() const noexcept { return get_arr().data(); }
			utils_cuda_available constexpr       auto data()       noexcept { return get_arr().data(); }
			 
			utils_cuda_available constexpr const value_type& operator[](size_t index) const noexcept { return get_arr()   [index]; }
			utils_cuda_available constexpr       value_type& operator[](size_t index)       noexcept { return get_arr()   [index]; }
			utils_cuda_available constexpr const value_type& at        (size_t index) const          { return get_arr().at(index); }
			utils_cuda_available constexpr       value_type& at        (size_t index)                { return get_arr().at(index); }
			 
			utils_cuda_available constexpr       iterator  begin()       noexcept { return get_arr(). begin(); }
			utils_cuda_available constexpr const_iterator  begin() const noexcept { return get_arr(). begin(); }
			utils_cuda_available constexpr const_iterator cbegin()       noexcept { return get_arr().cbegin(); }
			utils_cuda_available constexpr       iterator  end  ()       noexcept { return get_arr(). end  (); }
			utils_cuda_available constexpr const_iterator  end  () const noexcept { return get_arr(). end  (); }
			utils_cuda_available constexpr const_iterator cend  ()       noexcept { return get_arr().cend  (); }
			 
			utils_cuda_available constexpr       reverse_iterator  rbegin()       noexcept { return get_arr(). begin(); }
			utils_cuda_available constexpr const_reverse_iterator  rbegin() const noexcept { return get_arr(). begin(); }
			utils_cuda_available constexpr const_reverse_iterator crbegin()       noexcept { return get_arr().cbegin(); }
			utils_cuda_available constexpr       reverse_iterator  rend  ()       noexcept { return get_arr(). end  (); }
			utils_cuda_available constexpr const_reverse_iterator  rend  () const noexcept { return get_arr(). end  (); }
			utils_cuda_available constexpr const_reverse_iterator crend  ()       noexcept { return get_arr().cend  (); }
		};
	}

//namespace utils::output
//	{
//	namespace typeless
//		{
//		template <utils::details::vec::concepts::array T>
//		inline ::std::ostream& operator<<(::std::ostream& os, const T& container)
//			{
//			return utils::output::typeless::operator<<(os, container.array);
//			}
//		}
//	}