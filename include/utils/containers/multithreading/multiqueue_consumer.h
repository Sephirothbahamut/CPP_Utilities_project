#pragma once

#include <thread>
#include <functional>
#include <condition_variable>

#include "consumption_delegating_queue.h"
#include "operations_flags.h"

namespace utils::containers::multithreading
	{
	template <typename T, flags<operation_flag_bits> operations>
	class consumption_delegating_queue;

	class multiqueue_consumer
		{
		template <typename T, flags<operation_flag_bits> operations>
		friend class consumption_delegating_queue;
		

		struct queue_actions_t
			{
			std::function<void()> swap_and_consume;
			std::function<void()> consume_producer;
			};

		public:
			multiqueue_consumer() : thread{ &multiqueue_consumer::consumer, this } {}
			
			multiqueue_consumer(const std::function<void()>& pre_consumption, const std::function<void()>& post_consumption) : 
				pre_consumption  { pre_consumption  },
				post_consumption { post_consumption },
				thread           { &multiqueue_consumer::consumer, this } {}
			
			~multiqueue_consumer() { inner_flush(); }

			void flush()
				{
				inner_flush();
				running = true;
				thread = std::thread{&multiqueue_consumer::consumer, this};
				}

			template <typename T, flags<operation_flag_bits> npcq_operations>
			void bind(consumption_delegating_queue<T, npcq_operations>& npcq)
				{
				std::unique_lock lock{mutex};
				actions.emplace_back([&]() { npcq.swap_and_consume(); }, [&]() { npcq.consume_producer(); });
				npcq.consumer = this;
				}


		private:
			std::function<void()> pre_consumption {[](){}};
			std::function<void()> post_consumption{[](){}};


			void inner_flush()
				{
				running = false;
				work_available.notify_one();
				thread.join();

				pre_consumption();
				for (const auto& action : actions) { action.consume_producer(); }
				post_consumption();
				}

			std::condition_variable work_available;
			std::atomic_bool running{true};
			std::mutex mutex;

			std::thread thread;

			void consumer() noexcept
				{
				while (running)
					{
					std::unique_lock lock{mutex};
					work_available.wait(lock);

					pre_consumption ();
					for (const auto& action : actions) { action.swap_and_consume(); }
					post_consumption();
					}
				}

			std::vector<queue_actions_t> actions;
		};
	}
