#pragma once
#include "multitype_container.h"
#include "../polymorphic_value.h"
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

	//TODO ensure that all Types inherit from Base_type
	template <template<class> class Container_type, typename Base_type, typename ...Types>
	class polymorphic_container
		{
		public:
			polymorphic_container() = default;

			template <typename Arg_parent, typename ...Args> //TODO can I split the variadic?
			polymorphic_container(Arg_parent&& arg, Args&& ...args)
				: parent_container{std::forward<Arg_parent>(arg)}, set{std::forward<Args>(args)...} {}

			template <typename Type, typename ...Args>
			Type& emplace(Args ...args)
				{
				if constexpr (utils::variadic::contains_type<Type, Types...>::value)
					{
					return container_emplace_helper<Container_type, Type>::emplace(set.get_containing_type<Type>(), std::forward<Args>(args)...);
					}
				else 
					{ 
					return
						*dynamic_cast<Type*>(
							&*container_emplace_helper<Container_type, utils::polymorphic_value<Base_type>>
							::emplace(
								parent_container, 
								utils::make_polymorphic_value<Type>(std::forward<Args>(args)...)));
					}
				}

			template <typename Function>
			void for_each_container(Function function) { function(parent_container); set.for_each_container(function); }
			template <typename Function>
			void for_each_element(Function function)
				{
				for (utils::polymorphic_value<Base_type>& polyval : parent_container) { function(*polyval); }
				set.for_each_element(function);
				}

			template <typename Type, typename Function>
			void for_each_containing_type(Function function) { set.for_each_containing_type<Type>(function); }

			template <typename Type, typename Function>
			void for_each_element_of_type(Function function)
				{
				for (utils::polymorphic_value<Base_type>& element : parent_container)
					{
					Type* element_cast = dynamic_cast<Type*>(&*element);
					if (element_cast != nullptr)
						{
						function(*element_cast);
						}
					}

				set.for_each_element_of_type<Type>(function);
				}

			template <typename Type>
			auto& get()
				{
				if constexpr (utils::variadic::contains_type<Type, Types...>::value) { return set.get_containing_type<Type>(); }
				else { return parent_container; }
				}

			size_t size() { return parent_container.size() + set.size(); }

		private:

			Container_type<utils::polymorphic_value<Base_type>> parent_container;
			utils::multitype_container<Container_type, Types...> set;

		};

	}