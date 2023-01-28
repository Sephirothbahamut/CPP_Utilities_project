#pragma once

#include <array>
#include <concepts>

#include "../../output/std_containers.h"
#include "../../console/colour.h"

namespace utils::details::vec
	{
	// ...yes I could write all the methods twice, one in the vec base class and the other in the colour base class
	// but why do that when you can overcomplicate your life and waste your time in a funny mess of CRTPs with multiple inheritance? :)

	template<typename T, size_t SIZE, typename DERIVED_T>
	class common;

	namespace concepts
		{
		template<typename T>
		concept array = std::derived_from<T, vec::common<typename T::value_type, T::static_size, T>>;

		template <typename T1, typename T2>
		concept compatible_array = array<T1> && array<T2> && std::convertible_to<typename T1::value_type, typename T2::value_type>;
		}

	template <template<typename, size_t> class UNSPECIALIZED>
	struct store_unspecialized 
		{
		template <typename T, size_t size>
		using unspecialized_t = UNSPECIALIZED<T, size>;
		};

	template<typename T, size_t SIZE, typename DERIVED_T>
	class common
		{
		public:
			using derived_t = DERIVED_T;
	
		private:
			constexpr const derived_t& derived() const noexcept { return static_cast<const derived_t&>(*this); }
			constexpr       derived_t& derived()       noexcept { return static_cast<      derived_t&>(*this); }
			constexpr const auto     & get_arr() const noexcept { return derived().array; }
			constexpr       auto     & get_arr()       noexcept { return derived().array; }
	
			using arr_t = std::array<T, SIZE>;
			
		public:
			inline static constexpr const size_t static_size{std::tuple_size_v<arr_t>};
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
	
			constexpr size_t size() const noexcept { return get_arr().size(); }
	
			constexpr const value_type& operator[](size_t index) const noexcept { return get_arr()   [index]; }
			constexpr       value_type& operator[](size_t index)       noexcept { return get_arr()   [index]; }
			constexpr const value_type& at        (size_t index) const          { return get_arr().at(index); }
			constexpr       value_type& at        (size_t index)                { return get_arr().at(index); }
	
			constexpr       iterator  begin()       noexcept { return get_arr(). begin(); }
			constexpr const_iterator  begin() const noexcept { return get_arr(). begin(); }
			constexpr const_iterator cbegin()       noexcept { return get_arr().cbegin(); }
			constexpr       iterator  end  ()       noexcept { return get_arr(). end  (); }
			constexpr const_iterator  end  () const noexcept { return get_arr(). end  (); }
			constexpr const_iterator cend  ()       noexcept { return get_arr().cend  (); }
	
			constexpr       reverse_iterator  rbegin()       noexcept { return get_arr(). begin(); }
			constexpr const_reverse_iterator  rbegin() const noexcept { return get_arr(). begin(); }
			constexpr const_reverse_iterator crbegin()       noexcept { return get_arr().cbegin(); }
			constexpr       reverse_iterator  rend  ()       noexcept { return get_arr(). end  (); }
			constexpr const_reverse_iterator  rend  () const noexcept { return get_arr(). end  (); }
			constexpr const_reverse_iterator crend  ()       noexcept { return get_arr().cend  (); }
		};
	}

namespace utils::output
	{
	namespace typeless
		{
		template <utils::details::vec::concepts::array T>
		inline ::std::ostream& operator<<(::std::ostream& os, const T& container)
			{
			return utils::output::typeless::operator<<(os, container.array);
			}
		}
	}