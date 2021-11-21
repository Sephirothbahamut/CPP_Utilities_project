#pragma once

#include <variant>
#include <vector>
#include <array>
#include <iterator>

#include "shape.h"

namespace utils::math::geometry
	{
	class polygon : shape
		{
		public:
#pragma region vertices and segments iteration
			struct vertex_ref
				{
				float& x;
				float& y;

				vertex_ref(vec2f& source)      noexcept : x{source.x}, y{source.y} {}
				vertex_ref(float& x, float& y) noexcept : x{x},        y{y}        {}
				operator vec2f() const noexcept { return {x, y}; }
				vertex_ref& operator=(const vec2f& other) { x = other.x; y = other.y; }
				};
			struct segment_ref
				{
				vertex_ref a;
				vertex_ref b;
				
				segment_ref(vec2f& a,      vec2f& b)    noexcept : a{a}, b{b} {}
				segment_ref(vertex_ref a, vertex_ref b) noexcept : a{a}, b{b} {}
				operator segment() const noexcept { return {a, b}; }
				segment_ref& operator=(const segment& other) { a = other.a; b = other.b; }
				};

			template <typename container_t>
			class iterator
				{
				public:
					iterator() = default;
					iterator(container_t& container, size_t index = 0) noexcept : container{&container}, index{index} {}

					using iterator_category = std::random_access_iterator_tag;
					using value_type        = typename container_t::value_type;
					using pointer           = void;
					using reference         = value_type;
					using difference_type   = std::ptrdiff_t;

					iterator<container_t>& operator++()    noexcept { index++; return *this; }
					iterator<container_t>  operator++(int) noexcept { iterator<container_t> tmp(*this); ++(*this); return tmp; }
					iterator<container_t>& operator--()    noexcept { index--; return *this; }
					iterator<container_t>  operator--(int) noexcept { iterator<container_t> tmp(*this); --(*this); return tmp; }

					iterator<container_t>  operator+ (const difference_type& n)           const { return iterator<container_t>{container, {index + n}}; }
					iterator<container_t>& operator+=(const difference_type& n) { index += n; return *this; }
					iterator<container_t>  operator- (const difference_type& n)           const { return iterator<container_t>{container, {index - n}}; }
					iterator<container_t>& operator-=(const difference_type& n) { index -= n; return *this; }
					difference_type        operator- (const iterator<container_t>& other) const { return index - other.index; }

					friend iterator<container_t> operator+(const difference_type& n, const iterator<container_t>& iterator) { return {iterator.container, n + iterator.index}; }

					reference operator[](const difference_type& n) const { return (*container)[index + n]; }

					bool operator==(const iterator<container_t>& other) const noexcept { return index == other.index; }
					bool operator!=(const iterator<container_t>& other) const noexcept { return index != other.index; }
					bool operator< (const iterator<container_t>& other) const noexcept { return index < other.index; }
					bool operator> (const iterator<container_t>& other) const noexcept { return index > other.index; }
					bool operator<=(const iterator<container_t>& other) const noexcept { return index <= other.index; }
					bool operator>=(const iterator<container_t>& other) const noexcept { return index >= other.index; }

					reference   operator* () const noexcept { return  (*container)[index]; }

				private:
					size_t index{0};
					container_t* container{nullptr};
				};

			class vertices_ref
				{
				friend class polygon;
				public:
					using value_type = vertex_ref;

					vertices_ref(polygon* poly) noexcept : poly{poly} {}

					size_t size() const noexcept { return poly->vertices_count(); }
					vertex_ref operator[](size_t index) const noexcept { return poly->vertices_ref_get(index); }

					auto begin() const noexcept { return iterator{*this}; }
					auto end()   const noexcept { return iterator{*this, size()}; }
				private:
					polygon* poly;
				};


			class vertices_cpy
				{
				friend class polygon;
				public:
					using value_type = vec2f;

					vertices_cpy(const polygon* poly) noexcept : poly{poly} {}

					size_t size() const noexcept { return poly->vertices_count(); }
					vec2f operator[](size_t index) const noexcept { return poly->vertices_get(index); }

					auto begin() const noexcept { return iterator(*this); }
					auto end()   const noexcept { return iterator(*this, size()); }
				private:
					const polygon* poly;
				};
			class edges_ref
				{
				friend class polygon;
				public:
					using value_type = segment_ref;

					edges_ref(polygon* poly) noexcept : poly{poly} {}

					size_t size() const noexcept { return poly->edges_count(); }
					segment_ref operator[](size_t index) const noexcept { return poly->edges_ref_get(index); }

					auto begin() const noexcept { return iterator{*this}; }
					auto end()   const noexcept { return iterator{*this, size()}; }
				private:
					polygon* poly;
				};
			class edges_cpy
				{
				friend class polygon;
				public:
					using value_type = vec2f;

					edges_cpy(const polygon* poly) noexcept : poly{poly} {}

					size_t size() const noexcept { return poly->edges_count(); }
					segment operator[](size_t index) const noexcept { return poly->edges_get(index); }

					auto begin() const noexcept { return iterator(*this); }
					auto end()   const noexcept { return iterator(*this, size()); }
				private:
					const polygon* poly;
				};
#pragma endregion vertices and segments iteration

			vertices_ref get_vertices_ref()   noexcept { return vertices_ref{this}; }
			vertices_cpy get_vertices() const noexcept { return vertices_cpy{this}; }
			edges_ref    get_edges_ref()      noexcept { return edges_ref   {this}; }
			edges_cpy    get_edges()    const noexcept { return edges_cpy   {this}; }

			virtual size_t vertices_count() const noexcept = 0;
			virtual vertex_ref  vertices_ref_get(size_t index)       noexcept = 0;
			virtual vec2f       vertices_get    (size_t index) const noexcept = 0;
			virtual segment_ref edges_ref_get   (size_t index)       noexcept { return {vertices_ref_get(index), vertices_ref_get(index % vertices_count())}; };
			virtual segment     edges_get       (size_t index) const noexcept { return {vertices_get(index), vertices_get(index % vertices_count())}; };
			size_t edges_count() const noexcept { return vertices_count(); }

			segment get_edge(size_t index) const noexcept { return {vertices_get(index), vertices_get((index + 1) % vertices_count())}; }

		};

		class concave : polygon
			{};

		class n_gon : public polygon
			{
			public:
				virtual size_t vertices_count() const noexcept override final { return  _vertices.size(); };
				virtual vertex_ref vertices_ref_get(size_t index)       noexcept override { return  _vertices[index]; }
				virtual vec2f      vertices_get(size_t index) const noexcept override { return {_vertices[index]}; }

				n_gon(const std::vector<vec2f>& vertices) : _vertices{vertices} {}

			private:
				std::vector<vec2f> _vertices;
			};

	class triangle : public polygon
		{
		public:
			virtual size_t vertices_count() const noexcept override final { return 3; };
			virtual vertex_ref vertices_ref_get(size_t index)       noexcept override { return  _vertices[index];  }
			virtual vec2f      vertices_get    (size_t index) const noexcept override { return {_vertices[index]}; }
		private:
			std::array<vec2f, 3> _vertices;
		};

	class AABB : public polygon
		{
		public:
			float up{0.f};
			float dw{0.f};
			float ll{0.f};
			float rr{0.f};

			vec2f get_ul() const noexcept { return {up, ll}; }
			vec2f get_ur() const noexcept { return {up, rr}; }
			vec2f get_dl() const noexcept { return {dw, ll}; }
			vec2f get_dr() const noexcept { return {dw, rr}; }
			void set_ul(vec2f v) noexcept { up = v.y; ll = v.x; }
			void set_ur(vec2f v) noexcept { up = v.y; rr = v.x; }
			void set_dl(vec2f v) noexcept { dw = v.y; ll = v.x; }
			void set_dr(vec2f v) noexcept { dw = v.y; rr = v.x; }

			__declspec(property(get = get_ul, put = set_ul)) vec2f ul;
			__declspec(property(get = get_ur, put = set_ur)) vec2f ur;
			__declspec(property(get = get_dl, put = set_dl)) vec2f dl;
			__declspec(property(get = get_dr, put = set_dr)) vec2f dr;

			virtual size_t vertices_count() const noexcept { return 4; };
			virtual vertex_ref vertices_ref_get(size_t index) noexcept override
				{
				switch (index)
					{
					case 0: return {up, ll};
					case 1: return {up, rr};
					case 2: return {dw, rr};
					case 3: return {dw, ll};
					}
				};
			virtual vec2f  vertices_get(size_t index) const noexcept override
				{
				switch (index)
					{
					case 0: return {up, ll};
					case 1: return {up, rr};
					case 2: return {dw, rr};
					case 3: return {dw, ll};
					}
				};
		};
	using axis_aligned_bounding_box = AABB;
	}