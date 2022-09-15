#pragma once
#include <vector>
#include <deque>
#include <list>
#include <set>
#include <concepts>
#include "../variadic.h"

namespace utils::containers
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
		protected:
			using tuple_t = std::tuple<Container_type<Types>...>;
		public:
			template <typename ...Args>
			multitype_container(Args&&... args) : containers { std::forward<Args>(args)... } {}

			// TODO standard compliant iterators


			template <typename msg_t, typename ...Args>
			msg_t& emplace(Args ...args)
				{
				if constexpr (utils::variadic::contains_type<msg_t, Types...>::value)
					{
					return container_emplace_helper<Container_type, msg_t>::emplace(get_containing_type<msg_t>(), std::forward<Args>(args)...);
					}
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

			template <typename msg_t, typename Function>
			void for_each_element_of_type(Function function)
				{
				auto& container{ get_containing_type<msg_t>() };
				for (auto& element : container) { function(element); }
				}

			size_t size() 
				{
				size_t s = 0;
				utils::tuple::for_each_in_tuple(containers, [&](auto& container) { s += container.size(); });
				return s;
				}

		protected:
			tuple_t containers;

			template<typename T, std::size_t index = 0>
			static constexpr std::size_t get_index_containing_type()
				{
				if constexpr (std::same_as<T, typename std::tuple_element<index, tuple_t>::type::value_type>) { return index; }
				else { return get_index_containing_type<T, index + 1>(); }
				}
		};

	}