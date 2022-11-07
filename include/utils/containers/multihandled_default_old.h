#pragma once
#include "multihandled.h"

//TODO write test cases

namespace utils::containers
	{
	template <typename T, size_t inner_size = 8>
	class multihandled_default : public multihandled<T, inner_size>
		{
		protected:
			using parent_container_t = multihandled<T, inner_size>;

		public:
			using handle_t           = parent_container_t::handle_t;
			using value_type         = parent_container_t::value_type;
			using size_type          = parent_container_t::size_type;
			using reference          = parent_container_t::reference;
			using const_reference    = parent_container_t::const_reference;
			using pointer            = parent_container_t::pointer;
			using const_pointer      = parent_container_t::const_pointer;

			template <typename ...Args>
			multihandled_default(Args&&... args) :
				default_handle{ parent_container_t::emplace(std::forward<Args>(args)...)}
				{}

			void reset_handle(handle_t& handle)
				{
				if (parent_container_t::get_inner_iterator(handle) != parent_container_t::get_inner_iterator(default_handle))
					{
					parent_container_t::erase_and_remap(handle, default_handle);
					}
				}

			handle_t get_default()
				{
				return parent_container_t::splice(default_handle);
				}
			
		private:
			handle_t default_handle;
		};

	template <typename T, size_t inner_size = 8>
	using multidefaultoso = multihandled_default<T, inner_size>;
	}