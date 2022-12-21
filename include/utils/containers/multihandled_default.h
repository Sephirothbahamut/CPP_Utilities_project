#pragma once
#include "multihandled_unsafe.h"

//TODO write test cases

namespace utils::containers
	{
	/// <summary>
	/// Handles can either refer to an existing value or the default value. Two handles cannot refer to the same value unless it's the default one.
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <typeparam name="inner_size"></typeparam>
	template <typename T, size_t inner_size = 8>
	class multihandled_default : private multihandled_unsafe<T, inner_size>
		{
		protected:
			using parent_container_t = multihandled_unsafe<T, inner_size>;

		public:
			using handle_t           = parent_container_t::handle_t;
			using value_type         = parent_container_t::value_type;
			using size_type          = parent_container_t::size_type;
			using reference          = parent_container_t::reference;
			using const_reference    = parent_container_t::const_reference;
			using pointer            = parent_container_t::pointer;
			using const_pointer      = parent_container_t::const_pointer;


			template <typename ...Args>
			multihandled_default(Args&&... args) : default_value{std::forward<Args>(args)...}
				{
				inner_fake_handle.element_ptr = &default_value;
				handle_default.inner_handle.element_ptr = &inner_fake_handle;
				}

			void clear() noexcept { parent_container_t::clear(); }

			using parent_container_t::emplace;
			using parent_container_t::push;
			using parent_container_t::swap;

			template <typename ...Args>
			inline void emplace_at(handle_t& handle, Args&&... args)
				{
				if (!is_default(handle)) { parent_container_t::erase_content(handle); }
				parent_container_t::emplace_at(handle, std::forward<Args>(args)...);
				}

			inline void push_at(handle_t& handle, const value_type& value)
				{
				if (!is_default(handle)) { parent_container_t::erase_content(handle); }
				parent_container_t::push_at(handle, value);
				}

			void erase(const handle_t& handle) 
				{
				if (!is_default(handle)) { parent_container_t::erase_content(handle); }
				parent_container_t::erase_handle(handle);
				}

			/// <summary>
			/// Remaps an handle into a different value. The previous handle used to reference that value will be nullified.
			/// </summary>
			/// <param name="handle_to_remap"></param>
			/// <param name="handle_toward_target"></param>
			/// <returns></returns>
			void remap_and_clear_target_handle(handle_t& handle_to_remap, handle_t& handle_remap_target) noexcept
				{
				if (!is_default(handle_to_remap)) { parent_container_t::erase_handle(handle_to_remap); }
				parent_container_t::remap(handle_to_remap, handle_remap_target);
				//handle_remap_target.nullify
				}

			void reset_handle(handle_t& handle)
				{
				if (!is_default(handle)) { parent_container_t::erase_content_and_remap(handle, handle_default); }
				}

			handle_t clone_default()
				{
				return parent_container_t::clone(handle_default);
				}
			
			bool is_default(const handle_t& handle) const noexcept 
				{
				//Compares the pointed-at addresses instead of using == because handle_default being a fake handle doesn't exist within the handles container.
				return handle.get() == handle_default.get(); 
				}

		private:
			T default_value;
			utils::containers::hive::next<T>::iterator inner_fake_handle;
			handle_t handle_default;
		};
	}