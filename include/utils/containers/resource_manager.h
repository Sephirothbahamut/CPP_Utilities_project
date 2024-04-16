#pragma once

#include <string>
#include <functional>
#include <unordered_set>
#include <unordered_map>

#include "object_pool.h"
#include "../memory.h"
#include "../thread_pool.h"
#include "multithreading/producer_consumer_queue.h" //TODO replace with moodyycamel's non-blocking concurrentqueue, if it ever gets an std::optional<T> polling method

namespace utils::containers
	{
	template <typename T, typename IDENTIFIER_T = std::string>
	class resource_manager
		{
		public:
			using value_type = T;
			using identifier_t = IDENTIFIER_T;

		private:
			using inner_handle = utils::observer_ptr<value_type>;

		public:
			class handle
				{
				template <typename T, typename IDENTIFIER_T>
				friend class resource_manager;
				public:
					using value_type        = T;
					using reference         = value_type& ;
					using const_reference   = const reference;
					using pointer           = value_type* ;
					using const_pointer     = const pointer;
					using iterator_category = std::random_access_iterator_tag;
					using difference_type   = ptrdiff_t ;

					      reference operator* ()       noexcept { return **inner_handle_ptr; }
					const_reference operator* () const noexcept { return **inner_handle_ptr; }

					      pointer   operator->()       noexcept { return  *inner_handle_ptr; }
					const_pointer   operator->() const noexcept { return  *inner_handle_ptr; }

					      reference value()       { return operator*(); }
					const_reference value() const { return operator*(); }

					      pointer   get()       noexcept { return operator->(); }
					const_pointer   get() const noexcept { return operator->(); }

					bool has_value() const noexcept { return inner_handle_ptr; }

					bool operator== (const handle& other) const noexcept { return inner_handle_ptr == inner_handle_ptr; }

				private:
					handle(utils::observer_ptr<const inner_handle> inner_handle_ptr) : inner_handle_ptr{inner_handle_ptr} {}
					utils::observer_ptr<const inner_handle> inner_handle_ptr;
				};

			using loading_callable_t = std::function<value_type()>;
			using loaded_callback_t  = std::function<void(value_type&)>;

			resource_manager(utils::thread_pool& thread_pool, loading_callable_t default_constructor) :
				thread_pool{thread_pool}, 
				default_handle{live.emplace(default_constructor()).get()}
				{}

			auto get(const identifier_t& identifier) noexcept
				{
				struct result_t
					{
					handle handle;
					bool is_loading;
					bool has_value() const noexcept { return handle.has_value(); }
					operator bool () const noexcept { return has_value(); }
					};

				auto it{available.find(identifier)};
				if (it == available.end())
					{
					return result_t{nullptr, false};
					}
				return result_t{&it->second, it->second == default_handle};
				}
			handle get_default() noexcept
				{
				return &default_handle;
				}

			/// <summary> If already exist, do nothing </summary>
			handle load(const identifier_t& identifier, loading_callable_t loading_callable) noexcept
				{
				auto result{available.emplace(identifier, default_handle)};
				if (result.second) //insertion happened
					{
					add_loading_task(identifier, loading_callable);
					}
				return {&result.first->second};
				}
			/// <summary> If already exist, load anyway </summary>
			handle reload(const identifier_t& identifier, loading_callable_t loading_callable) noexcept
				{
				auto result{available.emplace(identifier, default_handle)};
				add_loading_task(identifier, loading_callable);
				return {&result.first->second};
				}

			void flush() noexcept
				{
				auto newly_loaded{loaded.swap_and_get()};
				for (auto& element : newly_loaded)
					{
					inner_handle inner_handle{live.emplace(std::move(element.value)).get()};
					auto it{available.find(element.identifier)};

					if (it == available.end()) { throw std::runtime_error("no"); } //theorically technically theoretically unreachable

					it->second = inner_handle;
					}
				}


		private:
			using object_pool_t = utils::containers::object_pool<value_type, 8Ui64, utils::containers::object_pool_handle_version::raw>;
			object_pool_t live;
			inner_handle default_handle;
			std::unordered_map<identifier_t, inner_handle> available;
			
			struct loaded_t { identifier_t identifier; value_type value; };

			utils::containers::multithreading::producer_consumer_queue<loaded_t> loaded;
			std::reference_wrapper<utils::thread_pool> thread_pool;

			void execute_loading_task(const identifier_t& identifier, loading_callable_t loading_callable) 
				{
				auto result{loading_callable()};
				loaded.emplace(identifier, result);
				}

			void add_loading_task(const identifier_t& identifier, loading_callable_t loading_callable)
				{
				thread_pool.get().push_task([this, identifier, loading_callable]() { execute_loading_task(identifier, loading_callable); });
				}
		};
	}