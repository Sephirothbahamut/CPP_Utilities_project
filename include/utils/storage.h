#pragma once

#include <span>
#include <array>
#include <vector>
#include <memory>
#include "math/math.h"

namespace utils::storage
	{
	namespace concepts
		{
		template <typename T, typename value_type>
		concept compatible_value_type = std::convertible_to<utils::remove_const_reference_t<T>, utils::remove_const_reference_t<value_type>>;
		template <typename T, typename value_type>
		concept can_construct_value_type = std::constructible_from<value_type, T>;

		template <typename T>
		concept vector = std::same_as<T, std::vector<typename T::value_type>>;
		template <typename T>
		concept array = std::same_as<T, std::array<typename T::value_type, std::tuple_size_v<T>>>;
		template <typename T>
		concept span = std::same_as<T, std::span<typename T::value_type, T::extent>>;
		}

	namespace create
		{
		//Thanks Luke from #include Discord server

		template <class value_type, typename T, std::size_t size>
		auto array(std::array<T, size>& x)
			requires(std::constructible_from<value_type, typename std::array<T, size>::reference>)
			{
			return[&]<std::size_t... is>(std::index_sequence<is...>)
				{
				return std::array{value_type{x[is]}...};
				}(std::make_index_sequence<size>());
			}
		template <class value_type, typename T, std::size_t size>
		auto array(const std::array<T, size>& x)
			requires(std::constructible_from<value_type, typename std::array<T, size>::reference>)
			{
			return[&]<std::size_t... is>(std::index_sequence<is...>)
				{
				return std::array{value_type{x[is]}...};
				}(std::make_index_sequence<size>());
			}
		template <typename T, std::size_t size>
		auto array_of_const_references(std::array<T, size>& x)
			{
			return array<std::reference_wrapper<const T>>(x);
			}
		template <typename T, std::size_t size>
		auto array_of_references(std::array<T, size>& x)
			{
			return array<std::reference_wrapper<T>>(x);
			}
		template <typename T, std::size_t size>
		auto array_of_const_references(const std::array<T, size>& x)
			{
			return array<std::reference_wrapper<const T>>(x);
			}
		template <typename T, std::size_t size>
		auto array_of_references(const std::array<T, size>& x)
			{
			return array_of_const_references(x);
			}
		
		template <class value_type, typename T, std::size_t size>
		auto vector(std::array<T, size>& x)
			requires(std::constructible_from<value_type, typename std::array<T, size>::reference>)
			{
			return[&]<std::size_t... is>(std::index_sequence<is...>)
				{
				return std::vector{value_type{x[is]}...};
				}(std::make_index_sequence<size>());
			}
		template <class value_type, typename T, std::size_t size>
		auto vector(const std::array<T, size>& x)
			requires(std::constructible_from<value_type, typename std::array<T, size>::reference>)
			{
			return[&]<std::size_t... is>(std::index_sequence<is...>)
				{
				return std::vector{value_type{x[is]}...};
				}(std::make_index_sequence<size>());
			}
		template <typename T, std::size_t size>
		auto vector_of_const_references(std::array<T, size>& x)
			{
			return vector<std::reference_wrapper<const T>>(x);
			}
		template <typename T, std::size_t size>
		auto vector_of_references(std::array<T, size>& x)
			{
			return vector<std::reference_wrapper<T>>(x);
			}
		template <typename T, std::size_t size>
		auto vector_of_const_references(const std::array<T, size>& x)
			{
			return vector<std::reference_wrapper<const T>>(x);
			}
		template <typename T, std::size_t size>
		auto vector_of_references(const std::array<T, size>& x)
			{
			return vector_of_const_references(x);
			}

		template <typename return_t, typename T, std::size_t size>
		return_t make(std::array<T, size>& x) requires(concepts::vector<return_t>) { return vector<typename return_t::value_type>(x); }
		template <typename return_t, typename T, std::size_t size>
		return_t make(std::array<T, size>& x) requires(concepts::array <return_t>) { return array <typename return_t::value_type>(x); }

		template <typename return_t, typename T, std::size_t size>
		return_t make_references(std::array<T, size>& x) requires(concepts::vector<return_t>) { return vector_of_references<T>(x); }
		template <typename return_t, typename T, std::size_t size>
		return_t make_references(const std::array<T, size>& x) requires(concepts::vector<return_t>) { return vector_of_references<T>(x); }
		template <typename return_t, typename T, std::size_t size>
		return_t make_const_references(std::array<T, size>& x) requires(concepts::vector<return_t>) { return vector_of_const_references<T>(x); }
		template <typename return_t, typename T, std::size_t size>
		return_t make_const_references(const std::array<T, size>& x) requires(concepts::vector<return_t>) { return vector_of_const_references<T>(x); }

		template <typename return_t, typename T, std::size_t size>
		return_t make_references(std::array<T, size>& x) requires(concepts::array<return_t>) { return array_of_references<T>(x); }
		template <typename return_t, typename T, std::size_t size>
		return_t make_references(const std::array<T, size>& x) requires(concepts::array<return_t>) { return array_of_references<T>(x); }
		template <typename return_t, typename T, std::size_t size>
		return_t make_const_references(std::array<T, size>& x) requires(concepts::array<return_t>) { return array_of_const_references<T>(x); }
		template <typename return_t, typename T, std::size_t size>
		return_t make_const_references(const std::array<T, size>& x) requires(concepts::array<return_t>) { return array_of_const_references<T>(x); }

		}


	enum class type
		{
		owner, observer
		};

	template <type storage_type, typename T>
	struct single
		{
		using value_type = T;
		inline static constexpr type static_storage_type{storage_type};
		inline static constexpr bool static_value_is_const{std::is_const_v<value_type>};

		using inner_storage_t = std::conditional_t
			<
			static_storage_type == type::owner,
			T,
			std::reference_wrapper<T>
			>;

		inner_storage_t storage;

		utils_gpu_available constexpr          const value_type& value     () const noexcept                                  { return static_cast<const value_type&>(storage); }
		utils_gpu_available constexpr                value_type& value     ()       noexcept requires(!static_value_is_const) { return static_cast<      value_type&>(storage); }
		utils_gpu_available constexpr operator const value_type&           () const noexcept                                  { return  value(); }
		utils_gpu_available constexpr operator       value_type&           ()       noexcept requires(!static_value_is_const) { return  value(); }
		utils_gpu_available constexpr          const value_type* operator->() const noexcept                                  { return &value(); }
		utils_gpu_available constexpr                value_type* operator->()       noexcept requires(!static_value_is_const) { return &value(); }
		};

	template <::utils::storage::type storage_type, typename T, size_t extent, bool sequential_observer>
	struct multiple;

	namespace concepts
		{
		template <typename T>
		concept multiple = std::same_as<T, utils::storage::multiple<T::static_storage_type, typename T::value_type, T::extent, T::static_sequential_observer>>;
		}


	template <::utils::storage::type storage_type, typename T, size_t EXTENT = std::dynamic_extent, bool sequential_observer = true>
	struct multiple
		{
		using value_type = T;
		inline static constexpr ::utils::storage::type static_storage_type{storage_type};
		inline static constexpr bool static_value_is_const{std::is_const_v<value_type>};
		inline static constexpr bool static_sequential_observer{sequential_observer};
		inline static constexpr size_t extent{EXTENT};
		using self_t = multiple<storage_type, T, extent, sequential_observer>;

		template <typename T2>
		using owner_storage_t = std::conditional_t<extent == std::dynamic_extent, std::vector<T2>, std::array<T2, extent>>;

		using inner_storage_t = std::conditional_t
			<
			static_storage_type == ::utils::storage::type::owner,
			owner_storage_t<T>,
			std::conditional_t
			/**/<
			/**/static_sequential_observer,
			/**/std::span<T, extent>,
			/**/owner_storage_t<std::reference_wrapper<value_type>>
			/**/>
			>;

		inner_storage_t storage;

		utils_gpu_available constexpr const auto data() const noexcept { return storage.data(); }
		utils_gpu_available constexpr       auto data()       noexcept { return storage.data(); }

		utils_gpu_available constexpr const value_type& operator[](size_t index) const noexcept                                  { return static_cast<const value_type&>(storage   [index]); }
		utils_gpu_available constexpr       value_type& operator[](size_t index)       noexcept requires(!static_value_is_const) { return static_cast<      value_type&>(storage   [index]); }
		utils_gpu_available constexpr const value_type& at        (size_t index) const                                           { return static_cast<const value_type&>(storage.at(index)); }
		utils_gpu_available constexpr       value_type& at        (size_t index)                requires(!static_value_is_const) { return static_cast<      value_type&>(storage.at(index)); }
 
		template <typename inner_iterator_t, typename T2>
		struct base_iterator
			{
			using self_t            = base_iterator<inner_iterator_t, T2>;
			using iterator_category = std::random_access_iterator_tag;
			using difference_type   = std::ptrdiff_t;
			using value_type        = T2;
			inline static constexpr bool static_value_is_const{std::is_const_v<value_type>};
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

		using iterator               = base_iterator<typename inner_storage_t::iterator              ,       value_type>;
		using const_iterator         = base_iterator<typename inner_storage_t::const_iterator        , const value_type>;
		using reverse_iterator       = base_iterator<typename inner_storage_t::reverse_iterator      ,       value_type>;
		using const_reverse_iterator = base_iterator<typename inner_storage_t::const_reverse_iterator, const value_type>;
		
		utils_gpu_available constexpr       iterator           begin()       noexcept { return {storage. begin()}; }
		utils_gpu_available constexpr const_iterator           begin() const noexcept { return {storage. begin()}; }
		utils_gpu_available constexpr const_iterator          cbegin()       noexcept { return {storage.cbegin()}; }
		utils_gpu_available constexpr       iterator           end  ()       noexcept { return {storage. end  ()}; }
		utils_gpu_available constexpr const_iterator           end  () const noexcept { return {storage. end  ()}; }
		utils_gpu_available constexpr const_iterator          cend  ()       noexcept { return {storage.cend  ()}; }
			 
		utils_gpu_available constexpr       reverse_iterator  rbegin()       noexcept { return {storage. begin()}; }
		utils_gpu_available constexpr const_reverse_iterator  rbegin() const noexcept { return {storage. begin()}; }
		utils_gpu_available constexpr const_reverse_iterator crbegin()       noexcept { return {storage.cbegin()}; }
		utils_gpu_available constexpr       reverse_iterator  rend  ()       noexcept { return {storage. end  ()}; }
		utils_gpu_available constexpr const_reverse_iterator  rend  () const noexcept { return {storage. end  ()}; }
		utils_gpu_available constexpr const_reverse_iterator crend  ()       noexcept { return {storage.cend  ()}; }

		utils_gpu_available constexpr multiple(inner_storage_t&& storage) : storage{storage} {}

		template <concepts::can_construct_value_type<value_type> ...Args>
		utils_gpu_available constexpr multiple(Args&&... args) requires(concepts::vector<inner_storage_t> || (concepts::array<inner_storage_t> && sizeof...(Args) == extent)) :
			storage{std::forward<Args>(args)...} {}
		
		template <concepts::multiple other_t>
		utils_gpu_available constexpr multiple(const other_t& other)
			requires(storage_type == utils::storage::type::observer && std::is_const_v<value_type>) :
			storage{utils::storage::create::make_const_references<inner_storage_t>(other.storage)}
			{}
		
		float c{1.f};

		template <concepts::multiple other_t>
		utils_gpu_available constexpr multiple(other_t& other)
			requires(storage_type == utils::storage::type::observer && !std::is_const_v<value_type> && !std::is_const_v<typename other_t::value_type>) :
			storage{utils::storage::create::make_references<inner_storage_t>(other.storage)}
			{}

		void f() {}
		};

	namespace owner
		{
		template <typename T>
		using single = ::utils::storage::single<::utils::storage::type::owner, T>;
		template <typename T, size_t extent = std::dynamic_extent>
		using multiple = ::utils::storage::multiple<::utils::storage::type::owner, T, extent, true>;
		}
	namespace observer
		{
		template <typename T>
		using single = ::utils::storage::single<::utils::storage::type::observer, T>;
		template <typename T, size_t extent = std::dynamic_extent, bool sequential = true>
		using multiple = ::utils::storage::multiple<::utils::storage::type::observer, T, extent, sequential>;
		}

	//template <typename T>
	//inline static consteval ::utils::storage::type get_type()
	//	{
	//	if constexpr (concepts::const_reference<T>) { return ::utils::storage::type::const_observer; }
	//	else if constexpr (concepts::reference<T>) { return ::utils::storage::type::observer; }
	//	else return ::utils::storage::type::owner;
	//	}
	}