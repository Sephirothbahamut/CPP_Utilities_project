#pragma once

#include "../../memory.h"

#include "operations_flags.h"
#include "multiqueue_consumer.h"
#include "consumable_queue.h"

namespace utils::containers::multithreading
	{
	class multiqueue_consumer;

	template <typename T, flags<operation_flag_bits> operations = flags<operation_flag_bits>{operation_flag_bits::none} >
	class consumption_delegating_queue : public consumable_queue<T, operations>
		{
		friend class multiqueue_consumer;
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


			using consumable_queue_t::consumable_queue;

			template <typename ...Args>
			void emplace(Args&&... args) noexcept
				{
				consumable_queue<T>::emplace(std::forward<Args>(args)...);
				consumer->work_available.notify_one();
				}

			void push(const value_type& message) noexcept
				{
				consumable_queue<T>::push(message);
				consumer->work_available.notify_one();
				}

		private:
			utils::observer_ptr<multiqueue_consumer> consumer;
		};
	}