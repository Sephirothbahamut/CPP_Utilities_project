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
			void push(T&& object) { objects.push_back(std::move(object)); }
			std::deque<T> objects;
		};

	template <typename T>
	class deferred_wrapper
		{
		public:
			template <typename ...Args>
			deferred_wrapper(deferred_deleter<T>& deleter, Args&&... args) : object{std::forward<Args>(args)...}, deleter{&deleter} {}

			deferred_wrapper(const deferred_wrapper& copy) = default;
			deferred_wrapper& operator=(const deferred_wrapper& copy) = default;

			deferred_wrapper(deferred_wrapper&& move) noexcept : object{std::move(move.object)}, deleter{move.deleter} { move.deleter = nullptr; }
			deferred_wrapper& operator=(deferred_wrapper&& move) noexcept { object = std::move(move.object); deleter = move.deleter; move.deleter = nullptr; return *this; }

			~deferred_wrapper() { if (deleter) { deleter->push(std::move(object)); } }

			const T& operator* () const { return *get(); }
			      T& operator* () { return *get(); }
			const T* operator->() const { return  get(); }
			      T* operator->() { return  get(); }

			const T* get() const { return &object; }

			T* get() { return &object; }

			T object;

			operator T& () { return object; }

		private:
			deferred_deleter<T>* deleter;
		};

	}