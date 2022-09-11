#pragma once

#include <vector>
#include <stdexcept>
#include <algorithm>
#include <unordered_set>
#include <memory>
#include "../../../../include/utils/polymorphic_value.h"

namespace utils
	{

	template <typename T>
	class enable_disable_vector;

	namespace _implementation_details
		{
		class enable_disable_generic
			{
			template <typename>
			friend class enable_disable_vector;

			public:
				enum class Action { unchanged, disable, enable, destroy };
				virtual void destroy() = 0;
				virtual void disable() = 0;
				virtual void enable() = 0;

			protected:
				bool active = true;
				size_t index = 0;
				Action action = Action::unchanged;
			};

		template <typename T>
		class enable_disable_specific : public enable_disable_generic
			{
			friend class enable_disable_vector<T>;

			public:
				enable_disable_specific(enable_disable_vector<T>& owner) : owner(&owner) {}//TODO turn constructor private and set friend to std::make_unique

				virtual void destroy() override
					{
					if (action == Action::disable) { owner->to_disable.erase(index); }
					else if (action == Action::enable) { owner->to_enable.erase(index); }

					if (action != Action::destroy) { owner->to_destroy.push_back(index); action = Action::destroy; }
					}
				virtual void disable() override
					{
					if (active)
						{
						if (action == Action::disable || action == Action::destroy) { return; }
						owner->to_disable.insert(index); action = Action::disable;
						}
					}
				virtual void enable() override
					{
					if (!active)
						{
						if (action == Action::enable || action == Action::destroy) { return; }
						owner->to_enable.insert(index); action = Action::enable;
						}
					}

			private:
				enable_disable_vector<T>* owner = nullptr;
			};
		}

	class enable_disable
		{
		template <typename>
		friend class enable_disable_vector;

		public:

			void destroy()
				{
#ifndef NDEBUG
				if (!ptr) { throw std::runtime_error("Trying to destroy object that was added to an enable_disable_vector before a single step passed."); }
#endif
				ptr->destroy();
				}
			void enable()
				{
#ifndef NDEBUG
				if (!ptr) { throw std::runtime_error("Trying to enable object that was added to an enable_disable_vector before a single step passed."); }
#endif
				ptr->enable();
				}
			void disable()
				{
#ifndef NDEBUG
				if (!ptr) { throw std::runtime_error("Trying to disable object that was added to an enable_disable_vector before a single step passed."); }
#endif
				ptr->disable();
				}


		private:
			void set(utils::polymorphic_value<_implementation_details::enable_disable_generic>& endis) { ptr = std::move(endis); }
			const _implementation_details::enable_disable_generic* operator->() const { return &*ptr; }
			_implementation_details::enable_disable_generic* operator->() { return &*ptr; }

			utils::polymorphic_value<_implementation_details::enable_disable_generic> ptr;
		};

	template <typename T>
	class enable_disable_vector
		{
		friend class _implementation_details::enable_disable_specific<T>;
		public:
			using value_type = T;
			using get_endis_signature = enable_disable & (*)(T& element);

			enable_disable_vector(get_endis_signature get_endis) : get_endis(get_endis) {}

			typename std::vector<T>::iterator begin() { return objects.begin() + first_active; }
			typename std::vector<T>::iterator end() { return objects.begin() + objects.size(); }
			typename std::vector<T>::iterator rbegin() { return objects.rbegin(); }
			typename std::vector<T>::iterator rend() { return objects.rend() - first_active; }

			template <typename ...Args>
			T& emplace(Args&&... args) { return new_active_objects.emplace_back(std::forward<Args>(args)...); }

			template <typename ...Args>
			T& emplace_inactive(Args&&... args) { return new_inactive_objects.emplace_back(std::forward<Args>(args)...); }

			void update()
				{
				update_destroy();
				update_enable_disable();
				update_new();
				}

			size_t size() { return end() - begin(); }

		private:
			std::vector<T> objects;
			std::vector<T> new_active_objects;
			std::vector<T> new_inactive_objects;

			size_t first_active = 0;

			std::unordered_set<size_t> to_disable;
			std::unordered_set<size_t> to_enable;
			std::vector       <size_t> to_destroy;

			get_endis_signature  get_endis;

			void update_destroy()
				{
				std::sort(to_destroy.begin(), to_destroy.end(), std::greater<>());    //descending
				size_t total_size = objects.size();

				for (size_t destroy : to_destroy)
					{
					if (destroy >= first_active) //we're destroying an active object, only need to swap with end.
						{
						if (destroy < total_size - 1)
							{
							std::swap(objects[destroy], objects[total_size - 1]);

							//update indices
							auto it = to_disable.find(total_size - 1);
							if (it != to_disable.end()) { to_disable.erase(it); to_disable.insert(destroy); }
							get_endis(objects[destroy])->index = destroy;
							}
						total_size--;
						}
					else //we're destroying a disabled object, need to move the active size index too and performa double swap
						{
						if (first_active == total_size)
							{
							if (destroy < total_size - 1)
								{
								std::swap(objects[destroy], objects[total_size - 1]);

								//update indices
								auto it = to_enable.find(total_size - 1);
								if (it != to_enable.end()) { to_enable.erase(it); to_enable.insert(destroy); }
								get_endis(objects[destroy])->index = destroy;
								}
							}
						else
							{
							if (destroy < first_active - 1)
								{
								std::swap(objects[destroy], objects[first_active - 1]);

								//update indices
								auto it = to_enable.find(first_active - 1);
								if (it != to_enable.end()) { to_enable.erase(it); to_enable.insert(destroy); }
								get_endis(objects[destroy])->index = destroy;
								destroy = first_active - 1;
								}

							std::swap(objects[destroy], objects[total_size - 1]);

							//update indices
							auto it = to_disable.find(total_size - 1);
							if (it != to_disable.end()) { to_disable.erase(it); to_disable.insert(destroy); }
							get_endis(objects[destroy])->index = destroy;
							}
						total_size--; first_active--;
						}
					}
				to_destroy.clear();

				objects.erase(objects.begin() + total_size, objects.end());
				}

			void update_enable_disable()
				{
				auto disable_it = to_disable.begin();
				auto enable_it = to_enable.begin();

				for (; disable_it != to_disable.end() && enable_it != to_enable.end(); disable_it++, enable_it++)
					{
					size_t disable = *disable_it;
					size_t enable = *enable_it;
					std::swap(objects[disable], objects[enable]);

					//update indices
					get_endis(objects[disable])->index = disable;
					get_endis(objects[enable])->index = enable;

					get_endis(objects[disable])->active = true; //the disable index is the position in   active elements where the object that was enabled  has now been moved
					get_endis(objects[enable])->active = false;//the enable  index is the position in inactive elements where the object that was disabled has now been moved
					}
				if (to_disable.size() > to_enable.size())
					{
					for (; disable_it != to_disable.end(); disable_it++)
						{
						size_t disable = *disable_it;
						if (disable != first_active)
							{
							std::swap(objects[disable], objects[first_active]);

							//update indices
							get_endis(objects[first_active])->index = first_active;
							get_endis(objects[disable])->index = disable;
							}

						get_endis(objects[first_active])->active = false;
						first_active++;
						}
					}
				else if (to_enable.size() > to_disable.size())
					{
					for (; enable_it != to_enable.end(); enable_it++)
						{
						size_t enable = *enable_it;

						if (enable != first_active - 1)
							{
							std::swap(objects[enable], objects[first_active - 1]);

							//update indices
							get_endis(objects[first_active - 1])->index = first_active - 1;
							get_endis(objects[enable])->index = enable;
							}

						get_endis(objects[first_active - 1])->active = true;
						first_active--;
						}
					}

				to_enable.clear();
				to_disable.clear();
				}

			void update_new()
				{
				for (auto& object : new_active_objects)
					{
					T& emplaced = objects.emplace_back(std::move(object));
					utils::polymorphic_value<_implementation_details::enable_disable_specific<T>> endis_spec = utils::make_polymorphic_value<_implementation_details::enable_disable_specific<T>>(*this);
					utils::polymorphic_value<_implementation_details::enable_disable_generic> endis{endis_spec};
					get_endis(emplaced).set(endis);
					get_endis(emplaced)->active = true;

					get_endis(objects[objects.size() - 1])->index = objects.size() - 1;
					}

				for (auto& object : new_inactive_objects)
					{
					T& emplaced = objects.emplace_back(std::move(object));
					utils::polymorphic_value<_implementation_details::enable_disable_specific<T>> endis_spec = utils::make_polymorphic_value<_implementation_details::enable_disable_specific<T>>(*this);
					utils::polymorphic_value<_implementation_details::enable_disable_generic> endis{endis_spec};
					get_endis(emplaced).set(endis);
					get_endis(emplaced)->active = false;

					if (first_active != objects.size() - 1)
						{
						get_endis(objects[first_active])->index = objects.size() - 1;
						std::swap(objects[first_active], emplaced);
						}

					get_endis(objects[first_active])->index = first_active;
					first_active++;
					}

				new_active_objects.clear();
				new_inactive_objects.clear();
				}
		};



	}