#pragma once

#include <utility>

#include "hive/next.h"

namespace utils::containers
	{
	template <typename T, size_t inner_size>
	class multihandled_default;

	/// <summary>
	/// This container is generally unsafe. Operations on handles do not give strong guarantees. Meant to be used as base to implement safe variations upon.
	/// Erasures and emplaces/pushes must not be concurrent. Dereferencing handles during those operations is safe.
	/// This is true because the hive::next implementation does not touch or rely on used slots during other operations.
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <typeparam name="inner_size"></typeparam>
	template <typename T, size_t inner_size = 8>
	class multihandled_unsafe
		{
		private:
			using inner_container_t   = utils::containers::hive::next<T, inner_size>;
			using handles_container_t = utils::containers::hive::next<typename inner_container_t::handle_raw, inner_size>;

		public:
			class handle_t 
				{
				friend class multihandled_unsafe <T, inner_size>;
				friend class multihandled_default<T, inner_size>;

				public:
					using value_type      = T;
					using reference       = value_type&;
					using const_reference = const value_type&;
					using pointer         = value_type*;
					using const_pointer   = const value_type* const;

					constexpr       reference operator* ()       noexcept { return **inner_handle; }
					constexpr const_reference operator* () const noexcept { return **inner_handle; }

					constexpr       pointer   operator->()       noexcept { return std::addressof(operator*()); }
					constexpr const_pointer   operator->() const noexcept { return std::addressof(operator*()); }

					constexpr       reference value     ()       noexcept { return **inner_handle; }
					constexpr const_reference value     () const noexcept { return **inner_handle; }

					constexpr       pointer   get       ()       noexcept { return std::addressof(operator*()); }
					constexpr const_pointer   get       () const noexcept { return std::addressof(operator*()); }

					bool operator==(const handle_t& other) const noexcept { return inner_handle == other.inner_handle; }



				private:
					handle_t() = default;
					handle_t(handles_container_t::handle_raw inner_handle) : inner_handle{inner_handle} {}

					typename handles_container_t::handle_raw inner_handle;
				};
			friend class handle_t;
			
			using value_type      = inner_container_t::value_type;
			using size_type       = inner_container_t::size_type;
			using reference       = inner_container_t::reference;
			using const_reference = inner_container_t::const_reference;
			using pointer         = inner_container_t::pointer;
			using const_pointer   = inner_container_t::const_pointer;

			void clear() noexcept { inner_container.clear(); handles_container.clear(); }

			template <typename ...Args>
			inline handle_t emplace(Args&&... args)
				{
				auto inner_inner_handle{inner_container.emplace(std::forward<Args>(args)...)};
				return {handles_container.emplace(inner_inner_handle)};
				}

			inline handle_t push(const value_type& value)
				{
				return emplace(value);
				}

			/// <summary>
			/// UNSAFE! Safe if the handle's content was erased
			/// Emplaces new content and redirects an existing handle to it
			/// </summary>
			/// <typeparam name="...Args"></typeparam>
			/// <param name="handle"></param>
			/// <param name="...args"></param>
			template <typename ...Args>
			inline void emplace_at(handle_t& handle, Args&&... args)
				{
				auto inner_inner_handle{inner_container.emplace(std::forward<Args>(args)...)};
				(*handle.inner_handle) = inner_inner_handle;
				}

			/// <summary>
			/// UNSAFE! Safe if the handle's content was erased
			/// Pushes new content and redirects an existing handle to it
			/// </summary>
			/// <typeparam name="...Args"></typeparam>
			/// <param name="handle"></param>
			/// <param name="...args"></param>
			inline void push_at(handle_t& handle, const value_type& value)
				{
				emplace_at(handle, value);
				}

			/// <summary>
			/// UNSAFE!
			/// Only erases the handle. Does not erase the content. Use when you know you have multiple handles to the same content. 
			/// Erasing every handle to a given content will cause a leak.
			/// </summary>
			/// <typeparam name="T"></typeparam>
			void erase_handle(const handle_t& handle) noexcept { handles_container.erase(handle.inner_handle); }

			/// <summary>
			/// UNSAFE!
			/// Only erases the content accessible through the handle. The handle will keep existing and can be remapped.
			/// </summary>
			/// <param name="handle"></param>
			void erase_content(const handle_t& handle) noexcept { inner_container.erase(*(handle.inner_handle)); }

			/// <summary>
			/// UNSAFE! Safe if the second parameter handle is guaranteed to not be dangling.
			/// Erases the content accessible through the handle, then remaps the handle to the content accessible through another.
			/// </summary>
			/// <param name="handle_to_remap"></param>
			/// <param name="handle_remap_target"></param>
			void erase_content_and_remap(handle_t& handle_to_remap, const handle_t& handle_remap_target) noexcept
				{
				erase_content(handle_to_remap);
				remap(handle_to_remap, handle_remap_target);
				}

			/// <summary>
			/// Erases both the handle and the content accessible through the handle. If other handles pointed to that content they're now dangling.
			/// </summary>
			/// <param name="handle"></param>
			void erase(const handle_t& handle) noexcept { erase_content(handle); erase_handle(handle); }

			/// <summary>
			/// Changes the content accessible through an handle to the content accessible through another.
			/// </summary>
			/// <param name="handle_to_remap"></param>
			/// <param name="handle_toward_target"></param>
			/// <returns></returns>
			void remap(handle_t& handle_to_remap, const handle_t& handle_remap_target) noexcept
				{
				*handle_to_remap.inner_handle = *handle_remap_target.inner_handle;
				}

			/// <summary>
			/// Swaps the content accessible between two handles
			/// </summary>
			/// <param name="handle_to_remap"></param>
			/// <param name="handle_toward_target"></param>
			/// <returns></returns>
			void swap(handle_t& a, handle_t& b) noexcept
				{
				std::swap(*(a.inner_handle), *(b.inner_handle));
				}

			/// <summary>
			/// Create a new handle to some existing content
			/// </summary>
			/// <param name="source"></param>
			/// <returns></returns>
			handle_t clone(const handle_t& source) noexcept
				{
				return {handles_container.emplace(*(source.inner_handle))};
				}

		private:
			inner_container_t inner_container;
			handles_container_t handles_container;
		};
	}