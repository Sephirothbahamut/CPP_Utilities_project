#pragma once

#include <array>
#include <bitset>
#include <memory>
#include <concepts>
#include <stdexcept>
#include <type_traits>

#include "../memory.h"
#include "../flags.h"
#include "../compilation/debug.h"

//TODO finish tests
//TODO const, reverse and reverse const iterators

namespace utils::containers
	{
	enum class object_pool_handle_version : uint8_t
		{
		raw    = 0b00000001,
		unique = 0b00000010,
		shared = 0b00000100
		};
	}

namespace utils::containers::details
	{
	template
		<
		typename T,
		size_t segment_size = 8,
		flags<object_pool_handle_version> HANDLE_VERSION_FLAGS = flags<object_pool_handle_version>::full(),
		std::unsigned_integral refcount_value_T = uint8_t,
		typename Allocator = std::allocator<T>
		>
	class object_pool_details //templated namespace, do not instantiate
		{
		public:
			using value_type        = T;
			using reference         = value_type& ;
			using const_reference   = const reference;
			using pointer           = value_type* ;
			using const_pointer     = const pointer;
			using iterator_category = std::random_access_iterator_tag;
			using difference_type   = ptrdiff_t ;

			inline static constexpr flags<object_pool_handle_version> handle_version_flags = HANDLE_VERSION_FLAGS;
			inline static constexpr const bool enabled_raw   {handle_version_flags.test(object_pool_handle_version::raw   )};
			inline static constexpr const bool enabled_unique{handle_version_flags.test(object_pool_handle_version::unique)};
			inline static constexpr const bool enabled_shared{handle_version_flags.test(object_pool_handle_version::shared)};

			class first_segment_t;

		private:
			// Shared handles imply a refcount array. That array is used to keep track of unique handles through the special value numeric_limits<recount_value_type>::max()
			// In absence of shared handles:
			// raw    handles alone imply complete absence of refcount and unique bitsets
			// unique handles alone imply complete absence of refcount and unique bitsets (if an element exists, an unique handle to it must exist somewhere in the program)
			// raw and unique together require unique bitset to keep track of which elements are owned and which aren't
			inline static constexpr const bool use_refcount      = enabled_shared;
			inline static constexpr const bool use_unique_bitset = enabled_raw && enabled_unique && !enabled_shared;

			using refcount_value_type = std::conditional_t<use_refcount, refcount_value_T, std::conditional_t<use_unique_bitset, typename std::bitset<segment_size>::reference, void>>;

			struct segment_t;
			union slot_t;
			
			struct handle_bare
				{
				utils::observer_ptr<segment_t> segment_ptr{nullptr};
				utils::observer_ptr<slot_t   > slot_ptr   {nullptr};
				};
			
			// not a variant, information about which field is active is held in a bitset, achieving better cache friendliness when iterating on densely occupied slots.
			union slot_t
				{
				T element;
				handle_bare free_slot_handle;
				 slot_t() {}
				~slot_t() {}
				};
			
#pragma region refcount containers definitions
			template <typename refcount_value_type>
			class refcount_base
				{
				public:
					refcount_value_T& operator[](size_t index) { return arr[index]; }
				private:
					std::array<refcount_value_type, segment_size> arr;
				};
			
			template <>
			class refcount_base<typename std::bitset<segment_size>::reference>
				{
				public:
					typename std::bitset<segment_size>::reference& operator[](size_t index) { return arr[index]; }
				private:
					std::bitset<segment_size> arr;
				};
			
			template <>
			class refcount_base<void> {};
			
			using refcount_t = refcount_base<refcount_value_type>;
#pragma endregion refcount containers definitions
			
			// Each segment keeps "segment_size" count of:
			// sequentially stored slots for elements
			// bitset representing occupied slots
			// refcount if needed
			// has the information to manage new/delete on individual slot's element field

			struct segment_t
				{
				segment_t(utils::observer_ptr<segment_t> prev_segment = nullptr) : prev_segment{prev_segment}
					{
					for (size_t i = 0; i < segment_size - 1; i++)
						{
						arr[i].free_slot_handle = {this, std::addressof(arr[i + 1])};
						if constexpr (use_refcount || use_unique_bitset) { refcount[i] = 0; }
						}
					arr[segment_size - 1].free_slot_handle = {this, nullptr};
					}
				~segment_t() { clear(); }
				void clear() 
					{
					for (size_t i = 0; i < segment_size; i++)
						{
						if (used_bitset[i]) { arr[i].element.~T(); }
						}
					}
			
				std::bitset<segment_size> used_bitset;
				refcount_t refcount;
			
				utils::observer_ptr<segment_t> prev_segment{nullptr};
				std::array<slot_t, segment_size> arr;
				utils::unique_deepcopy_ptr<segment_t> next_segment{nullptr};
			
				template <typename ...Args>
				inline void emplace(utils::observer_ptr<slot_t> slot_ptr, Args&&... args)
					{
					auto bitset_index{slot_ptr - std::addressof(arr[0])};
					if constexpr (utils::compilation::debug)
						{
						if (used_bitset[bitset_index])
							{
							throw std::runtime_error{"Attempting to emplace element on non-empty slot"};
							}
						}
			
					new(std::addressof(slot_ptr->element)) T{std::forward<Args>(args)...}; //may throw
					used_bitset[bitset_index] = true;
					}

				inline auto& get_refcount(utils::observer_ptr<slot_t> slot_ptr) noexcept
					requires(use_refcount) //TODO merge condition with the next function
					{
					auto refcount_index{slot_ptr - std::addressof(arr[0])};
					return refcount[refcount_index];
					}
				inline refcount_value_type get_refcount(utils::observer_ptr<slot_t> slot_ptr) noexcept
					requires(use_unique_bitset)
					{
					auto refcount_index{slot_ptr - std::addressof(arr[0])};
					return refcount[refcount_index];
					}
			
				inline handle_bare erase(utils::observer_ptr<slot_t> slot_ptr, handle_bare free_slot_handle)
					{
					auto bitset_index{slot_ptr - std::addressof(arr[0])};
					if constexpr (utils::compilation::debug)
						{
						if (!used_bitset[bitset_index])
							{
							throw std::runtime_error{"Attempting to erase element from empty slot"};
							}
						}
			
					used_bitset[bitset_index] = false;
					slot_ptr->element.~T();
					slot_ptr->free_slot_handle = free_slot_handle;
					return {this, slot_ptr};
					}
				};

			class handle_base : protected handle_bare
				{
				friend class first_segment_t;
				public:
					/// <summary> Undefined behaviour if has_value() returns false. </summary><returns> A reference to the referred object. </returns>
						  reference operator* ()       noexcept { enforce_has_value_debug(); return handle_bare::slot_ptr->element; }
					/// <summary> Undefined behaviour if has_value() returns false. </summary><returns> A reference to the referred object. </returns>
					const_reference operator* () const noexcept { enforce_has_value_debug(); return handle_bare::slot_ptr->element; }
							
					/// <summary> Undefined behaviour if has_value() returns false. </summary><returns> A pointer to the referred object. </returns>
						  pointer   operator->()       noexcept { enforce_has_value_debug(); return std::addressof(operator*()); }
					/// <summary> Undefined behaviour if has_value() returns false. </summary><returns> A pointer to the referred object. </returns>
					const_pointer   operator->() const noexcept { enforce_has_value_debug(); return std::addressof(operator*()); }

					/// <summary> Throws if has_value() returns false. </summary><returns> A reference to the referred object. </returns>
						  reference value     ()                { has_value_or_throw(); return operator*(); }
					/// <summary> Throws if has_value() returns false. </summary><returns> A reference to the referred object. </returns>
					const_reference value     () const          { has_value_or_throw(); return operator*(); }
							
					/// <summary> Undefined behaviour if has_value() returns false.</summary><returns> A pointer to the referred object. </returns>
						  pointer   get       ()       noexcept { enforce_has_value_debug(); return operator->(); }
					/// <summary> Undefined behaviour if has_value() returns false.</summary><returns> A pointer to the referred object. </returns>
					const_pointer   get       () const noexcept { enforce_has_value_debug(); return operator->(); }
							
					/// <summary>
					/// Check if this handle refers to an existing value.
					/// Can be false if the handle was set to null or if the referred object was erased from another handle.
					/// </summary>
					/// <returns>True if the handle can be dereferenced to a valid object, false otherwise. </returns>
					bool has_value() const noexcept
						{
						return handle_bare::slot_ptr && handle_bare::segment_ptr->used_bitset[index_in_segment()];
						}
					/// <summary>
					/// Check if this handle refers to an existing value.
					/// Can be false if the handle was set to null or if the referred object was erased from another handle.
					/// </summary>
					/// <returns>True if the handle can be dereferenced to a valid object, false otherwise. </returns>
					operator bool() const noexcept { return has_value(); }

					/// <summary>
					/// Only available if the container supports shared ownership.
					/// Undefined behaviour if has_value() returns false.
					/// </summary>
					/// <returns>The amount of shared handles that own to the same object this handle refers to. </returns>
					refcount_value_type use_count() const noexcept
						requires (use_refcount)
						{
						enforce_has_value_debug();
						return refcount(); 
						}

					/// <summary>
					/// Only available if the container supports shared ownership.
					/// Undefined behaviour if has_value() returns false.
					/// </summary>
					/// <returns>True if at least one shared handle owns the same object this handle refers to. False otherwise. </returns>
					bool has_shared_ownership() const noexcept
						requires (enabled_shared)
						{
						enforce_has_value_debug();

						refcount_value_type tmp{refcount()};
						if constexpr (enabled_unique)
							{
							return tmp > 0 && tmp < std::numeric_limits<refcount_value_type>::max();
							}
						else { return tmp > 0; }
						}
								
					/// <summary>
					/// Only available if the container supports unique ownership.
					/// Undefined behaviour if has_value() returns false.
					/// </summary>
					/// <returns>True if at an unique handle owns the same object this handle refers to. False otherwise. </returns>
					bool has_unique_ownership() const noexcept
						requires (enabled_unique)
						{
						enforce_has_value_debug();

						if constexpr (use_refcount || use_unique_bitset)
							{
							return refcount() == std::numeric_limits<refcount_value_type>::max();
							}
						//If container is only enabled to have uniques and not raws/shared, there's no refcount. Entity existstence is 1:1 to the unique handle's existence.
						else { return true; }
						}

					bool operator== (const handle_base& other) const noexcept { return handle_bare::slot_ptr == other.slot_ptr; }

				protected:
					handle_base() = default;
					handle_base(utils::observer_ptr<segment_t> segment_ptr, utils::observer_ptr<slot_t> slot_ptr) : handle_bare{segment_ptr, slot_ptr} {}

					auto& refcount()       noexcept requires(use_refcount     ) { return handle_bare::segment_ptr->get_refcount(handle_bare::slot_ptr); }
					auto  refcount() const noexcept requires(use_refcount     ) { return handle_bare::segment_ptr->get_refcount(handle_bare::slot_ptr); }
					auto  refcount()       noexcept requires(use_unique_bitset) { return handle_bare::segment_ptr->get_refcount(handle_bare::slot_ptr); }
					auto  refcount() const noexcept requires(use_unique_bitset) { return handle_bare::segment_ptr->get_refcount(handle_bare::slot_ptr); }
			
					size_t index_in_segment() const noexcept
						{
						return handle_bare::slot_ptr - std::addressof(handle_bare::segment_ptr->arr[0]);
						}

					void has_value_or_throw() const
						{
						if (!has_value())
							{
							throw std::logic_error{"Attempting to perform an operation that requires an handle to refer to a valid object on an handle that does not."};
							}
						}
					void enforce_has_value_debug() const
						{
						enforce_initialized_debug();
						if constexpr (utils::compilation::debug)
							{
							has_value_or_throw();
							}
						}
					void enforce_initialized_debug() const
						{
						if constexpr (utils::compilation::debug)
							{
							if (handle_bare::segment_ptr == nullptr) 
								{
								throw std::logic_error{"Attempting to perform an operation on an uninitialized handle."}; 
								}
							}
						}
					void enforce_unowned_debug() const
						{
						if constexpr (utils::compilation::debug)
							{
							bool tmp{false};
							if constexpr (enabled_unique) { if (has_unique_ownership()) { tmp = true; } }
							if constexpr (enabled_shared) { if (has_shared_ownership()) { tmp = true; } }
							if (tmp)
								{
								throw std::logic_error{"Attempting to perform an operation that requires a non-owned handle on an owned handle."};
								}
							}
						}
					void enforce_shared_debug() const
						{
						if constexpr (utils::compilation::debug)
							{
							bool tmp{false};
							if (!enabled_shared) { tmp = true; }
							if constexpr (enabled_shared) { if (!has_shared_ownership()) { tmp = true; } }
							if (tmp)
								{
								throw std::logic_error{"Attempting to perform an operation that requires a sharedly owned handle on a non sharedly owned handle."};
								}
							}
						}
					void enforce_shareable_debug() const
						{
						if constexpr (utils::compilation::debug)
							{
							if constexpr (enabled_unique)
								{
								if (has_unique_ownership())
									{
									throw std::logic_error{"Attempting to perform an operation that requires a non-owned or shared handle on an uniquely owned handle."};
									}
								}
							}
						}
				};
			
		public:
			class first_segment_t : private segment_t
				{
				public:
					using value_type        = object_pool_details::value_type       ;
					using reference         = object_pool_details::reference        ;
					using const_reference   = object_pool_details::const_reference  ;
					using pointer           = object_pool_details::pointer          ;
					using const_pointer     = object_pool_details::const_pointer    ;
					using iterator_category = object_pool_details::iterator_category;
					using difference_type   = object_pool_details::difference_type  ;

					inline static constexpr const bool enabled_raw   {object_pool_details::enabled_raw   };
					inline static constexpr const bool enabled_unique{object_pool_details::enabled_unique};
					inline static constexpr const bool enabled_shared{object_pool_details::enabled_shared};

					first_segment_t()
						{
						this->free_slot_handle.segment_ptr = this;
						this->free_slot_handle.slot_ptr = this->arr.data();
						}

					void clear()
						{
						segment_t::clear();
						segment_t::next_segment.reset();
						}

					/// <summary>
					/// A non-owning handle. Acts like an observer pointer, with additional features to retrieve information about potential owning handles that are owning the observed object.
					/// </summary>
					class handle_raw : public handle_base
						{
						friend class first_segment_t;
						public:
							using value_type        = object_pool_details::value_type       ;
							using reference         = object_pool_details::reference        ;
							using const_reference   = object_pool_details::const_reference  ;
							using pointer           = object_pool_details::pointer          ;
							using const_pointer     = object_pool_details::const_pointer    ;
							using iterator_category = object_pool_details::iterator_category;
							using difference_type   = object_pool_details::difference_type  ;

							inline static constexpr const bool enabled_raw   {object_pool_details::enabled_raw   };
							inline static constexpr const bool enabled_unique{object_pool_details::enabled_unique};
							inline static constexpr const bool enabled_shared{object_pool_details::enabled_shared};
							
							/// <summary> 
							/// Only available if the container supports raw ownership.
							/// Creates an empty handle. 
							/// Any method is undefined behaviour except for copy or move assignment, until a copy or move assignment from a valid handle occurred.
							//// </summary>
							handle_raw() requires (enabled_raw) = default;
			
							/// <summary>
							/// Nullifies the handle. has_value() will return false after this operation.
							/// </summary>
							void release() noexcept { handle_bare::slot_ptr = nullptr; }

							/// <summary>
							/// Erases the object referred to by this handle from the container.
							/// Undefined behaviour if the same object was owned by other non-raw handles.
							/// </summary>
							void reset() utils_if_release(noexcept)
								{
								if (handle_base::has_value())
									{
									if constexpr (utils::compilation::debug)
										{
										if constexpr (enabled_unique)
											{
											//has_unique_ownership() is always true in containers that only support unique handles
											//so we mustn't throw in that case
											if (handle_base::has_unique_ownership() && (enabled_raw || enabled_shared))
												{
												throw std::runtime_error{"Attempting to erase an handled owned by another unique handle."};
												}
											}
										if constexpr (enabled_shared)
											{
											if (handle_base::has_shared_ownership())
												{
												throw std::runtime_error{"Attempting to erase an handled owned by one or more shared handles."};
												}
											}
										}

									first_segment_ptr->erase(*this);
									handle_bare::slot_ptr = nullptr;
									}
								}
			
						protected:
							handle_raw() requires (!enabled_raw) = default;

							handle_raw(utils::observer_ptr<first_segment_t> first_segment_ptr, handle_base handle_base_instance) :
								first_segment_ptr{first_segment_ptr}, handle_base{handle_base_instance}
								{}
			
							utils::observer_ptr<first_segment_t> first_segment_ptr{nullptr};

						};

					/// <summary>
					/// A smart handle with unique ownership. Acts like an unique pointer.
					/// </summary>
					class handle_unique : public handle_raw
						{
						friend class first_segment_t;

						public:
							using value_type        = object_pool_details::value_type       ;
							using reference         = object_pool_details::reference        ;
							using const_reference   = object_pool_details::const_reference  ;
							using pointer           = object_pool_details::pointer          ;
							using const_pointer     = object_pool_details::const_pointer    ;
							using iterator_category = object_pool_details::iterator_category;
							using difference_type   = object_pool_details::difference_type  ;

							inline static constexpr const bool enabled_raw   {object_pool_details::enabled_raw   };
							inline static constexpr const bool enabled_unique{object_pool_details::enabled_unique};
							inline static constexpr const bool enabled_shared{object_pool_details::enabled_shared};

							/// <summary> 
							/// Only available if the container supports unique ownership.
							/// Creates an empty handle. 
							/// Any method is undefined behaviour except for move assignment, until a move assignment from a valid handle occurred.
							//// </summary>
							handle_unique() requires(enabled_unique) = default;

							handle_unique           (const handle_unique&  copy) = delete;
							handle_unique& operator=(const handle_unique&  copy) = delete;
							handle_unique           (      handle_unique&& move) noexcept 
								{
								handle_raw::operator=(move);
								move.handle_raw::release(); //use handle_raw's release so we don't zero the refcount in the container
								}
							handle_unique& operator=(      handle_unique&& move) noexcept 
								{
								reset();
			
								handle_raw::operator=(move);
								move.handle_raw::release(); //use handle_raw's release so we don't zero the refcount in the container
			
								return *this; 
								}

							/// <summary>
							/// Only available if the container supports both raw handles and unique ownership.
							/// Takes ownership of an object referred to by a raw handle.
							/// Undefined behaviour if the object referred to by the parameter raw handle was already owned by another owning handle.
							/// </summary>
							handle_unique(handle_raw& handle_raw_instance) requires (enabled_raw && enabled_unique) 
								{
								handle_raw_instance.enforce_unowned_debug();

								handle_raw::operator=(handle_raw_instance);
								if (handle_raw::has_value())
									{
									handle_raw::refcount() = std::numeric_limits<object_pool_details::refcount_value_type>::max();
									}
								}

							/// <summary>
							/// Only available if the container supports both raw handles and unique ownership.
							/// Takes ownership of an object referred to by a raw handle.
							/// Undefined behaviour if the object referred to by the parameter raw handle was already owned by another owning handle.
							/// </summary>
							handle_unique& operator=(handle_raw& handle_raw_instance)
								requires (enabled_raw && enabled_unique)
								{
								handle_raw_instance.enforce_unowned_debug();

								reset();

								handle_raw::operator=(handle_raw_instance);
								if (handle_raw::has_value())
									{
									handle_raw::refcount() = std::numeric_limits<object_pool_details::refcount_value_type>::max();
									}

								return *this;
								}
			
							~handle_unique() { reset(); }

							/// <summary>
							/// Erases the object referred to by this handle from the container.
							/// </summary>
							void reset()
								{
								release_refcount();
								handle_raw::reset();
								}

							/// <summary>
							/// Erases the object referred to by this handle from the container.
							/// Then replaces it taking ownership of the object referred to from the parameter raw handle.
							/// Undefined behaviour if the object referred to by the parameter raw handle was already owned by another owning handle.
							/// </summary>
							void reset(handle_raw& handle_raw_param)
								requires (enabled_raw)
								{
								operator=(std::move(handle_unique{handle_raw_param}));
								}

							/// <summary>
							/// Only available if the container supports raw handles.
							/// Releases the ownership of the referred object.
							/// Nullifies the handle. has_value() will return false after this operation.
							/// </summary>
							/// <returns>A raw handle referring to the previously owned object, if any.</returns>
							handle_raw release()
								requires (enabled_raw)
								{
								release_refcount();
			
								handle_raw ret{*this};
								handle_raw::release();
								return ret;
								}
			
						protected:
							handle_unique(handle_raw& handle_raw_instance)
								requires (!enabled_raw) :
								handle_raw{handle_raw_instance}
								{
								if constexpr (use_refcount || use_unique_bitset)
									{
									handle_raw::refcount() = std::numeric_limits<object_pool_details::refcount_value_type>::max();
									}
								};
			
							void release_refcount() noexcept
								{
								if constexpr (use_refcount || use_unique_bitset)
									{
									if (handle_raw::has_value())
										{
										handle_raw::refcount() = 0;
										}
									}
								}
						};
					
					/// <summary>
					/// A smart handle with unique ownership. Acts like a shared pointer.
					/// </summary>
					class handle_shared : public handle_raw
						{
						friend class first_segment_t;
						public:
							using value_type        = object_pool_details::value_type       ;
							using reference         = object_pool_details::reference        ;
							using const_reference   = object_pool_details::const_reference  ;
							using pointer           = object_pool_details::pointer          ;
							using const_pointer     = object_pool_details::const_pointer    ;
							using iterator_category = object_pool_details::iterator_category;
							using difference_type   = object_pool_details::difference_type  ;

							inline static constexpr const bool enabled_raw   {object_pool_details::enabled_raw   };
							inline static constexpr const bool enabled_unique{object_pool_details::enabled_unique};
							inline static constexpr const bool enabled_shared{object_pool_details::enabled_shared};

							/// <summary> 
							/// Only available if the container supports shared ownership.
							/// Creates an empty handle. 
							/// Any method is undefined behaviour except for copy or move assignment, until a copy or move assignment from a valid handle occurred.
							//// </summary>
							handle_shared() requires(enabled_shared) = default;
							
							handle_shared           (const handle_shared&  copy) noexcept 
								{
								handle_raw::operator=(copy);
								if (handle_raw::has_value()) { handle_raw::refcount()++; }
								}
							handle_shared& operator=(const handle_shared&  copy)
								{
								reset();
			
								handle_raw::operator=(copy);
								if (handle_raw::has_value()) { handle_raw::refcount()++; }
			
								return *this;
								}
			
							handle_shared           (      handle_shared&& move) noexcept
								{
								handle_raw::operator=(move);
								move.handle_raw::release(); //use handle_raw's release so we don't decrease the refcount in the container
								}
							handle_shared& operator=(      handle_shared&& move) noexcept 
								{
								reset();
			
								handle_raw::operator=(move);
								move.handle_raw::release(); //use handle_raw's release so we don't decrease the refcount in the container
			
								return *this; 
								}

							/// <summary>
							/// Only available if the container supports both raw handles and shared ownership.
							/// Takes ownership of an object referred to by a raw handle.
							/// Undefined behaviour if the object referred to by the parameter raw handle was already owned by an unique handle.
							/// </summary>
							handle_shared(handle_raw& handle_raw_instance)
								requires (enabled_raw && enabled_shared) 
								{
								handle_raw_instance.enforce_shareable_debug();

								handle_raw::operator=(handle_raw_instance);
								handle_raw::refcount()++;
								}
								
							/// <summary>
							/// Only available if the container supports both raw handles and shared ownership.
							/// Takes ownership of an object referred to by a raw handle.
							/// Undefined behaviour if the object referred to by the parameter raw handle was already owned by an unique handle.
							/// </summary>
							handle_shared& operator=(handle_raw& handle_raw_instance)
								requires (enabled_raw && enabled_shared)
								{
								handle_raw_instance.enforce_shareable_debug();

								reset();

								handle_raw::operator=(handle_raw_instance);
								handle_raw::refcount()++;

								return *this;
								}
			
							~handle_shared() { reset(); }
			
							/// <summary>
							/// Erases the object referred to by this handle from the container.
							/// </summary>
							void reset() 
								{
								if (release_refcount()) { handle_raw::reset(); }
								else { handle_raw::release(); }
								}

							/// <summary>
							/// Erases the object referred to by this handle from the container.
							/// Then replaces it taking ownership of the object referred to from the parameter raw handle.
							/// Undefined behaviour if the object referred to by the parameter raw handle was already owned by an unique handle.
							/// </summary>
							void reset(handle_raw& handle_raw_param)
								{
								handle_raw::enforce_shareable_debug();
								operator=(std::move(handle_shared{handle_raw_param}));
								}

							/// <summary>
							/// Only available if the container supports raw handles.
							/// Releases the ownership of the referred object.
							/// Nullifies the handle. has_value() will return false after this operation.
							/// </summary>
							/// <returns> A raw handle referring to the previously owned object, if any. </returns>
							handle_raw release()
								requires (enabled_raw)
								{
								release_refcount();
			
								handle_raw ret{*this};
								handle_raw::release();
								return ret;
								}
			
						protected:
							handle_shared(handle_raw& handle_raw_instance)
								requires (!enabled_raw) :
								handle_raw{handle_raw_instance}
								{
								handle_raw::refcount()++;
								};
			
							/// <returns> True if this was the last shared pointer to that resource, false otherwise. </returns>
							bool release_refcount() noexcept
								{
								if (handle_raw::has_value())
									{
									handle_raw::refcount()--;
									if (handle_raw::refcount() == 0)
										{
										return true;
										}
									}
								return false;
								}
						};
						
					class iterator : public handle_base
						{
						friend class first_segment_t;
						friend struct segment_t;
					
						public:
							using value_type        = object_pool_details::value_type       ;
							using reference         = object_pool_details::reference        ;
							using const_reference   = object_pool_details::const_reference  ;
							using pointer           = object_pool_details::pointer          ;
							using const_pointer     = object_pool_details::const_pointer    ;
							using iterator_category = object_pool_details::iterator_category;
							using difference_type   = object_pool_details::difference_type  ;
					
							iterator() = default;
					
							iterator  operator+ (difference_type rhs) const noexcept
								{//TODO can do better, naive is enough for now
								auto ret{*this};
								for (size_t i = 0; i < rhs; i++) { ret++; }
								return ret;
								}
							iterator  operator- (difference_type rhs) const noexcept
								{//TODO can do better, naive is enough for now
								auto ret{*this};
								for (size_t i = 0; i < rhs; i++) { ret--; }
								return ret;
								}
					
							difference_type operator- (const iterator& rhs) const noexcept
								{ //TODO
								throw std::exception{"TODO"};
								return 0;
								}
					
							iterator& operator+=(difference_type rhs) noexcept { *this = *this + rhs; return *this; }
							iterator& operator-=(difference_type rhs) noexcept { *this = *this - rhs; return *this; }
					
							iterator& operator++() noexcept
								{
								auto current_segment_end{handle_bare::segment_ptr->arr.data() + segment_size};
								while (true)
									{
									handle_bare::slot_ptr++;

									if (handle_bare::slot_ptr == current_segment_end)
										{
										if (handle_bare::segment_ptr->next_segment)
											{
											handle_bare::segment_ptr = handle_bare::segment_ptr->next_segment.get();
											handle_bare::slot_ptr    = handle_bare::segment_ptr->arr.data();
											current_segment_end = handle_bare::segment_ptr->arr.data() + segment_size;
											}
										else { break; }
										}
									if (handle_base::has_value()) { break; }
									}
					
								return *this;
								}
					
							iterator operator++(int) noexcept { auto ret{*this}; operator++(); return ret; }
					
							iterator& operator--() noexcept
								{
								do
									{
									handle_bare::slot_ptr--;
									if (handle_bare::slot_ptr == (handle_bare::segment_ptr->arr.data() - 1))
										{
										if constexpr (utils::compilation::debug)
											{
											if (!handle_bare::segment_ptr->prev_segment) { throw std::out_of_range{"Trying to access previous segment from the first segment."}; }
											}
										handle_bare::segment_ptr = handle_bare::segment_ptr->prev_segment;
										handle_bare::slot_ptr = handle_bare::segment_ptr->arr.data() + (segment_size - 1);
										}
									} while (!handle_base::has_value());
					
								return *this;
								}

							iterator operator--(int) noexcept { auto ret{*this}; operator--(); return ret; }
					
							auto operator<=>(const iterator& rhs) const noexcept { return (rhs - *this); }
					
						protected:
							iterator(utils::observer_ptr<segment_t> segment_ptr, utils::observer_ptr<slot_t> slot_ptr) : handle_base{segment_ptr, slot_ptr} {}
					
							/// <returns> True if end of segment was reached. </returns>
							bool advance_until_value_or_end_of_segment()
								{
								do
									{
									handle_bare::slot_ptr++;
									if (handle_base::slot_ptr == (handle_bare::segment_ptr->arr.data() + segment_size))
										{
										return true;
										}
									} 
								while (!handle_base::has_value());
								return false;
								}
						};

					template <object_pool_handle_version object_pool_handle_version = object_pool_handle_version::raw, typename ...Args>
					inline auto emplace(Args&&... args)
						{
						if constexpr (object_pool_handle_version == object_pool_handle_version::raw)
							{
							return emplace_inner(std::forward<Args>(args)...);
							}
						else if constexpr (object_pool_handle_version == object_pool_handle_version::unique)
							{
							return make_unique(std::forward<Args>(args)...);
							}
						else if constexpr (object_pool_handle_version == object_pool_handle_version::shared)
							{
							return make_shared(std::forward<Args>(args)...);
							}
						}
			
					template <typename ...Args>
					[[nodiscard]] inline handle_unique make_unique(Args&&... args)
						requires (enabled_unique)
						{
						auto tmp{emplace_inner(std::forward<Args>(args)...)};
						return {tmp};
						}
			
					template <typename ...Args>
					[[nodiscard]] inline handle_shared make_shared(Args&&... args)
						requires (enabled_shared)
						{
						auto tmp{emplace_inner(std::forward<Args>(args)...)};
						return {tmp};
						}
						
					[[nodiscard]] inline handle_raw handle_to_iterator(iterator it)
						requires (enabled_raw)
						{
						return {this, it};
						}

					iterator begin()
						{
						iterator ret{this, segment_t::arr.data()};
						if (!ret.has_value())
							{
							while (ret.advance_until_value_or_end_of_segment() && ret.segment_ptr->next_segment)
								{
								ret.segment_ptr = ret.segment_ptr->next_segment.get();
								}
							}
						return ret;
						}
					
					iterator end() { return {last_segment_ptr, last_segment_ptr->arr.data() + segment_size}; }
			
				private:
					handle_bare free_slot_handle;
					utils::observer_ptr<segment_t> last_segment_ptr{this}; //TODO check if this is always == to free_slot_handle.segment_ptr

					template <typename ...Args>
					inline handle_raw emplace_inner(Args&&... args)
						{
						if (free_slot_handle.slot_ptr == nullptr)
							{
							free_slot_handle.segment_ptr->next_segment = std::make_unique<segment_t>(free_slot_handle.segment_ptr);
							
							last_segment_ptr = free_slot_handle.segment_ptr->next_segment.get();
							free_slot_handle = {last_segment_ptr, last_segment_ptr->arr.data()};
							}
			
						auto next_free_slot_it{free_slot_handle.slot_ptr->free_slot_handle};
			
						free_slot_handle.segment_ptr->emplace(free_slot_handle.slot_ptr, std::forward<Args>(args)...);

						handle_base base{handle_base{free_slot_handle.segment_ptr, free_slot_handle.slot_ptr}};
						handle_raw ret{this, base};
			
						free_slot_handle = next_free_slot_it;
			
						return ret;
						}
			
					inline void erase(handle_bare& handle_bare)
						{
						free_slot_handle = handle_bare.segment_ptr->erase(handle_bare.slot_ptr, free_slot_handle);
						}
			
				};
			
		};
	}

