#pragma once

#include <variant>
#include <vector>
#include <array>
#include <iterator>
#include <ranges>
#include <initializer_list>

#include "aabb.h"
#include "segment.h"
#include "../vec2.h"
#include "../angle.h"
#include "../transform2.h"


namespace utils::math::geometry
	{
	class polygon
		{
		public:
			polygon(std::initializer_list<vec2f>&& vertices) : _vertices{ std::forward<std::initializer_list<vec2f>>(vertices) } {};

			polygon(const std::vector<vec2f>& vertices) : _vertices{vertices} {}
			polygon(std::vector<vec2f>&& vertices) : _vertices{std::move(vertices)} {}

#pragma region vertices and segments iteration
#pragma region support types
			struct segment_ref
				{
				utils::math::vecref2f a;
				utils::math::vecref2f b;

				segment_ref(vec2f& a, vec2f& b) noexcept : a{a}, b{b} {}
				operator segment() const noexcept { return {a, b}; }

				segment_ref(segment_ref&& move) = delete;
				segment_ref& operator=(segment_ref&& move) = delete;

				segment_ref& operator=(const segment& other) // { a.operator=(other.a); b.operator=(other.b); return *this; } 
					//TODO understand why it tries to cast other.a and other.b to vecref, trying to call vecref::operator=(vecref) instead of trying to call vecref::operator=(vec)
					{
					a.x = other.a.x;
					a.y = other.a.y;
					b.x = other.b.x;
					b.y = other.b.y;
					return *this;
					}
				};

			template <typename container_t>
			class iterator
				{
				public:
					iterator() = default;
					iterator(container_t& container, size_t index = 0) noexcept : container{&container}, index{index} {}

					using iterator_category = std::random_access_iterator_tag;
					using value_type = typename container_t::value_type;
					using pointer = void;
					using reference = value_type;
					using difference_type = std::ptrdiff_t;

					iterator<container_t>& operator++()    noexcept { index++; return *this; }
					iterator<container_t>  operator++(int) noexcept { iterator<container_t> tmp(*this); ++(*this); return tmp; }
					iterator<container_t>& operator--()    noexcept { index--; return *this; }
					iterator<container_t>  operator--(int) noexcept { iterator<container_t> tmp(*this); --(*this); return tmp; }

					iterator<container_t>& operator+=(const difference_type& n) { index += n; return *this; }
					iterator<container_t>& operator-=(const difference_type& n) { index -= n; return *this; }
					iterator<container_t>  operator+ (const difference_type& n)           const { return iterator<container_t>{container, {index + n}}; }
					iterator<container_t>  operator- (const difference_type& n)           const { return iterator<container_t>{container, {index - n}}; }
					difference_type        operator- (const iterator<container_t>& other) const { return index - other.index; }

					friend iterator<container_t> operator+(const difference_type& n, const iterator<container_t>& iterator) { return {iterator.container, n + iterator.index}; }

					reference operator[](const difference_type& n) const { return (*container)[index + n]; }

					bool operator== (const iterator<container_t>& other) const noexcept { return index ==  other.index; }
					bool operator<=>(const iterator<container_t>& other) const noexcept { return index <=> other.index; }

					reference operator* () const noexcept { return  (*container)[index]; }

				private:
					size_t index{0};
					utils::observer_ptr<container_t> container{nullptr};
				};
			class edges_ref
				{
				friend class polygon;
				public:
					using value_type = segment_ref;

					size_t size() const noexcept { return poly_ptr->edges_count(); }
					segment_ref operator[](size_t index) const noexcept { return poly_ptr->edges_get(index); }

					auto begin() const noexcept { return iterator{*this}; }
					auto end()   const noexcept { return iterator{*this, size()}; }

				private:
					edges_ref(polygon& poly) noexcept : poly_ptr{&poly} {}
					utils::observer_ptr<polygon> poly_ptr;
				};
			class edges_cpy
				{
				friend class polygon;
				public:
					using value_type = segment;

					size_t size() const noexcept { return poly_ptr->edges_count(); }
					segment operator[](size_t index) const noexcept { return poly_ptr->edges_get(index); }

					auto begin() const noexcept { return iterator(*this); }
					auto end()   const noexcept { return iterator(*this, size()); }

				private:
					edges_cpy(const polygon& poly) noexcept : poly_ptr{&poly} {}
					utils::observer_ptr<const polygon> poly_ptr;
				};
#pragma endregion support types

			std::ranges::ref_view<      std::vector<vec2f>> get_vertices()       noexcept { return std::views::all(_vertices); };
			std::ranges::ref_view<const std::vector<vec2f>> get_vertices() const noexcept { return std::views::all(_vertices); };

			edges_ref    get_edges()       noexcept { return edges_ref{*this}; }
			edges_cpy    get_edges() const noexcept { return edges_cpy{*this}; }

		private:
			segment_ref edges_get(size_t index)       noexcept { return {_vertices[index], _vertices[(index + 1) % _vertices.size()]}; };
			segment     edges_get(size_t index) const noexcept { return {_vertices[index], _vertices[(index + 1) % _vertices.size()]}; };
			//segment get_edge(size_t index) const noexcept { return {_vertices[index], _vertices[(index + 1) % _vertices.size()]]}; }
			size_t edges_count() const noexcept { return _vertices.size(); }

#pragma endregion vertices and segments iteration
		public:
			aabb bounding_box() const noexcept
				{
				aabb ret
					{
					.ll =  constants::finf,
					.up =  constants::finf,
					.rr = -constants::finf,
					.dw = -constants::finf,
					};

				for(const auto& vertex : get_vertices())
					{
					ret.left  = std::min(ret.left,  vertex.x);
					ret.up    = std::min(ret.up,    vertex.y);
					ret.right = std::max(ret.right, vertex.x);
					ret.down  = std::max(ret.down,  vertex.y);
					}

				return ret;
				}

			private:
				std::vector<vec2f> _vertices;


		};

	struct convex_polygon : polygon 
		{
		convex_polygon(std::initializer_list<vec2f>&& vertices) : polygon{std::forward<std::initializer_list<vec2f>>(vertices)} {};
		convex_polygon(const std::vector<vec2f>& vertices) : polygon{vertices} {}
		convex_polygon(      std::vector<vec2f>& vertices) : polygon{std::move(vertices)} {}
		};

	aabb bounding_box(const polygon& from) { return from.bounding_box(); }
	}