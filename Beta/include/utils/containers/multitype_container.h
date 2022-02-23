#pragma once
#include <utils/variadic.h>

namespace utils
	{
	template <template<class> class Container_type, typename ...Types>
	class multitype_container
		{
		public:
			template <typename ...Args>
			multitype_container(Args&&... args) : containers { std::forward<Args>(args)... } {}

			using tuple_t = std::tuple<Container_type<Types>...>;

			tuple_t containers;

			// TODO standard compliant iterators

			template <typename msg_t>
			auto& get_containing_type()
				{
				return std::get<get_index_containing_type<msg_t>()>(containers);
				}

			template <typename Function>
			void for_each_container(Function function) { utils::tuple::for_each_in_tuple(containers, [&](auto& container) { function(container); }); }
			template <typename Function>
			void for_each_element(Function function) { for_each_container([&](auto& container) { for (auto& element : container) { function(element); } }); }

			template <typename msg_t, typename Function>
			void for_each_element_of_type(Function function)
				{
				for_each_containing_type<msg_t>([&](auto& container) { for (auto& element : container) { function(element); } });
				}

			template <typename msg_t, typename Function>
			void for_each_containing_type(Function function)
				{
				utils::tuple::for_each_in_tuple(containers, [&](auto& container)
					{
					if constexpr (std::is_base_of_v<msg_t, typename std::remove_reference_t<decltype(container)>::value_type>) { function(container); }
					});
				}

			size_t size() 
				{
				size_t s = 0;
				utils::tuple::for_each_in_tuple(containers, [&](auto& container) { s += container.size(); });
				return s;
				}

		private:

			template<typename T, std::size_t index = 0>
			static constexpr std::size_t get_index_containing_type()
				{
				if constexpr (std::is_same_v<T, typename std::tuple_element<index, tuple_t>::type::value_type>) { return index; }
				else { return get_index_containing_type<T, index + 1>(); }
				}
			template<typename T, std::size_t index = 0>
			static constexpr std::size_t get_index_containing_type_or_derived()
				{
				if constexpr (std::is_base_of<T, typename std::tuple_element<index, tuple_t>::type::value_type>) { return index; }
				else { return get_index_containing_type_or_derived<T, index + 1>(); }
				}
		};

	}