namespace utils::containers
	{
	/// <summary>
	///		A container with fast insertion and removal. Iteration speed varies. If many removals happened, iteration will be slowed down. Otherwise iteration performance is similar to a deque.
	///		Handles require a pointer to the first segment, which is statically allocated, so they will be invalidated if the container itself is subject to a move operation.
	///		In that case it might be desirable to store the object_pool in an unique_ptr.
	///		An object_pool that doesn't grow past its segment size acts similarly to a static array.
	/// </summary>
	/// <param name="T"> The type contained by the object pool</param>
	/// <param name="Allocator"></param>
	/// <param name="segment_size">
	///		The size of sequential storage clusters for the type T. The first cluster is statically allocated.
	/// </param>
	/// <param name="refcount_value_T"> 
	///		The type used for refcounts if shared handles are enabled. 
	///		The maximum amount of shared handles to the same resource is std::numeric_limits  refcount_value_T  ::max() - 1. 
	///		The last value is reserved as special value for unique handles if unique handles are enabled.
	/// </param>
	/// <param name="handle_version_flags"></param>
	template
		<
		typename T,
		size_t segment_size = 8,
		flags<object_pool_handle_version> handle_version_flags = flags<object_pool_handle_version>::full(),
		std::unsigned_integral refcount_value_T = uint8_t,
		typename Allocator = std::allocator<T>
		>
	using object_pool = details::object_pool_details<T, segment_size, handle_version_flags, refcount_value_T, Allocator>::first_segment_t;
	}