#pragma once
#include <vector>
#include <deque>
#include <list>
#include <set>

#include <variant>
#include <tuple>
#include <execution>
#include <algorithm>
#include "../variadic.h"

namespace utils
	{
	template<template<typename> typename Container_type, typename T>
	struct container_emplace_helper
		{};

	template<typename T>
	struct container_emplace_helper<std::vector, T>
		{
		template<typename... Args>
		static T& emplace(std::vector<T>& v, Args&&... args)
			{
			return v.emplace_back(std::forward<Args>(args)...);
			}
		};
	template<typename T>
	struct container_emplace_helper<std::deque, T>
		{
		template<typename... Args>
		static T& emplace(std::deque<T>& v, Args&&... args)
			{
			return v.emplace_back(std::forward<Args>(args)...);
			}
		};
	template<typename T>
	struct container_emplace_helper<std::list, T>
		{
		template<typename... Args>
		static T& emplace(std::list<T>& v, Args&&... args)
			{
			return v.emplace_back(std::forward<Args>(args)...);
			}
		};
	template<typename T>
	struct container_emplace_helper<std::set, T>
		{
		template<typename... Args>
		static T& emplace(std::set<T>& v, Args&&... args)
			{
			return v.emplace(std::forward<Args>(args)...);
			}
		};

	template <template<class> class Container_type, typename ...Types>
	class multitype_container
		{
		public:
			using tuple_t = std::tuple<Container_type<Types>...>;

			tuple_t containers;

			template <typename ...Args>
			multitype_container(Args&&... args) : containers { std::forward<Args>(args)... } {}

			template <typename msg_t, typename ...Args>
			msg_t& emplace(Args ...args)
				{
				static_assert(utils::variadic::contains_type<msg_t, Types...>::value, "Trying to emplace in a multitype_container a non supported type.");

				return container_emplace_helper<Container_type, msg_t>::emplace(get_containing_type<msg_t>(), std::forward<Args>(args)...);
				}

			template <typename msg_t>
			auto& get_containing_type()
				{
				return std::get<get_index_containing_type<msg_t>()>(containers);
				}

			template <typename Function>
			void for_each_container(Function function) { utils::tuple::for_each_in_tuple(containers, [&](auto& container) { function(container); }); }
			template <typename Function>
			void for_each_element(Function function) { for_each_container([&](auto& container) { for (auto& element : container) { function(element); } }); }


			template <typename execution_policy, typename Function>
			void for_each_element(execution_policy policy, Function function) 
				{
				for_each_container([&](auto& container)
					{
					std::for_each(policy, container.begin(), container.end(), function);
					});
				}

			template <typename msg_t, typename Function>
			void for_each_element_of_type(Function function)
				{
				for_each_containing_type<msg_t>([&](auto& container) { for (auto& element : container) { function(element); } });
				}

			template <typename msg_t, typename execution_policy, typename Function>
			void for_each_element_of_type(execution_policy policy, Function function)
				{
				for_each_containing_type<msg_t>([&](auto& container)
					{
					std::for_each(policy, container.begin(), container.end(), function);
					});
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


			/*// TODO standard compliant iterators
			template <typename T>
			class iterator
				{
				public:
					iterator(multitype_container& container) : 
						container{&container}, 
						inner_container_index{get_index_containing_type_or_derived<T>()},
						current_iterator{container.containers.get<inner_container_index ??? >().begin()},
						current_end     {container.containers.get<inner_container_index ??? >().end()}
						{
						
						}

					iterator<T, typename Types...> operator++()
						{
						
						}
				private:
					multitype_container* container;
					size_t inner_container_index;
					std::variant<container_type<Types...>::iterator> current_iterator;
					std::variant<container_type<Types...>::iterator> current_end;
				};

			template <typename T>
			iterator<T> begin() { return iterator(*this); }*/


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