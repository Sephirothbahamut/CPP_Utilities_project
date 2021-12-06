
#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>
#include <array>

#include "include/utils/containers/buffer.h"
#include "include/utils/math/vec2.h"
//#include "include/utils/math/geometry/segment.h"
//#include "include/utils/math/geometry/polygon.h"

#include "include/utils/index_range.h"

template <std::forward_iterator T>
void f(T t) {};

int main()
	{
	/*namespace utm = utils::math;

	utm::vec2f a{2, 3};
	utm::vec2f b{4, 5};

	float      dot_result;
	utm::vec2f cross_result;

	dot_result   = utm::vec2f::dot(a, b);
	cross_result = a * b;

	using namespace utm::operators;

	dot_result   = a <dot> b;
	cross_result = a <cross> b;*/

	/*using namespace utils::math::geometry;

	using it_t = polygon::iterator<polygon::vertices_cpy>;
	static_assert(std::weakly_incrementable<it_t>);
	static_assert(std::indirectly_readable<it_t>);
	static_assert(std::input_or_output_iterator<it_t>);
	static_assert(std::input_iterator<it_t>);
	static_assert(std::bidirectional_iterator<it_t>);
	static_assert(std::random_access_iterator<it_t>);

	polygon::triangle tri;

	polygon::n_gon n_gon{{{0, 0}, {1, 0}, {1, 1}, {0.5, 2}, {0, 1}}};
	polygon::AABB aabb;

	aabb.up = 0;
	aabb.dw = 1;
	aabb.ll = 0;
	aabb.rr = 1;

	for (auto vertex : n_gon.get_vertices())
		{
		std::cout << "(" << vertex.x << ", " << vertex.y << ") ";
		}

	for (auto edge : n_gon.get_edges_ref())
		{
		edge.a.x++;
		edge.a.y++;
		}

	f(n_gon.get_vertices().begin());
	auto vertices = n_gon.get_vertices();
	auto edges = n_gon.get_edges_ref();
	for (size_t i : utils::indices(edges))
		{
		}

	std::cout << "\n_______________________\n";

	for (auto vertex : n_gon.get_vertices())
		{
		std::cout << "(" << vertex.x << ", " << vertex.y << ") ";
		}*/

	std::vector<int> v;
	for (size_t i : utils::indices(v))
		{

		}
	}
