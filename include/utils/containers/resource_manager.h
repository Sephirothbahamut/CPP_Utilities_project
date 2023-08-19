#pragma once

//#include <utility>
//#include <functional>
//#include <unordered_map>
//
//#include "object_pool.h"
//#include "../memory.h"
//#include "../compilation/debug.h"
//
//namespace utils::containers
//	{
//	template <typename T, typename key_T = std::string>
//	requires std::movable<T>
//	class resource_manager
//		{
//		using objects_pool = object_pool<T, 8Ui64, false, false, true>;
//	
//		struct handles_pool_value_t
//			{
//			typename objects_pool::handle_shared object_handle;
//			std::function<T()> factory;
//	
//			void reload() { object_handle.value() = std::move(factory()); }
//			};
//	
//		using handles_pool = object_pool<handles_pool_value_t, 8Ui64, false, false, true>;
//	
//		public:
//			using value_type        = T;
//			using reference         = value_type& ;
//			using const_reference   = const reference;
//			using pointer           = value_type* ;
//			using const_pointer     = const pointer;
//			using iterator_category = std::random_access_iterator_tag;
//			using difference_type   = ptrdiff_t ;
//			
//			resource_manager(std::function<T()> default_resource_factory) :
//				default_object_handle{objects.make_shared(std::move(default_resource_factory()))},
//				default_handle       {handles.make_shared(default_object_handle                )}
//				{}
//	
//			class handle
//				{
//				friend class resource_manager;
//				public:
//					using value_type        = resource_manager::value_type       ;
//					using reference         = resource_manager::reference        ;
//					using const_reference   = resource_manager::const_reference  ;
//					using pointer           = resource_manager::pointer          ;
//					using const_pointer     = resource_manager::const_pointer    ;
//					using iterator_category = resource_manager::iterator_category;
//					using difference_type   = resource_manager::difference_type  ;
//					
//						  reference operator* ()       noexcept { return handle_to_object_handle->object_handle.operator* (); }
//					const_reference operator* () const noexcept { return handle_to_object_handle->object_handle.operator* (); }
//						  pointer   operator->()       noexcept { return handle_to_object_handle->object_handle.operator->(); }
//					const_pointer   operator->() const noexcept { return handle_to_object_handle->object_handle.operator->(); }
//						  reference value     ()                { return handle_to_object_handle->object_handle.value     (); }
//					const_reference value     () const          { return handle_to_object_handle->object_handle.value     (); }
//						  pointer   get       ()       noexcept { return handle_to_object_handle->object_handle.get       (); }
//					const_pointer   get       () const noexcept { return handle_to_object_handle->object_handle.get       (); }
//	
//					void reload() { handle_to_object_handle->reload(); }
//	
//				private:
//					handle(handles_pool::handle_shared handle_to_object_handle, resource_manager& resource_manager) : 
//						handle_to_object_handle{handle_to_object_handle},
//						resource_manager_ptr   {&resource_manager}
//						{}
//	
//					handles_pool::handle_shared           handle_to_object_handle;
//					utils::observer_ptr<resource_manager> resource_manager_ptr;
//				};
//	
//			handle load(const key_T& key, std::function<T()> factory)
//				{
//				auto it{map.find(key)};
//				if (it == map.end())
//					{
//					auto handle_to_object_handle{handles.make_shared(objects.make_shared(std::move(factory())), factory)};
//					map[key] = handle_to_object_handle;
//					return {handle_to_object_handle, *this};
//					}
//				else 
//					{
//					return {it->second, *this};
//					}
//				}
//	
//			handle reload(const key_T& key)
//				{
//				auto it{map.find(key)};
//	
//				if constexpr (utils::compilation::debug)
//					{
//					if (it == map.end())
//						{
//						throw std::logic_error{"Attempting to reload a resource that was never loaded."};
//						}
//					}
//	
//				it->second->reload();
//				}
//	
//		private:
//			objects_pool objects;
//			handles_pool handles;
//	
//			std::unordered_map<key_T, handles_pool::handle_shared> map;
//	
//			handles_pool::handle_shared default_object_handle;
//			handles_pool::handle_shared default_handle;
//		};
//	}

#include <string>
#include <thread>
#include <expected>
#include <functional>
#include <unordered_set>
#include <unordered_map>

#include "../memory.h"
#include "../thread_pool.h"
#include "multithreading/producer_consumer_queue.h" //TODO replace with moodyycamel's non-blocking concurrentqueue, if it ever gets an std::optional<T> polling method

