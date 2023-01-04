#pragma once

#include <utility>
#include <functional>
#include <unordered_map>

#include "object_pool.h"
#include "../memory.h"
#include "../compilation/debug.h"

namespace utils::containers
	{
	template <typename T, typename key_T = std::string>
	requires std::movable<T>
	class resource_manager
		{
		using objects_pool = object_pool<T, 8Ui64, false, false, true>;

		struct handles_pool_value_t
			{
			typename objects_pool::handle_shared object_handle;
			std::function<T()> factory;

			void reload() { object_handle.value() = std::move(factory()); }
			};

		using handles_pool = object_pool<handles_pool_value_t, 8Ui64, false, false, true>;

		public:
			using value_type        = T;
			using reference         = value_type& ;
			using const_reference   = const reference;
			using pointer           = value_type* ;
			using const_pointer     = const pointer;
			using iterator_category = std::random_access_iterator_tag;
			using difference_type   = ptrdiff_t ;
			
			resource_manager(std::function<T()> default_resource_factory) :
				default_object_handle{objects.make_shared(std::move(default_resource_factory()))},
				default_handle       {handles.make_shared(default_object_handle                )}
				{}

			class handle
				{
				friend class resource_manager;
				public:
					using value_type        = resource_manager::value_type       ;
					using reference         = resource_manager::reference        ;
					using const_reference   = resource_manager::const_reference  ;
					using pointer           = resource_manager::pointer          ;
					using const_pointer     = resource_manager::const_pointer    ;
					using iterator_category = resource_manager::iterator_category;
					using difference_type   = resource_manager::difference_type  ;
					
						  reference operator* ()       noexcept { return handle_to_object_handle->object_handle.operator* (); }
					const_reference operator* () const noexcept { return handle_to_object_handle->object_handle.operator* (); }
						  pointer   operator->()       noexcept { return handle_to_object_handle->object_handle.operator->(); }
					const_pointer   operator->() const noexcept { return handle_to_object_handle->object_handle.operator->(); }
						  reference value     ()                { return handle_to_object_handle->object_handle.value     (); }
					const_reference value     () const          { return handle_to_object_handle->object_handle.value     (); }
						  pointer   get       ()       noexcept { return handle_to_object_handle->object_handle.get       (); }
					const_pointer   get       () const noexcept { return handle_to_object_handle->object_handle.get       (); }

					void reload() { handle_to_object_handle->reload(); }

				private:
					handle(handles_pool::handle_shared handle_to_object_handle, resource_manager& resource_manager) : 
						handle_to_object_handle{handle_to_object_handle},
						resource_manager_ptr   {&resource_manager}
						{}

					handles_pool::handle_shared           handle_to_object_handle;
					utils::observer_ptr<resource_manager> resource_manager_ptr;
				};

			handle load(const key_T& key, std::function<T()> factory)
				{
				auto it{map.find(key)};
				if (it == map.end())
					{
					auto handle_to_object_handle{handles.make_shared(objects.make_shared(std::move(factory())), factory)};
					map[key] = handle_to_object_handle;
					return {handle_to_object_handle, *this};
					}
				else 
					{
					return {it->second, *this};
					}
				}

			handle reload(const key_T& key)
				{
				auto it{map.find(key)};

				if constexpr (utils::compilation::debug)
					{
					if (it == map.end())
						{
						throw std::logic_error{"Attempting to reload a resource that was never loaded."};
						}
					}

				it->second->reload();
				}

		private:
			objects_pool objects;
			handles_pool handles;

			std::unordered_map<key_T, handles_pool::handle_shared> map;

			handles_pool::handle_shared default_object_handle;
			handles_pool::handle_shared default_handle;
		};
	}