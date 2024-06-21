#pragma once

#include <span>
#include <array>
#include <vector>
#include <memory>
#include <cassert>
#include <concepts>

#include "math/math.h"
#include "compilation/debug.h"
#include "oop/disable_move_copy.h"

namespace utils::storage
	{
	namespace concepts
		{
		template <typename T, typename value_type>
		concept compatible_value_type = std::convertible_to<utils::remove_const_reference_t<T>, utils::remove_const_reference_t<value_type>>;
		template <typename T, typename value_type>
		concept can_construct_value_type = std::constructible_from<value_type, T>;

		template <typename T>
		concept vector = std::same_as<std::remove_cvref_t<T>, std::vector<typename std::remove_cvref_t<T>::value_type>>;
		template <typename T>
		concept array = std::same_as<std::remove_cvref_t<T>, std::array<typename std::remove_cvref_t<T>::value_type, std::tuple_size<std::remove_cvref_t<T>>::value>>; //Note: using tuple_size_t won't work
		template <typename T>
		concept span = std::same_as<std::remove_cvref_t<T>, std::span<typename std::remove_cvref_t<T>::element_type, std::remove_cvref_t<T>::extent>>;
		}

	struct type
		{
		utils_gpu_available consteval bool is_observer() const noexcept { return !owner_value; }
		utils_gpu_available consteval bool is_owner   () const noexcept { return  owner_value; }
		utils_gpu_available consteval bool is_const   () const noexcept { return  const_value; }
		utils_gpu_available consteval bool can_construct_from_const() const noexcept { return owner_value || const_value; }

		struct create : utils::oop::non_constructible
			{
			template <typename T2>
			utils_gpu_available static consteval type from          (                        ) noexcept { return {.owner_value{!std::is_reference_v<T2>}, .const_value{std::is_const_v<std::remove_reference_t<T2>>}}; }
			utils_gpu_available static consteval type owner         (bool const_value = false) noexcept { return {.owner_value{true                    }, .const_value{const_value                                 }}; }
			utils_gpu_available static consteval type observer      (bool const_value = false) noexcept { return {.owner_value{false                   }, .const_value{const_value                                 }}; }
			utils_gpu_available static consteval type const_observer(                        ) noexcept { return {.owner_value{false                   }, .const_value{true                                        }}; }
			};

		const bool owner_value;
		const bool const_value;
		};

	template <typename A, typename B>
	struct constness_matching
		{
		static constexpr bool other_const{std::remove_cvref_t<B>::storage_type.is_const() || std::is_const_v<B>};
		static constexpr bool compatible_constness{std::remove_cvref_t<A>::storage_type.is_const() || std::remove_cvref_t<A>::storage_type.is_owner() || !other_const};
		};

	template <typename T, type storage_type>
	using storage_type_for = std::conditional_t
		/**/<
		/**/storage_type.is_owner(),
		/**/std::conditional_t
		/**//**/<
		/**//**/storage_type.is_const(),
		/**//**/const T,
		/**//**/T
		/**//**/>,
		/**/std::conditional_t
		/**//**/<
		/**//**/storage_type.is_const(),
		/**//**/const T&,
		/**//**/T&
		/**//**/>
		/**/>;

	template <typename T>
	struct single
		{
		using value_type = std::remove_reference_t<T>;
		inline static constexpr type storage_type{utils::storage::type::create::from<T>()};

		using inner_storage_t = std::conditional_t
			<
			storage_type.is_owner(),
			T,
			std::reference_wrapper<T>
			>;

		inner_storage_t storage;

		utils_gpu_available constexpr          const value_type& value     () const noexcept                                    { return static_cast<const value_type&>(storage); }
		utils_gpu_available constexpr                value_type& value     ()       noexcept requires(!storage_type.is_const()) { return static_cast<      value_type&>(storage); }
		utils_gpu_available constexpr operator const value_type&           () const noexcept                                    { return  value(); }
		utils_gpu_available constexpr operator       value_type&           ()       noexcept requires(!storage_type.is_const()) { return  value(); }
		utils_gpu_available constexpr          const value_type* operator->() const noexcept                                    { return &value(); }
		utils_gpu_available constexpr                value_type* operator->()       noexcept requires(!storage_type.is_const()) { return &value(); }
		};

	template <typename T, size_t extent, bool sequential_observer>
	struct multiple;

	namespace concepts
		{
		template <typename T>
		concept multiple = std::derived_from<std::remove_cvref_t<T>, utils::storage::multiple<typename std::remove_cvref_t<T>::template_type, std::remove_cvref_t<T>::extent, std::remove_cvref_t<T>::sequential_observer>>;
		}


	template <typename T, size_t EXTENT = std::dynamic_extent, bool SEQUENTIAL_OBSERVER = true>
	struct multiple
		{
		using template_type = T;
		using const_aware_value_type = std::remove_reference_t<T>;
		using value_type = std::remove_cvref_t<T>;
		inline static constexpr type storage_type{utils::storage::type::create::from<T>()};
		inline static constexpr bool sequential_observer{SEQUENTIAL_OBSERVER};
		inline static constexpr size_t extent{EXTENT};
		using self_t = multiple<T, extent, sequential_observer>;

		template <typename T2>
		using owner_storage_t = std::conditional_t<extent == std::dynamic_extent, std::vector<T2>, std::array<T2, extent>>;

		using inner_storage_t = std::conditional_t
			<
			storage_type.is_owner(),
			owner_storage_t<const_aware_value_type>,
			std::conditional_t
			/**/<
			/**/sequential_observer,
			/**/std::span<const_aware_value_type, extent>,
			/**/owner_storage_t<std::reference_wrapper<const_aware_value_type>>
			/**/>
			>;

		inner_storage_t storage;

		utils_gpu_available constexpr size_t size() const noexcept { return storage.size(); }
		utils_gpu_available constexpr const auto data() const noexcept { return storage.data(); }
		utils_gpu_available constexpr       auto data()       noexcept { return storage.data(); }

		utils_gpu_available constexpr const       value_type& operator[](size_t index) const noexcept                                    { return static_cast<const       value_type&>(storage   [index]); }
		utils_gpu_available constexpr const_aware_value_type& operator[](size_t index)       noexcept requires(!storage_type.is_const()) { return static_cast<const_aware_value_type&>(storage   [index]); }
		utils_gpu_available constexpr const       value_type& at        (size_t index) const                                             { return static_cast<const       value_type&>(storage.at(index)); }
		utils_gpu_available constexpr const_aware_value_type& at        (size_t index)                requires(!storage_type.is_const()) { return static_cast<const_aware_value_type&>(storage.at(index)); }
 
		utils_gpu_available constexpr void rebind(size_t index, const_aware_value_type& new_value) noexcept requires(storage_type.is_observer())
			{
			if constexpr (utils::compilation::debug) { assert(index < size()); }
			storage[index] = new_value;
			}

		template <typename inner_iterator_t, typename T2>
		struct base_iterator
			{
			using self_t            = base_iterator<inner_iterator_t, T2>;
			using iterator_category = std::random_access_iterator_tag;
			using difference_type   = std::ptrdiff_t;
			using value_type        = T2;
			using pointer           = T2*;
			using reference         = T2&;

			inner_iterator_t inner_iterator;

			utils_gpu_available constexpr self_t  operator+ (difference_type value) const noexcept { return {inner_iterator + value}; }
			utils_gpu_available constexpr self_t  operator- (difference_type value) const noexcept { return {inner_iterator - value}; }
			utils_gpu_available constexpr self_t& operator+=(difference_type value) const noexcept { inner_iterator += value; return *this; }
			utils_gpu_available constexpr self_t& operator-=(difference_type value) const noexcept { inner_iterator += value; return *this; }

			utils_gpu_available constexpr self_t& operator++(   ) noexcept { inner_iterator++; return *this; }
			utils_gpu_available constexpr self_t& operator--(   ) noexcept { inner_iterator--; return *this; }
			utils_gpu_available constexpr self_t  operator++(int) noexcept { self_t tmp{*this}; ++(*this); return tmp; }
			utils_gpu_available constexpr self_t  operator--(int) noexcept { self_t tmp{*this}; --(*this); return tmp; }

			utils_gpu_available constexpr auto operator<=>(const self_t& other) const noexcept { return inner_iterator <=> other.inner_iterator; }
			utils_gpu_available constexpr auto operator== (const self_t& other) const noexcept { return inner_iterator ==  other.inner_iterator; }
			
			utils_gpu_available constexpr reference operator* () noexcept { return static_cast<reference>(inner_iterator.operator* ()); }
			utils_gpu_available constexpr pointer   operator->() noexcept { return static_cast<pointer  >(inner_iterator.operator->()); }
			};

		using iterator               = base_iterator<typename inner_storage_t::iterator              ,       const_aware_value_type>;
		using reverse_iterator       = base_iterator<typename inner_storage_t::reverse_iterator      ,       const_aware_value_type>;
		//TODO When CUDA supports C++23 this will suffice for all containers. Until then, std::span doesn't have const_iterator defined in C++20
		//using const_iterator         = base_iterator<typename inner_storage_t::const_iterator        , const const_aware_value_type>;
		//using const_reverse_iterator = base_iterator<typename inner_storage_t::const_reverse_iterator, const const_aware_value_type>;
		//until then, keep this uglyness
		template <typename       T> struct inner_storage_const_iterator            { using type =       typename T::const_iterator        ; };
		template <typename       T> struct inner_storage_const_reverse_iterator    { using type =       typename T::const_reverse_iterator; };
		template <concepts::span T> struct inner_storage_const_iterator        <T> { using type = const typename T::      iterator        ; };
		template <concepts::span T> struct inner_storage_const_reverse_iterator<T> { using type = const typename T::      reverse_iterator; };

		template <typename T> using inner_storage_const_iterator_t         = typename inner_storage_const_iterator        <T>::type;
		template <typename T> using inner_storage_const_reverse_iterator_t = typename inner_storage_const_reverse_iterator<T>::type;

		using const_iterator         = base_iterator<inner_storage_const_iterator_t        <inner_storage_t>, const const_aware_value_type>;
		using const_reverse_iterator = base_iterator<inner_storage_const_reverse_iterator_t<inner_storage_t>, const const_aware_value_type>;
		
		utils_gpu_available constexpr       iterator           begin()       noexcept { return {storage. begin ()}; }
		utils_gpu_available constexpr const_iterator           begin() const noexcept { return {storage. begin ()}; }
		utils_gpu_available constexpr const_iterator          cbegin()       noexcept { if constexpr (!concepts::span<inner_storage_t>) { return {storage.cbegin()}; } else { return {storage.begin()}; } }
		utils_gpu_available constexpr       iterator           end  ()       noexcept { return {storage. end   ()}; }
		utils_gpu_available constexpr const_iterator           end  () const noexcept { return {storage. end   ()}; }
		utils_gpu_available constexpr const_iterator          cend  ()       noexcept { if constexpr (!concepts::span<inner_storage_t>) { return {storage.cend  ()}; } else { return {storage.end  ()}; } }
			 
		utils_gpu_available constexpr       reverse_iterator  rbegin()       noexcept { return {storage. rbegin()}; }
		utils_gpu_available constexpr const_reverse_iterator  rbegin() const noexcept { return {storage. rbegin()}; }
		utils_gpu_available constexpr const_reverse_iterator crbegin()       noexcept { if constexpr (!concepts::span<inner_storage_t>) { return {storage.crbegin()}; } else { return {storage.rbegin()}; } }
		utils_gpu_available constexpr       reverse_iterator  rend  ()       noexcept { return {storage. rend  ()}; }
		utils_gpu_available constexpr const_reverse_iterator  rend  () const noexcept { return {storage. rend  ()}; }
		utils_gpu_available constexpr const_reverse_iterator crend  ()       noexcept { if constexpr (!concepts::span<inner_storage_t>) { return {storage.crend  ()}; } else { return {storage.crend  ()}; } }

		utils_gpu_available constexpr multiple() requires(storage_type == ::utils::storage::type::owner) = default;

		utils_gpu_available constexpr multiple(inner_storage_t&& storage) : storage{storage} {}

		template <concepts::can_construct_value_type<typename inner_storage_t::value_type> ...Args>
		utils_gpu_available constexpr multiple(Args&&... args)
			requires
				(
				concepts::vector<inner_storage_t> || 
					(
					concepts::array<inner_storage_t> &&
						(
						(storage_type.is_owner   () && sizeof...(Args) <= extent) ||
						(storage_type.is_observer() && sizeof...(Args) == extent)
						)
					)
				) :
			storage{std::forward<Args>(args)...} {}

		template <concepts::multiple other_t>
		static inner_storage_t inner_create(other_t& other) noexcept
			requires 
				(
				std::constructible_from<typename inner_storage_t::value_type, typename other_t::inner_storage_t::reference> 
				&& constness_matching<self_t, other_t>::compatible_constness
				)
			{
			using inner_value_type = typename inner_storage_t::value_type;

			// If owner can construct each element from each other's element freely.
			// If sparse observer with const value can construct without issues
			// If sparse observer without const value, the source mustn't be const and mustn't have const values
			static constexpr bool other_const          {other_t::storage_type.is_const() || std::is_const_v<other_t>};
			static constexpr bool compatible_constness {storage_type.is_const() || storage_type.is_owner() || !other_const};
			static constexpr bool initialize_memberwise{storage_type.is_owner() || (storage_type.is_observer() && !sequential_observer)};
			static constexpr bool other_may_have_less  {storage_type.is_owner()};

			if constexpr (storage_type.is_observer() && extent != std::dynamic_extent)
				{
				if constexpr (other_t::extent != std::dynamic_extent)
					{
					static_assert(extent <= other_t::extent);
					}
				else
					{
					assert(extent <= other.size());
					}
				}

			if constexpr (compatible_constness)
				{
				if constexpr (initialize_memberwise)
					{
					if constexpr (utils::storage::concepts::vector<inner_storage_t> && utils::storage::concepts::vector<typename other_t::inner_storage_t>)
						{
						inner_storage_t ret; 
						ret.reserve(other.storage.size());
						for (auto& element : other.storage) 
							{
							ret.emplace_back(element); 
							}
						return ret;
						}
					else
						{
						static constexpr size_t ret_extent{extent == std::dynamic_extent ? other_t::extent : extent};

						return[&]<std::size_t... is>(std::index_sequence<is...>)
							{
							if constexpr (other_may_have_less) { return inner_storage_t{(is < other.storage.size() ? inner_value_type{other[is]} : inner_value_type{})...}; }
							else                               { return inner_storage_t{                             inner_value_type{other[is]}                      ...}; }
							}(std::make_index_sequence<ret_extent>());
						}
					}
				else if constexpr (storage_type.is_observer() && sequential_observer)
					{
					if constexpr (extent != std::dynamic_extent) { assert(extent == other.size()); }
					return inner_storage_t(&(*(other.begin())), other.size());
					}
				}
			}

		utils_gpu_available constexpr multiple(const concepts::multiple auto& other) noexcept requires(storage_type.can_construct_from_const()) : storage{inner_create(other)} {}
		utils_gpu_available constexpr multiple(      concepts::multiple auto& other) noexcept : storage{inner_create(other)} {}

		//template <concepts::multiple other_t>
		//utils_gpu_available constexpr multiple& operator=(other_t& other) noexcept
		//	{
		//	for (size_t i = 0; i < length; i++)
		//		{
		//
		//		}
		//	}
		};
	}