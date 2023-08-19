#pragma once

#include <mutex>
#include <vector>
#include <optional>

namespace utils::containers::multithreading
	{
	template <typename T>
	class producer_consumer_queue
		{
		public:
			using value_type      = std::vector<T>::value_type     ; 
			using allocator_type  = std::vector<T>::allocator_type ; 
			using pointer         = std::vector<T>::pointer        ; 
			using const_pointer   = std::vector<T>::const_pointer  ; 
			using reference       = std::vector<T>::reference      ; 
			using const_reference = std::vector<T>::const_reference; 
			using size_type       = std::vector<T>::size_type      ; 
			using difference_type = std::vector<T>::difference_type;

			template <typename ...Args>
			void emplace(Args&&... args) 
				{
				std::unique_lock lock{ queues_access_mutex };
				producer_data.emplace_back(std::forward<Args>(args)...);
				}

			void push(const T& element)
				{
				std::unique_lock lock{ queues_access_mutex };
				producer_data.push_back(element);
				}

			std::vector<T>& swap_and_get()
				{
				consumer_data.clear();
				if (true)
					{
					std::unique_lock lock{queues_access_mutex};
					std::swap(producer_data, consumer_data);
					}
				return consumer_data;
				}

			std::optional<value_type> get()
				{
				if (consumer_data.empty()) { return std::nullopt; }
				auto ret{std::move(consumer_data.back())};
				consumer_data.pop_back();
				return std::move(ret);
				}

		protected:
			std::mutex queues_access_mutex;
			std::vector<T> producer_data;
			std::vector<T> consumer_data;
		};
	}