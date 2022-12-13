#pragma once
#include "polymorphic_access_container.h"
#include "../polymorphic_value.h"
#include "../variadic.h"

namespace utils::containers
	{
	//TODO ensure that all Types inherit from Base_type
	//TODO test cases
	template <template<class> class Container_type, typename Base_type, typename ...Types>
	class polymorphic_container
		{
		public:
			polymorphic_container() = default;

			template <typename Arg_parent, typename ...Args>
			polymorphic_container(Arg_parent&& arg, Args&& ...args)
				: parent_container{std::forward<Arg_parent>(arg)}, set{std::forward<Args>(args)...} {}

			template <typename msg_t, typename ...Args>
			msg_t& emplace(Args ...args)
				{
				if constexpr (utils::variadic::contains_type<msg_t, Types...>::value)
					{
					return container_emplace_helper<Container_type, msg_t>::emplace(set.get_containing_type<msg_t>(), std::forward<Args>(args)...);
					}
				else 
					{ 
					return
						*dynamic_cast<msg_t*>(
							&*container_emplace_helper<Container_type, utils::polymorphic_value<Base_type>>
							::emplace(
								parent_container, 
								utils::make_polymorphic_value<msg_t>(std::forward<Args>(args)...)));
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

			template <typename msg_t, typename Function>
			void for_each_containing_type(Function function) { set.for_each_containing_type<msg_t>(function); }

			template <typename msg_t, typename Function>
			void for_each_element_of_type(Function function)
				{
				for (utils::polymorphic_value<Base_type>& element : parent_container)
					{
					msg_t* element_cast = dynamic_cast<msg_t*>(&*element);
					if (element_cast != nullptr)
						{
						function(*element_cast);
						}
					}

				set.for_each_element_of_type<msg_t>(function);
				}

			template <typename msg_t>
			auto& get()
				{
				if constexpr (utils::variadic::contains_type<msg_t, Types...>::value) { return set.get_containing_type<msg_t>(); }
				else { return parent_container; }
				}

			size_t size() { return parent_container.size() + set.size(); }

		private:

			Container_type<utils::polymorphic_value<Base_type>> parent_container;
			utils::containers::polymorphic_access_container<Container_type, Types...> set;

		};

	}