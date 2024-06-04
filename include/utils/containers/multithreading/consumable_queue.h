#pragma once

#include <functional>

#include "producer_consumer_queue.h"
#include "operations_flags.h"

namespace utils::containers::multithreading
	{
	namespace details
		{
		template <typename T> class consume_operation_pre_void { protected: void consume_pre (std::vector<T>&) const noexcept {} };
		template <typename T> class consume_operation_post_void{ protected: void consume_post(std::vector<T>&) const noexcept {} };

		template <typename T>
		class consume_operation_current
			{
			protected:
				consume_operation_current() = delete;
				consume_operation_current(const std::function<void(T&)>& consume) : consume{consume} {}

				std::function<void(T&)> consume;

				void consume_current(std::vector<T>& elements) { for (auto& element : elements) { consume(element); } }
			};

		template <typename T>
		class consume_operation_pre
			{
			protected:
				consume_operation_pre() = delete;
				consume_operation_pre(const std::function<void(std::vector<T>&)>& pre_consumption) : pre_consumption{pre_consumption} {}

				std::function<void(std::vector<T>&)> pre_consumption;

				void consume_pre(std::vector<T>& elements) { pre_consumption(elements); }
			};
		template <typename T>
		class consume_operation_post
			{
			protected:
				consume_operation_post() = delete;
				consume_operation_post(const std::function<void(std::vector<T>&)>& post_consumption) : post_consumption{post_consumption} {}

				std::function<void(std::vector<T>&)> post_consumption;

				void consume_post(std::vector<T>& elements) { post_consumption(elements); }
			};

		template <typename T, flags<operation_flag_bits> operations> using get_consume_operation_pre     = std::conditional_t<operations.test(operation_flag_bits::pre ), consume_operation_pre    <T>, consume_operation_pre_void    <T>>;
		template <typename T, flags<operation_flag_bits> operations> using get_consume_operation_post    = std::conditional_t<operations.test(operation_flag_bits::post), consume_operation_post   <T>, consume_operation_post_void   <T>>;
		}

	template <typename T, flags<operation_flag_bits> operations = flags<operation_flag_bits>{operation_flag_bits::none}>
	class consumable_queue : 
		public producer_consumer_queue<T>, 
		details::consume_operation_current<T>, 
		details::get_consume_operation_pre <T, operations>, 
		details::get_consume_operation_post<T, operations>
		{
		using current_t = details::consume_operation_current <T>;
		using pre_t     = details::get_consume_operation_pre <T, operations>;
		using post_t    = details::get_consume_operation_post<T, operations>;

		public:
			using value_type      = producer_consumer_queue<T>::value_type     ; 
			using allocator_type  = producer_consumer_queue<T>::allocator_type ; 
			using pointer         = producer_consumer_queue<T>::pointer        ; 
			using const_pointer   = producer_consumer_queue<T>::const_pointer  ; 
			using reference       = producer_consumer_queue<T>::reference      ; 
			using const_reference = producer_consumer_queue<T>::const_reference; 
			using size_type       = producer_consumer_queue<T>::size_type      ; 
			using difference_type = producer_consumer_queue<T>::difference_type;

			consumable_queue(const std::function<void(            T &)>& consume         ) requires(operations.none()) : current_t {consume} {}
			
			consumable_queue(const std::function<void(            T &)>& consume         , 
				             const std::function<void(std::vector<T>&)>& pre_consumption ) requires(operations.test(operation_flag_bits::pre))
                                                                                           : current_t {consume         },
				                                                                             pre_t     {pre_consumption } {}
			
			consumable_queue(const std::function<void(            T &)>& consume         , 
				             const std::function<void(std::vector<T>&)>& post_consumption) requires(operations.test(operation_flag_bits::post))
                                                                                           : current_t {consume         },
				                                                                             post_t    {post_consumption} {}
			
			consumable_queue(const std::function<void(            T &)>& consume         , 
				             const std::function<void(std::vector<T>&)>& pre_consumption , 
				             const std::function<void(std::vector<T>&)>& post_consumption) requires(operations.all())
				                                                                           : current_t {consume         },
				                                                                             pre_t     {pre_consumption },
				                                                                             post_t    {post_consumption} {}

			void swap_and_consume() { consume_all(producer_consumer_queue<T>::swap_and_get()); }

			void consume_producer() { consume_all(producer_consumer_queue<T>::producer_data); producer_consumer_queue<T>::producer_data.clear(); }

			void consume_all(std::vector<T>& elements) 
				{
				pre_t    ::consume_pre    (elements);
				current_t::consume_current(elements);
				post_t   ::consume_post   (elements);
				}
		};
	}