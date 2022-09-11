#pragma once
#include <deque>

namespace utils
	{
	template <typename T> class deferred_wrapper;

	template <typename T>
	class deferred_deleter
		{
		friend class deferred_wrapper<T>;
		public:
			void flush() { objects.clear(); }
			size_t size() { return objects.size(); }
			bool empty() { return !size(); }
		private:
			void push(T&& element) { objects.push_back(std::move(element)); }
			std::deque<T> objects;
		};

	template <typename T>
	class deferred_wrapper
		{
		public:
			template <typename ...Args>
			deferred_wrapper(deferred_deleter<T>& deleter, Args&&... args) : element{std::forward<Args>(args)...}, deleter{&deleter} {}

			deferred_wrapper(const deferred_wrapper& copy) = default;
			deferred_wrapper& operator=(const deferred_wrapper& copy) = default;

			deferred_wrapper(deferred_wrapper&& move) noexcept : element{std::move(move.element)}, deleter{move.deleter} { move.deleter = nullptr; }
			deferred_wrapper& operator=(deferred_wrapper&& move) noexcept { element = std::move(move.element); deleter = move.deleter; move.deleter = nullptr; return *this; }

			~deferred_wrapper() { if (deleter) { deleter->push(std::move(element)); } }

			const T& operator* () const { return *get(); }
			      T& operator* () { return *get(); }
			const T* operator->() const { return  get(); }
			      T* operator->() { return  get(); }

			const T* get() const { return &element; }

			T* get() { return &element; }

			T element;

			operator T& () { return element; }

		private:
			deferred_deleter<T>* deleter;
		};

	}