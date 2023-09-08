#pragma once

#include <thread>
#include <atomic>
#include <functional>
#include <condition_variable>
#include "consumable_queue.h"

namespace utils::containers::multithreading
	{
	template <typename T, flags<operation_flag_bits> operations = flags<operation_flag_bits>{operation_flag_bits::none} >
	class self_consuming_queue : public consumable_queue<T, operations>
		{
		using consumable_queue_t = consumable_queue<T, operations>;
		public:
			using value_type      = consumable_queue_t::value_type     ; 
			using allocator_type  = consumable_queue_t::allocator_type ; 
			using pointer         = consumable_queue_t::pointer        ; 
			using const_pointer   = consumable_queue_t::const_pointer  ; 
			using reference       = consumable_queue_t::reference      ; 
			using const_reference = consumable_queue_t::const_reference; 
			using size_type       = consumable_queue_t::size_type      ; 
			using difference_type = consumable_queue_t::difference_type;


			self_consuming_queue(const std::function<void(            T &)>& consume         ) requires(operations.none()) : consumable_queue_t{consume}, thread{&self_consuming_queue::consumer, this} {}
			
			self_consuming_queue(const std::function<void(            T &)>& consume         ,
				                 const std::function<void(std::vector<T>&)>& pre_consumption ) requires(operations == operation_flag_bits::pre)
                                                                                             : consumable_queue_t{consume, pre_consumption             }, thread{&self_consuming_queue::consumer, this} {}
			
			self_consuming_queue(const std::function<void(            T &)>& consume         ,
				                 const std::function<void(std::vector<T>&)>& post_consumption) requires(operations == operation_flag_bits::post)
                                                                                             : consumable_queue_t{consume, post_consumption            }, thread{&self_consuming_queue::consumer, this} {}
			
			self_consuming_queue(const std::function<void(            T &)>& consume         ,
				                 const std::function<void(std::vector<T>&)>& pre_consumption , 
				                 const std::function<void(std::vector<T>&)>& post_consumption) requires(operations.all())
				                                                                             : consumable_queue_t{consume, pre_consumption, post_consumption}, thread{&self_consuming_queue::consumer, this} {}

			~self_consuming_queue()
				{
				inner_flush();
				}

			template <typename ...Args>
			void emplace(Args&&... args) noexcept
				{
				consumable_queue_t::emplace(std::forward<Args>(args)...);
				work_available.notify_one();
				}

			void push(const value_type& message) noexcept
				{
				consumable_queue_t::push(message);
				work_available.notify_one();
				}

			void flush()
				{
				inner_flush();
				running = true;
				thread = std::thread{ &self_consuming_queue::consumer, this };
				}

		private:

			//Please compiler, this method is NOT static. Wake up!
			void inner_flush()
				{
				running = false;
				work_available.notify_one();
				thread.join();

				consumable_queue_t::consume_producer();
				}

			void consumer() noexcept
				{
				while (running)
					{
					if (true)
						{
						std::unique_lock lock{producer_consumer_queue<T>::queues_access_mutex };
						if (consumable_queue_t::producer_data.empty()) { work_available.wait(lock); }

						consumable_queue_t::consumer_data.clear();
						consumable_queue_t::consumer_data.swap(producer_consumer_queue<T>::producer_data);
						}

					consumable_queue_t::consume_all(producer_consumer_queue<T>::consumer_data);
					}
				}

			std::atomic_bool running{ true };
			std::condition_variable work_available;

			std::thread thread;
		};
	}