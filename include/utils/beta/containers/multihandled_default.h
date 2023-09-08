#pragma once

#include <utility>
#include <concepts>

#include "../../containers/object_pool.h"

namespace utils::containers
	{
	/// <summary>
	/// This container is generally unsafe. Operations on handles do not give strong guarantees. Meant to be used as base to implement safe variations upon.
	/// Erasures and emplaces/pushes must not be concurrent. Dereferencing handles during those operations is safe.
	/// This is true because the hive::next implementation does not touch or rely on used slots during other operations.
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <typeparam name="inner_size"></typeparam>

	template <typename T, size_t inner_size = 8>
	class multihandled_default
		{
		private:
			class inner_container_t : public utils::containers::object_pool<T, inner_size, utils::containers::object_pool_handle_version::raw>
				{
				private:
					using inner_inner_container_t = utils::containers::object_pool<T, inner_size, utils::containers::object_pool_handle_version::raw>;

				public:
					/// <summary>
					/// Raw when pointing to default, unique otherwise
					/// </summary>
					struct handle_rawnique : inner_inner_container_t::handle_raw
						{
						~handle_rawnique()
							{
							if (!is_default()) { inner_inner_container_t::handle_raw::reset(); }
							}

						bool is_default() const noexcept
							{
							auto& container{*static_cast<inner_container_t*>(inner_inner_container_t::handle_raw::first_segment_ptr)};
							return inner_inner_container_t::handle_raw::operator==(container.default_handle);
							}
						};

					inner_container_t::handle_rawnique default_handle;

					template <typename ...Args>
					inner_container_t(Args&&... args) : default_handle{inner_inner_container_t::emplace(std::forward<Args>(args)...)} {}

				public:
					template <typename ...Args>
					inline handle_rawnique emplace(Args&&... args)
						{
						return {inner_inner_container_t::emplace(std::forward<Args>(args)...)};
						}
				};

			using handles_container_t = utils::containers::object_pool
				<
				typename inner_container_t::handle_rawnique, inner_size, 
				utils::flags
					{
					utils::containers::object_pool_handle_version::raw,
					utils::containers::object_pool_handle_version::unique,
					utils::containers::object_pool_handle_version::shared
					}
				>;

		private:
			inner_container_t   inner_container;
			handles_container_t handles_container;

			struct handle_flag {};

			template <typename handle_t>
			struct handle_base : handle_t, handle_flag
				{
				friend class multihandled_default<T, inner_size>;
				private:
					using parent_t = handle_t;
					inner_container_t::handle_rawnique& get_rawnique() noexcept { return parent_t::operator*(); }

				public:
					bool is_default() const noexcept { return parent_t::operator->()->is_default(); }
				
					auto& operator* ()       noexcept { return parent_t::operator* (). operator* (); }
					auto& operator* () const noexcept { return parent_t::operator* (). operator* (); }
					auto  operator->()       noexcept { return parent_t::operator->()->operator->(); }
					auto  operator->() const noexcept { return parent_t::operator->()->operator->(); }
					auto& value     ()                { return parent_t::value(). value(); }
					auto& value     () const          { return parent_t::value(). value(); }
					auto  get       ()       noexcept { return parent_t::get  ()->get  (); }
					auto  get       () const noexcept { return parent_t::get  ()->get  (); }
				};

		public:
			using handle_raw    = handle_base<typename handles_container_t::handle_raw   >;
			using handle_unique = handle_base<typename handles_container_t::handle_unique>;
			using handle_shared = handle_base<typename handles_container_t::handle_shared>;

			template <typename ...Args>
			multihandled_default(Args&&... args) : inner_container{std::forward<Args>(args)...} {}

			void clear() { inner_container.clear(); handles_container.clear(); }

			template <typename ...Args>
			inline handle_raw emplace(Args&&... args)
				{
				static_assert(std::constructible_from<T, Args...>, "Invalid parameters for object construction");

				auto rawnique{inner_container.emplace(std::forward<Args>(args)...)};
				return {handles_container.emplace(rawnique)};
				}

			template <typename ...Args>
			[[nodiscard]] inline handle_unique make_unique(Args&&... args)
				{
				static_assert(std::constructible_from<T, Args...>, "Invalid parameters for object construction");

				auto rawnique{inner_container.emplace(std::forward<Args>(args)...)};
				return {handles_container.make_unique(rawnique)};
				}

			template <typename ...Args>
			[[nodiscard]] inline handle_shared make_shared(Args&&... args)
				{
				static_assert(std::constructible_from<T, Args...>, "Invalid parameters for object construction");

				auto rawnique{inner_container.emplace(std::forward<Args>(args)...)};
				return {handles_container.make_shared(rawnique)};
				}

			using value_type      = inner_container_t::value_type;
			using reference       = inner_container_t::reference;
			using const_reference = inner_container_t::const_reference;
			using pointer         = inner_container_t::pointer;
			using const_pointer   = inner_container_t::const_pointer;

			template <object_pool_handle_version object_pool_handle_version = object_pool_handle_version::raw>
			auto clone_default() noexcept
				{
				auto default_handle_copy{inner_container.default_handle};
				return handle_base{handles_container.emplace<object_pool_handle_version>(default_handle_copy)};
				}

			template <std::derived_from<handle_flag> T, typename ...Args>
			void emplace_in_handle(T& handle, Args&&... args)
				{
				static_assert(std::constructible_from<value_type, Args...>, "Invalid parameters for object construction");

				if (!handle.is_default()) { throw std::runtime_error{"This handle was already reassigned from default."}; }
				typename inner_container_t::handle_rawnique& rawnique_to_default{handle.get_rawnique()};

				auto rawnique_new{inner_container.emplace(std::forward<Args>(args)...)};
				rawnique_to_default = rawnique_new;
				}

		};
	}