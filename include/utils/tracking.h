#pragma once
#include <memory>
#include <stdexcept>

namespace utils
	{
	template <typename T, typename Trakcer_type>
	class tracking_ptr;
	template <typename T>
	class trackable_wrapper;

	namespace _implementation_details
		{
		// A pointer to an identified object. This object lives in the heap and is used to share information with all identifiers about the object moving in memory.
		template <typename T>
		class tracker
			{
			public:
				tracker() = default;
				tracker(T* identified) noexcept : identified{identified} {}

				tracker(const tracker& copy) = delete;
				tracker& operator=(const tracker& copy) = delete;

				tracker(tracker&& move) = delete;
				tracker& operator=(tracker&& move) = delete;

				T* identified{nullptr};
			};
		}

	class trackable
		{
		template <typename, typename>
		friend class tracking_ptr;
		public:
			trackable() : inner_identifier{std::make_shared<_implementation_details::tracker<trackable>>(this)} {}

			trackable(const trackable& copy) : inner_identifier{std::make_shared<_implementation_details::tracker<trackable>>(this)} {}
			trackable& operator=(const trackable& copy) { return *this; }

			trackable(trackable&& move) noexcept : inner_identifier{std::move(move.inner_identifier)} { inner_identifier->identified = this; }
			trackable& operator=(trackable&& move) noexcept { inner_identifier = std::move(move.inner_identifier); inner_identifier->identified = this; return *this; }
			~trackable() { if (inner_identifier) { inner_identifier->identified = nullptr; } }

		private:
			std::shared_ptr<_implementation_details::tracker<trackable>> inner_identifier;
		};

	// Defines an instance of T which must be identified; that object will be tied to a dynamically allocated _implementation_details::tracker. Whenever the object is moved, the _implementation_details::tracker is updated. When the object is delted, the _implementation_details::tracker's value is set to null, so other still living Identifiers will know that the object is dead.
	template <typename T>
	class trackable_wrapper
		{
		friend class tracking_ptr<T, T>;
		public:
			template <typename ...Args>
			trackable_wrapper(Args&&... args) : object{std::forward<Args>(args)...}, inner_identifier{std::make_shared<_implementation_details::tracker<T>>(&object)} {}

			trackable_wrapper(trackable_wrapper& copy) : trackable_wrapper{static_cast<const trackable_wrapper&>(copy)} {}

			trackable_wrapper(const trackable_wrapper& copy) : object{copy.object}, inner_identifier{std::make_shared<_implementation_details::tracker<T>>(&object)} {}
			trackable_wrapper& operator=(const trackable_wrapper& copy) { object = copy.object; return *this; } //Note: no need to reassign the pointer, already points to current instance

			trackable_wrapper(trackable_wrapper&& move) noexcept : object{std::move(move.object)}, inner_identifier{std::move(move.inner_identifier)} { inner_identifier->identified = &object; }
			trackable_wrapper& operator=(trackable_wrapper&& move) noexcept { object = std::move(move.object); inner_identifier = std::move(move.inner_identifier); inner_identifier->identified = &object; return *this; }

			~trackable_wrapper() { if (inner_identifier) { inner_identifier->identified = nullptr; } }

			const T& operator* () const { return *get(); }
				  T& operator* ()       { return *get(); }
			const T* operator->() const { return  get(); }
				  T* operator->()       { return  get(); }

			const T* get() const
				{
#ifndef NDEBUG
				if (!inner_identifier || inner_identifier->identified == nullptr) { throw std::runtime_error{"Attempting to retrive object from an identifier which identified object had already been destroyed."}; }
#endif
				return &object;
				}

			T* get()
				{
#ifndef NDEBUG
				if (!inner_identifier || inner_identifier->identified == nullptr) { throw std::runtime_error{"Attempting to retrive object from an identifier which identified object had already been destroyed."}; }
#endif
				return &object;
				}

			T object;

			operator T& () { return object; }
			//explicit operator tracking_ptr<T>() const { return {object}; }
		private:
			std::shared_ptr<_implementation_details::tracker<T>> inner_identifier;
		};

	template <typename T, typename Tracker_type = trackable>
	class tracking_ptr
		{
		public:
			tracking_ptr() = default;
			tracking_ptr(Tracker_type& identified) : inner_identifier{identified.inner_identifier} {}
			tracking_ptr& operator=(Tracker_type& identified) { inner_identifier = identified.inner_identifier; return *this; }

			tracking_ptr(const tracking_ptr& copy) = default;
			tracking_ptr& operator=(const tracking_ptr& copy) = default;

			tracking_ptr(tracking_ptr&& move) = default;
			tracking_ptr& operator=(tracking_ptr&& move) = default;


			const T& operator* () const { check_all(); return static_cast<T&>(*inner_identifier->identified); }
			T& operator* () { check_all(); return static_cast<T&>(*inner_identifier->identified); }
			const T* operator->() const { check_all(); return  static_cast<T*>(inner_identifier->identified); }
			T* operator->() { check_all(); return  static_cast<T*>(inner_identifier->identified); }

			const T* get() const { check_initialized(); return static_cast<T*>(inner_identifier->identified); }
			T* get() { check_initialized(); return static_cast<T*>(inner_identifier->identified); }

			bool has_value() const noexcept { return inner_identifier && inner_identifier->identified != nullptr; }
			explicit operator bool() const noexcept { return has_value(); }

		private:
			std::shared_ptr<_implementation_details::tracker<Tracker_type>> inner_identifier{nullptr};

			void check_initialized() const
				{
#ifndef NDEBUG
				if (!inner_identifier) { throw std::runtime_error{"Trying to use an uninitialized Identifier."}; }
#endif
				}

			void check_has_value() const
				{
#ifndef NDEBUG
				if (inner_identifier->identified == nullptr) { throw std::runtime_error{"Trying to retrive object from an identifier which identified object had already been destroyed."}; }
#endif
				}

			void check_all() const { check_initialized(); check_has_value(); }
		};
	}