#pragma once

//https://cpp-tip-of-the-day.blogspot.com/2014/05/building-custom-iterators.html
//https://web.stanford.edu/class/cs107l/handouts/04-Custom-Iterators.pdf

/*
indexed_container
Automatically assigns new elements to an id (and returns that id on insertion).
Ids of removed elements are recycled as soon as possible rather than further extending the internal container.
It is assumed that the constructor of the contained type T takes size_t as first argument, so the object can know its own id
*/

//TODO cout_container and cout_container_fancy

#include <set>
#include <optional>
#include <iostream> //TODO remove when adding exceptions
#include <concepts>

namespace utils
	{
	template <typename T>
	class indexed_container;

	class indexed_element
		{
		template <typename T>
		friend class indexed_container;

		private:
			size_t id;
			inline void set_id(size_t id) { this->id = id; }

		public:
			inline indexed_element(size_t id) : id(id) {}
			inline size_t get_id() const noexcept { return id; }
		};

	template <typename T>
		requires std::is_base_of_v<indexed_element, T>
	class indexed_container
		{
		static_assert(std::is_base_of<indexed_element, T>::value, "Types contained in an indexed_container must inherit from indexed_element.");

		public:
			typedef indexed_iterator<T> iterator;
			friend class indexed_iterator<T>;

		private:
			std::optional<T>* vec;
			size_t max_id = 0;
			size_t reserved = 1;
			std::set<size_t> unused_ids;

			size_t next_id()
				{
				size_t ret;
				if (unused_ids.empty()) { ret = max_id++; }
				else
					{
					ret = *unused_ids.begin();
					unused_ids.erase(ret);
					}
				return ret;
				}

			void validate_id(size_t id) const
				{
				if (exists(id))
					{//TODO throw exception
					std::cerr << "Trying to access invalid id " << id << std::endl;
					exit(0);
					}
				}

			void grow()
				{
				std::optional<T>* new_vec = new std::optional<T>[reserved * 2];
				for (size_t i = 0; i < reserved; i++)
					{
					if (vec[i]) { new_vec[i].swap(vec[i]); }
					}
				reserved *= 2;
				delete[] vec;
				vec = new_vec;
				}

		public:

			class iterator
				{
				private:
					size_t id;
					const indexed_container<T>* container;

				public:
					T& operator*() { return container->at(id); }
					T& operator->() { return container->at(id); }
					indexed_iterator(size_t id, const indexed_container<T>& container) : id(id), container(&container) {}
					indexed_iterator& operator=(const indexed_iterator<T>& rhs)
						{
						id = rhs.id;
						container = rhs.container;
						return *this;
						}
					indexed_iterator& operator++() // pre-increment
						{
						do { id++; }
						while (id < container->max_id && !container->vec[id]);

						return *this;
						}
					indexed_iterator operator++(int)// post-increment
						{
						indexed_iterator<T> it(id, *container);
						++(*this);
						return it;
						}
					indexed_iterator& operator--() // pre-increment
						{
						do { id--; }
						while (!container->vec[id] && id != 0);
						return *this;
						}
					indexed_iterator operator--(int)// post-increment
						{
						indexed_iterator<T> it(id, *container);
						++(*this);
						return it;
						}
					bool operator==(const indexed_iterator& rhs) { return id == rhs.id; }
					bool operator!=(const indexed_iterator& rhs) { return id != rhs.id; }
					bool operator<(const indexed_iterator& rhs) { return id < rhs.id; }
					bool operator>(const indexed_iterator& rhs) { return id > rhs.id; }
					bool operator<=(const indexed_iterator& rhs) { return id <= rhs.id; }
					bool operator>=(const indexed_iterator& rhs) { return id >= rhs.id; }
				};

			indexed_container() { vec = new std::optional<T>[1]; }
			~indexed_container() { delete[] vec; }

			void clear()
				{
				for (size_t i = 0; i < max_id; i++) { vec[i] = std::nullopt; }
				max_id = 0;
				unused_ids = {};
				}
			size_t size() const { return max_id - unused_ids.size(); }
			bool empty() const { return size() == 0; }

			template <typename... Args>
			size_t emplace(Args&&... args)
				{
				size_t id = next_id();
				if (id >= reserved) { grow(); }
				vec[id].emplace(std::forward<Args>(args)...);
				vec[id].value().set_id(id);
				return id;
				}
			template<typename ...Args>
			size_t emplace_at(size_t id, Args&&... args)
				{
				while (id >= reserved) { grow(); }
				if (id > max_id)
					{
					for (size_t i = max_id; i < id; i++) { unused_ids.insert(i); }
					max_id = id + 1;
					}
				else { if (!vec[id]) { unused_ids.erase(id); } }

				vec[id].emplace(std::forward<Args>(args)...);
				vec[id].value().set_id(id);
				return id;
				}

			void erase(size_t id)
				{
				validate_id(id);
				unused_ids.insert(id);
				vec[id] = std::nullopt;
				}

			bool exists(size_t id) const { return id >= max_id || !vec[id]; }
			T& at(size_t id) const
				{
				validate_id(id);
				return vec[id].value();
				}
			T& operator[](size_t id) const
				{
				validate_id(id);
				return vec[id].value();
				}

			iterator begin() const
				{
				size_t id = 0;
				while (!vec[id] && id < max_id) { id++; }
				//indexed_iterator<T> ret(id, this);
				//return ret;
				return indexed_iterator<T>(id, *this);
				}
			iterator end() const
				{
				return indexed_iterator<T>(max_id, *this);
				}
		};

	}