namespace utils::containers
	{
	struct loading_error
		{
		bool is_loading;
		};

	template <typename T, typename IDENTIFIER_T = std::string>
	class resource_manager
		{
		public:
			using value_type   = T;
			using identifier_t = IDENTIFIER_T;
			using loading_result_t   = std::expected<value_type, loading_error>;
			using loading_callable_t = std::function<loading_result_t()>;
			using loaded_callback_t  = std::function<void(value_type&)>;

			resource_manager(utils::thread_pool& thread_pool) : thread_pool{thread_pool} {}

			loading_result_t get(const identifier_t& identifier) noexcept
				{
				auto it{available.find(identifier)};
				if (it == available.end())
					{
					return std::unexpected(loading_error{.is_loading{loading.contains(identifier)}});
					}
				return it->second;
				}

			loading_result_t load(const identifier_t& identifier, loading_callable_t loading_callable) noexcept
				{
				auto ret{get(identifier)};
				if (ret.has_value() || ret.error().is_loading) { return ret; }
				else
					{
					loading.emplace(identifier);
					add_loading_task(identifier, loading_callable);
					return std::unexpected(loading_error{.is_loading{true}});
					}
				}

			void flush() noexcept
				{
				if (true)
					{
					auto newly_loaded{loaded.swap_and_get()};
					for (auto& element : newly_loaded)
						{
						auto emplace_result{available.emplace(std::move(element.identifier), std::move(element.value))};
						auto& map_pair{*emplace_result.first};
						auto& emplaced_value{map_pair.second};

						auto& callbacks{loading[element.identifier]};
						for (const loaded_callback_t& callback : callbacks)
							{
							callback(emplaced_value);
							}
						loading.erase(element.identifier);
						}
					}
				}


		private:
			std::unordered_map<identifier_t, value_type> available;
			
			struct loaded_t { identifier_t identifier; value_type value; };

			utils::containers::multithreading::producer_consumer_queue<loaded_t> loaded;
			//std::unordered_map<identifier_t, value_type> loaded;
			std::unordered_map<identifier_t, std::vector<loaded_callback_t>> loading;
			std::reference_wrapper<utils::thread_pool> thread_pool;

			void execute_loading_task(const identifier_t& identifier, loading_callable_t loading_callable) 
				{
				auto result{loading_callable()};
				if (result.has_value())
					{
					loaded.enqueue({identifier, result.value()});
					}
				else if (result.error().is_loading)
					{
					add_loading_task(identifier, loading_callable);
					}
				}

			void add_loading_task(const identifier_t& identifier, loading_callable_t loading_callable)
				{
				thread_pool.get().push_task([this, identifier, loading_callable]() { execute_loading_task(identifier, loading_callable); });
				}
		};
	}

struct type_a
	{
	type_a(std::string filename) : n{static_cast<int>(filename[0])} {}
	int n;
	};
struct type_b
	{
	type_b(const type_a& a) : n{a.n} {}
	int n;
	};


int main()
	{
	utils::thread_pool thread_pool{8};
	utils::containers::resource_manager<type_a> resource_manager_a{thread_pool};
	utils::containers::resource_manager<type_b> resource_manager_b{thread_pool};


	while (true)
		{
		auto result{resource_manager_b.load("rty", [&resource_manager_a]() -> utils::containers::resource_manager<type_b>::loading_result_t
			{
			auto result{resource_manager_a.load("asd", []() 
				{
				std::cout << "asd being constructed\n";
				type_a ret{"asd"};
				std::cout << "asd construction completed\n";
				return ret;
				})};

			if (!result.has_value()) 
				{
				if (result.error().is_loading)
					{
					std::cout << "rty loading attempt caused asd to start loading\n";
					}
				return result; 
				}

			std::cout << "rty being constructed\n";
			type_b ret{result.value()};
			std::cout << "rty construction completed\n";
			return ret;
			})};

		resource_manager_a.flush();
		resource_manager_b.flush();
		
		if (result.has_value())
			{
			std::cout << "main loop, rty is loaded\n";
			}
		else if(result.error().is_loading)
			{
			std::cout << "main loop, rty is loading\n";
			}

		if (resource_manager_a.get("qwe").has_value() &&
			resource_manager_b.get("rty").has_value())
			{
			break;
			}
		}

	std::cout << "test done!\n";
	}