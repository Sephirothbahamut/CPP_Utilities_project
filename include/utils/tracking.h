#pragma once
#include <memory>
#include <stdexcept>
#include <concepts>

#include "wrapper.h"
#include "compilation/debug.h"

//TODO create dynamic pointer only once the object starts being tracked
//TODO alternative with diamond wrapper once the todo in wrapper.h is done

namespace utils
	{
	class trackable;
	template <typename T>
	class tracking_ptr;
	template <typename T>
	class trackable_wrapper;

	namespace details
		{
		template <typename T>
		class trackable_base;

		// A pointer to a tracked object. This object lives in the heap and is used to share information with all identifiers about the object moving in memory.
		template <typename T>
		class tracker_t
			{
			template <typename>
			friend class tracking_ptr;
			template <typename>
			friend class trackable_base;

			public:
				tracker_t() = default;
				tracker_t(T* tracked) noexcept : tracked{tracked} {}

				tracker_t(const tracker_t& copy) = delete;
				tracker_t& operator=(const tracker_t& copy) = delete;

				tracker_t(tracker_t&& move) = delete;
				tracker_t& operator=(tracker_t&& move) = delete;

				T* tracked{nullptr};
			};

		template <typename T>
		class trackable_base
			{
			template <typename>
			friend class tracking_ptr;

			public:
				trackable_base(T* ptr) noexcept : tracker{std::make_shared<tracker_t<T>>(ptr)} {}

				trackable_base(const trackable_base& copy) = delete;
				trackable_base& operator=(const trackable_base& copy) = delete;
				trackable_base(trackable_base&& move) noexcept = delete;			  //specialized move needs tracked object's address
				trackable_base& operator=(trackable_base&& move) noexcept = delete; //specialized move needs tracked object's address

				trackable_base(trackable_base&& move, T* my_address) noexcept
					//I take the move source's tracker; tracking_ptr that were tracking my source will now track me.
					: tracker{std::move(move.tracker)}
					{
					//Source needs a new tracker pointing at its address, which is still stored in the tracker I stole
					move.tracker = std::make_shared<tracker_t<T>>(tracker->tracked);

					//Update my tracker with my address. tracking_ptr following the target of the source now point at me, where the source has moved.
					tracker->tracked = my_address;
					}
				trackable_base& move(trackable_base&& move, T* my_address) noexcept
					{
					//I don't exist anymore; my tracker is nullified; tracking_ptr that were tracking me will now point to nullptr, because something else has been moved in my place
					tracker->tracked = nullptr;

					//I take the move source's tracker; tracking_ptr that were tracking my source will now track me.
					tracker = std::move(move.tracker);

					//Source needs a new tracker pointing at its address, which is still stored in the tracker I stole
					move.tracker = std::make_shared<tracker_t<T>>(tracker->tracked);

					//Update my tracker with my address. tracking_ptr following the target of the source now point at me, where the source has moved.
					tracker->tracked = my_address;
					return *this;
					}

				~trackable_base() noexcept { if (tracker) { update_identified(nullptr); } }

			protected:
				void update_identified(T* ptr) noexcept { tracker->tracked = ptr; }
				std::shared_ptr<tracker_t<T>> tracker;
			};
		}

	// The inheritance way
	// The tracked object's address is always this.
	class trackable : public details::trackable_base<trackable>
		{
		using tup = details::trackable_base<trackable>;
		public:
			trackable() noexcept : tup{this} {} //Create a new tracker for myself

			trackable(const trackable& copy) noexcept : tup{this} {} //Create a new tracker for myself
			trackable& operator=(const trackable& copy) noexcept { return *this; } //No need to create a new tracker; it already exists and points to my current self.

			trackable(trackable&& move) noexcept : tup{std::move(move), this} {}
			trackable& operator=(trackable&& move) noexcept { tup::move(std::move(move), this); return *this; }
		};

	// The wrapper way
	template <typename T>
	class trackable_wrapper : public details::trackable_base<T>, public wrapper<T>
		{
		using tup = details::trackable_base<T>;
		public:
			using wrapper<T>::value_type;

			template <typename ...Args>
			trackable_wrapper(Args&&... args) : wrapper<T>{std::forward<Args>(args)...}, tup{std::addressof(wrapper<T>::element)} {}

			//Note: copy constructors may throw for wrapper.
			trackable_wrapper(trackable_wrapper& copy) : trackable_wrapper{static_cast<const trackable_wrapper&>(copy)} {}

			trackable_wrapper(const trackable_wrapper& copy) : wrapper<T>{copy}, tup{std::addressof(wrapper<T>::element)} {}
			trackable_wrapper& operator=(const trackable_wrapper& copy) { wrapper<T>::operator=(copy); return *this; }

			trackable_wrapper(trackable_wrapper&& move) noexcept
				: wrapper<T>{std::move(move.element)}, tup{std::move(move), std::addressof(wrapper<T>::element)} {}
			trackable_wrapper& operator=(trackable_wrapper&& move) noexcept
				{
				wrapper<T>::operator=(std::move(move));
				tup::move(std::move(move), std::addressof(wrapper<T>::element));
				return *this;
				}
		};

	namespace concepts
		{
		template <typename T>
		concept trackable_wrapper = std::derived_from<T, utils::trackable>;
		template <typename T>
		concept trackable_via_inheritance = std::derived_from<T, utils::trackable> && !trackable_wrapper<T>;

		template <typename T>
		concept trackable = trackable_wrapper<T> || trackable_via_inheritance<T>;
		}

	template <typename T>
	class tracking_ptr
		{
		public:
			inline static constexpr bool inheritance{concepts::trackable_via_inheritance<T>};
			inline static constexpr bool wrapped{!inheritance};
			using value_type   = T;
			using tracker_type = std::conditional<wrapped, trackable_wrapper<T>, T/*trackable*/>;

			tracking_ptr() = default;
			
			//for some reason doesn't work, need to specify both constructors without the using
			//tracking_ptr           (tracker_type& tracked) : tracker  {tracked.tracker} {}
			//tracking_ptr& operator=(tracker_type& tracked) { tracker = tracked.tracker; return *this; }
			tracking_ptr           (                  T & tracked) requires(inheritance) : tracker  {tracked.tracker} {}
			tracking_ptr& operator=(                  T & tracked) requires(inheritance) { tracker = tracked.tracker; return *this; }
			tracking_ptr           (trackable_wrapper<T>& tracked) requires(wrapped    ) : tracker  {tracked.tracker} {}
			tracking_ptr& operator=(trackable_wrapper<T>& tracked) requires(wrapped    ) { tracker = tracked.tracker; return *this; }

			tracking_ptr(const tracking_ptr& copy) = default;
			tracking_ptr& operator=(const tracking_ptr& copy) = default;

			tracking_ptr(tracking_ptr&& move) = default;
			tracking_ptr& operator=(tracking_ptr&& move) = default;


			const value_type& operator* () const noexcept { if constexpr (compilation::debug) { check_all(); } return static_cast<value_type>(*tracker->tracked); }
			      value_type& operator* ()       noexcept { if constexpr (compilation::debug) { check_all(); } return static_cast<value_type>(*tracker->tracked); }
			const value_type* operator->() const noexcept { if constexpr (compilation::debug) { check_all(); } return static_cast<value_type>( tracker->tracked); }
			      value_type* operator->()       noexcept { if constexpr (compilation::debug) { check_all(); } return static_cast<value_type>( tracker->tracked); }

			const T* get() const { check_initialized(); return static_cast<T*>(tracker->tracked); }
			      T* get()       { check_initialized(); return static_cast<T*>(tracker->tracked); }

			operator bool()         const noexcept { return is_initialized() && has_value(); }
			bool is_initialized()   const noexcept { return tracker.get(); }
			bool has_value()        const noexcept { if constexpr (compilation::debug) { return has_value_except(); } return tracker->tracked; }
			bool has_value_except() const { check_initialized(); return tracker->tracked; }

		private:
			std::shared_ptr<details::tracker_t<tracker_type>> tracker{nullptr};

			void check_initialized() const
				{
				if (!tracker) { throw std::runtime_error{"Trying to use an uninitialized Identifier."}; }
				}

			void check_has_value() const
				{
				if (tracker->tracked == nullptr) { throw std::runtime_error{"Trying to retrive object from an identifier which tracked object had already been destroyed."}; }
				}

			void check_all() const { check_initialized(); check_has_value(); }
		};
	}
