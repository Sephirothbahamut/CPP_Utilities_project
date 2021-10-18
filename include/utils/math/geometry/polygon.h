#pragma once

#include <variant>
#include <vector>
#include <array>

#include "../vec2.h"
#include "../../index_range.h"
//#include "segment.h" Must be included by segment

namespace utils::math::geometry
	{
	class shape
		{
		public:
			virtual bool contains(const vec2f& point) = 0;
			virtual bool contains(const shape& shape) = 0;
		};

	class polygon : shape
		{
		friend class iterable_vertices;
		friend class iterable_edges;
		public:
			virtual size_t vertices_count() const noexcept = 0;
			virtual vec2f get_vertex(size_t index) const noexcept = 0;
			segment get_edge(size_t index) const noexcept { return {get_vertex(index), get_vertex((index + 1) % vertices_count())}; }

		protected:
		};

	class concave : public polygon
		{
		public:
			virtual size_t vertices_count() const noexcept override final { return  _vertices.size(); };
			virtual vec2f get_vertex(size_t index) const noexcept { return  _vertices[index]; }
			
		private:
			std::vector<vec2f> _vertices;
		};

	class triangle : public polygon 
		{
		public:
			virtual size_t vertices_count() const noexcept override final { return 3; };
			virtual vec2f get_vertex(size_t index) const noexcept { return  _vertices[index]; }
		private:
			std::array<vec2f, 3> _vertices;
		};

	class AABB : public polygon
		{
		public:
			virtual size_t vertices_count() const noexcept { return 4; };
			virtual vec2f get_vertex(size_t index) const noexcept
				{
				switch (index)
					{
					case 0: return {ul.x, ul.y};
					case 1: return {dr.x, ul.y};
					case 2: return {dr.x, dr.y};
					case 3: return {ul.x, dr.y};
					}
				};

		private:
			vec2f ul, dr;
		};
	}