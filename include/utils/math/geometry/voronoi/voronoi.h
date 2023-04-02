#pragma once

#include <vector>

#include "../../vec2.h"
#include "../shapes/polygon.h"

namespace utils::math::geometry::voronoi
	{
	struct cell : polygon //TODO make convex_polygon when convex_polygon's concept is fixed
		{
		inline cell(const vec2f& site, const std::vector<vec2f>& vertices) : polygon{vertices}, site{site} {}

		std::vector<utils::observer_ptr<cell>> neighbours;

		vec2f site;
		};

	class graph
		{
		public:
			graph(const std::span<vec2f>& points, const rect<float>& bounding_box);

			//std::vector<vec2f> vertices;
			//std::vector<edge_ref> edges;

			std::vector<cell> cells;
		};
	}

#ifdef utils_c_libraries_implementation
#define JC_VORONOI_IMPLEMENTATION
#include "../../../third_party/jc_voronoi.h"

namespace utils::math::geometry::voronoi
	{
	graph::graph(const std::span<vec2f>& points, const rect<float>& bounding_box)
		{
		std::vector<jcv_point> jcv_points(points.size());
		for (size_t i{0}; i < points.size(); i++)
			{
			jcv_points[i].x = points[i].x;
			jcv_points[i].y = points[i].y;
			}

		jcv_rect jcv_bounding_box{{bounding_box.ll, bounding_box.up}, {bounding_box.rr, bounding_box.dw}};

		jcv_diagram diagram;

		memset(&diagram, 0, sizeof(jcv_diagram));

		jcv_diagram_generate(jcv_points.size(), jcv_points.data(), &jcv_bounding_box, 0, &diagram);
		const jcv_site* sites{jcv_diagram_get_sites(&diagram)};

		for (size_t i{0}; i < diagram.numsites; i++)
			{
			std::vector<vec2f> vertices;

			jcv_graphedge* graph_edge{sites[i].edges};
			while (graph_edge)
				{
				vertices.emplace_back(graph_edge->pos[0].x, graph_edge->pos[0].y);
				graph_edge = graph_edge->next;
				}

			cells.emplace_back(utils::math::vec2f{sites[i].p.x, sites[i].p.y}, vertices);
			}

		for (size_t i{0}; i < diagram.numsites; i++)
			{
			jcv_graphedge* graph_edge{sites[i].edges};
			size_t edge_index{0};
			while (graph_edge)
				{
				cells[i].neighbours.push_back(graph_edge->neighbor ? &cells[graph_edge->neighbor->index] : nullptr);
				edge_index++;
				graph_edge = graph_edge->next;
				}
			}

		jcv_diagram_free(&diagram);
		}
	}
#endif