#pragma once

//CHANGED		cambiati dall'ultima volta
//TOTEST		confrontare performances tra versioni different
//TODO			To do

#include <array>
#include <vector>
#include <list>
#include <optional>
#include <stdexcept>
#include <ostream>
#include <algorithm>
#include <iterator>

#include "../cout_utilities.h"

namespace utils
	{
	//declare output methods
	template <typename T, size_t S> class listed_array;

	namespace cout
		{
		template <typename T, size_t S>
		std::ostream& operator<<(std::ostream& os, const listed_array<T, S>& la) { return la.out(os); };
		}

	template <typename T, size_t internal_capacity>
	class listed_array
		{
		using coords_t = std::pair<size_t, size_t>;
		using storage_t = std::aligned_storage_t<sizeof(T), alignof(T)>;
		using arr_t = std::array<storage_t, internal_capacity>;
		using ptr_t = std::unique_ptr<arr_t>;

		public:
			/*class iterator
				{
				private:
					coords_t coords;
					const listed_array& la;

				public:
					iterator(const listed_array& la, size_t index) : la(la), coords(la.get_coords(index)) {}
					iterator(const listed_array& la, coords_t coords) : la(la), coords(coords) {}
					iterator(const iterator& oth) : coords(oth.coords), la(oth.la) {}
					iterator(iterator&& oth) : coords(oth.coords), la(oth.la) {}
					iterator& operator=(const iterator& oth) { coords = oth.coords; return *this; }
					iterator& operator=(iterator&& oth) { coords = oth.coords; return *this; }

					iterator& operator++() //pre
						{
						//coords.first += (++coords.second) / internal_capacity;
						//coords.second %= internal_capacity;
						if (coords.second == internal_capacity - 1)
							{
							coords.second = 0;
							coords.first++;
							}
						else { coords.second++; }
						return *this;
						}
					iterator operator++(int) // post
						{
						coords_t prev = coords;
						++(*this);
						return { la, prev };
						}
					iterator& operator--() // pre
						{
						if (coords.second == 0)
							{
							coords.second = internal_capacity - 1;
							coords.first--;
							}
						else { coords.second--; }
						return *this;
						}
					iterator operator--(int) // post
						{
						coords_t prev = coords;
						--(*this);
						return { la, prev };
						}
					iterator operator()(const listed_array& la) { return { la, coords }; }

					T& operator*() const { return la.get_val(coords); }
					T& operator->() const { return la.get_val(coords); }
					bool operator==(const iterator& oth) const { return coords == oth.coords; }
					bool operator!=(const iterator& oth) const { return coords != oth.coords; }
				};*/
			class iterator
				{
				private:
					const listed_array& la;

				public:
					size_t index;
					iterator(const listed_array& la, size_t index) : la(la), index(index) {}
					iterator(const iterator& oth) : index(oth.index), la(oth.la) {}
					iterator(iterator&& oth) : index(oth.index), la(oth.la) {}
					iterator& operator=(const iterator& oth) { index = oth.index; return *this; }
					iterator& operator=(iterator&& oth) { index = oth.index; return *this; }

					iterator& operator++() { index++; return *this; }
					iterator operator++(int) { size_t prev = index++; return { la, prev }; }
					iterator& operator--() { index--; return *this; }
					iterator operator--(int) { size_t prev = index--; return { la, prev }; }
					iterator operator()(const listed_array& la) { return { la, index }; }

					T& operator*() const { return la.get_val(index); }
					T& operator->() const { return la.get_val(index); }
					bool operator==(const iterator& oth) const { return index == oth.index; }
					bool operator!=(const iterator& oth) const { return index != oth.index; }
					bool operator<(const iterator& oth)  const { return index <  oth.index; }
				};
		private:
			std::vector<ptr_t> arrs;
			size_t _size = 0;

			size_t get_arr_index(size_t index) const { return index / internal_capacity; }
			size_t get_internal_index(size_t index) const { return index % internal_capacity; }
			coords_t get_coords(size_t index) const { return{ get_arr_index(index), get_internal_index(index) }; }
			arr_t& get_arr(size_t index) const { return *(arrs[get_arr_index(index)].get()); }

			__forceinline T* cast_ptr(storage_t& storage) const { return reinterpret_cast<T*>(&storage); };
			__forceinline T* get_ptr(size_t index) const { return cast_ptr(get_arr(index)[get_internal_index(index)]); }
			__forceinline T* get_ptr(coords_t coords) const { return cast_ptr(get_arr(coords.first)[coords.second]); }
			__forceinline T* storage_end() const { return get_ptr(size()); }
			__forceinline T& get_val(size_t index) const { return *get_ptr(index); }
			__forceinline T& get_val(coords_t coords) const { return *get_ptr(coords); }

			void grow_to(size_t target) { while (arrs.size() != target) { grow(); } }
			void grow()
				{
				ptr_t tmp = std::make_unique<arr_t>();
				arrs.push_back(std::move(tmp));
				}

			/*void advance_content(size_t index) //CHANGED //TODO
				{
				if (size() >= capacity()) { grow(); }

				if (size() > 0)
					{
					get_arr(size()).push_back(get_val(size() - 1));
					for (size_t i = size() - 1; i > index; i--) { get_val(i) = std::move(get_val(i - 1)); }
					_size++;
					}
				}
			/*
			void recede_content(size_t index)
				{
				if (size() > 0)
					{
					for (size_t i = index; i < size() - 1; i++) { get_val(i) = std::move(get_val(i + 1)); }
					auto tmp = get_arr(size() - 1);
					tmp.erase(tmp.end() - 1);
					_size--;
					}
				}*/
			// ╔═══════════════════════════════════════════════════╦═══════════════════╦═════════════════════════════════════════════╗
			// ╠═══════════════════════════════════════════════════╣      OUTPUT       ╠═════════════════════════════════════════════╣
			// ╚═══════════════════════════════════════════════════╩═══════════════════╩═════════════════════════════════════════════╝
			void out_internal_arr(std::ostream& os, arr_t& arr, size_t until = internal_capacity) const
				{
				namespace ccu = cout::support;
				ccu::brace(os, "[");
				for (size_t i = 0; i < until - 1; i++)
					{
					ccu::value(os, *cast_ptr(arr[i]));
					ccu::separ(os, ", ");
					}
				ccu::value(os, *cast_ptr(arr[until - 1]));
				ccu::brace(os, "]");
				}

			std::ostream& out(std::ostream& os) const
				{
				namespace ccu = cout::support;

				ccu::brace(os, "[");
				if (!empty())
					{
					arr_t& last_arr = get_arr(size() - 1);
					auto arrs_it = arrs.begin();
					while (arrs_it->get() != &last_arr)
						{
						auto& arr = *(arrs_it->get());
						out_internal_arr(os, arr);
						ccu::separ(os, " -> ");
						arrs_it++;
						}
					out_internal_arr(os, last_arr, get_internal_index(size() - 1));
					}
				ccu::brace(os, "]");
				return os;
				}

		public:
			// ╔═══════════════════════════════════════════════════╦═══════════════════╦═════════════════════════════════════════════╗
			// ╠═══════════════════════════════════════════════════╣   CONSTRUCTORS    ╠═════════════════════════════════════════════╣
			// ╚═══════════════════════════════════════════════════╩═══════════════════╩═════════════════════════════════════════════╝
			listed_array() {};
			listed_array(const listed_array& copy) { *this = copy; } //CHANGED
			listed_array(listed_array&& move) { *this = std::move(move); }
			~listed_array() { clear(); }

			listed_array& operator=(const listed_array& copy) //CHANGED
				{
				clear();
				grow_to(copy.arrs.size());
				for (auto it = copy.begin(); it != copy.end(); it++) { *it(*this) = *it; }
				_size = copy.size();
				return *this;
				}

			listed_array& operator=(listed_array&& move) //CHANGED
				{
				arrs = std::move(move.arrs);
				_size = move.size();
				move._size = 0;
				return *this;
				}

			/*
			void copy_for_index(const listed_array& copy) // best with debug
				{
				clear();
				grow_to(copy.arrs.size());
				for (size_t i = 0; i < copy.size(); i++)
					{
					get_val(i) = copy[i];
					}
				_size = copy.size();
				}
			void copy_for_iterator(const listed_array& copy) // best with release
				{
				clear();
				grow_to(copy.arrs.size());
				for (auto it = copy.begin(); it != copy.end(); it++) { *it(*this) = *it; }
				_size = copy.size();
				}
			void copy_for_each(const listed_array& copy) // just don't
				{
				clear();
				grow_to(copy.arrs.size());
				for (T& element : copy) { push_back(element); }
				}
			*/
			// ╔═══════════════════════════════════════════════════╦════════════════════╦════════════════════════════════════════════╗
			// ╠═══════════════════════════════════════════════════╣ ACCESS & ITERATORS ╠════════════════════════════════════════════╣
			// ╚═══════════════════════════════════════════════════╩════════════════════╩════════════════════════════════════════════╝

			T& operator[](size_t index) const
				{
#ifdef _DEBUG
				return at(index);
#else
				return get_val(index);
#endif
				}
			T& at(size_t index) const
				{
				if (index >= size()) { throw std::out_of_range("invalid listed_array subscript"); }
				return get_val(index);
				}

			iterator begin() const { return { *this, 0 }; }
			iterator end() const { return { *this, size() }; }
			/*iterator begin() const { return { *this, {0, 0} }; }
			iterator end() const { return { *this, get_coords(size()) }; }*/

			// ╔═══════════════════════════════════════════════════╦═══════════════════╦═════════════════════════════════════════════╗
			// ╠═══════════════════════════════════════════════════╣     ADDITION      ╠═════════════════════════════════════════════╣
			// ╚═══════════════════════════════════════════════════╩═══════════════════╩═════════════════════════════════════════════╝

			void push_back(const T& value)
				{
				if (size() >= capacity()) { grow(); }
				new (storage_end()) T(value);
				_size++;
				}
			void push_back(T&& value)
				{
				if (size() >= capacity()) { grow(); }
				new (storage_end()) T(std::move(value));
				_size++;
				}
			/*size_t insert(size_t index, const T& value)
				{
				advance_content(index);
				new (get_ptr(index)) T(value);
				return index;
				}*/

			template<typename ... Args>
			void emplace_back(Args&& ... args)
				{
				if (size() >= capacity()) { grow(); }
				new (storage_end()) T(std::forward<Args>(args)...);
				_size++;
				}

			/*template<typename ... Args>
			size_t emplace(size_t index, Args ... args)
				{
				advance_content(index);
				new (get_ptr(index)) T(std::forward<Args>(args)...);
				return index;
				}*/

			// ╔═══════════════════════════════════════════════════╦═══════════════════╦═════════════════════════════════════════════╗
			// ╠═══════════════════════════════════════════════════╣     REMOTION      ╠═════════════════════════════════════════════╣
			// ╚═══════════════════════════════════════════════════╩═══════════════════╩═════════════════════════════════════════════╝
			/*void erase(size_t index)
				{
				if (index == size() - 1)
					{
					get_arr(index).erase(get_internal_index(index));
					_size--;
					}
				else
					{

					}
				}*/

			void clear() //CHANGED //TOTEST
				{
				//TOTEST
				if constexpr (!std::is_trivially_destructible<T>::value)
					{
					//for (size_t i = 0; i < size(); i++) { get_ptr(i)->~T(); }
					//for (T& element : *this) { element.~T(); }
					std::for_each(begin(), end(), [](T& element) { element.~T(); });
					}
				arrs.clear();
				_size = 0;
				}

			// ================================= UTILITY

			bool empty() const { return !size(); }
			size_t size() const { return _size; }

			// ╔═══════════════════════════════════════════════════╦═══════════════════╦═════════════════════════════════════════════╗
			// ╠═══════════════════════════════════════════════════╣ MEMORY MANAGEMENT ╠═════════════════════════════════════════════╣
			// ╚═══════════════════════════════════════════════════╩═══════════════════╩═════════════════════════════════════════════╝

			size_t capacity() const { return arrs.size() * internal_capacity; }
			void reserve(size_t amount)
				{
				if (amount > capacity())
					{
					size_t tmp = get_arr_index(amount);
					grow_to(tmp + 1);
					}
				}
			void shrink_to_fit() //CHANGED
				{
				size_t used_size = get_arr_index(size());
				if (used_size < arrs.size()) { arrs.resize(used_size); }
				}

			// ╔═══════════════════════════════════════════════════╦═══════════════════╦═════════════════════════════════════════════╗
			// ╠═══════════════════════════════════════════════════╣       OTHER       ╠═════════════════════════════════════════════╣
			// ╚═══════════════════════════════════════════════════╩═══════════════════╩═════════════════════════════════════════════╝

			friend std::ostream& cout::operator<<(std::ostream& os, const listed_array<T, internal_capacity>& la);
		};
	}
