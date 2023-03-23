#include <string>
#include <iostream>

#include "include/utils/memory.h"
#include "include/utils/console/colour.h"

#include "include/utils/math/vec.h"
#include "include/utils/math/vec2.h"
#include "include/utils/math/vec3.h"
#include "include/utils/math/angle.h"
#include "include/utils/math/rect.h"

#include "include/utils/output/std_containers.h"
#include "include/utils/graphics/colour.h"

#include "include/utils/containers/matrix.h"
#include "include/utils/logger.h"

#include "include/utils/containers/handled_container.h"
#include "include/utils/containers/multihandled.h"

#include "include/utils/containers/hive/next.h"
#include "include/utils/containers/linked_vector.h"
#include "include/utils/containers/segmented_vector.h"

#include "include/utils/containers/multithreading/multiqueue_consumer.h"

#include "include/utils/containers/object_pool.h"

#include "include/utils/oop/counting.h"

#include "include/utils/math/geometry/interactions.h"

using civ = utils::oop::counting_invalidating_move;

struct test_struct : civ
	{
	test_struct(int v) : v{v} {}
	int v;
	};

int main()
	{
	using namespace utils::output;

	utils::math::geometry::circle<false> circ{.center{0.f, 0.f}, .radius{5.f}};

	utils::math::geometry::point<false> p{1.f, 1.f};
	
	p.closest_point_and_distance(circ);
	std::cout << circ.contains(circ);

	//if (auto opt{circ.collision(p)})
	//	{
	//	std::cout << opt.value();
	//	}



	//Memory references to sequential storage on gpu

	//std::vector<utils::math::vec2f> memory_pool
	//	{
	//		//polygon vertices
	//		{1, 2}, {2, 3}, {3, 4}, 
	//		//point
	//		{2.5, 2.5},
	//		//circle {pos }, {radius, 0}
	//		{3, 2}, {2, 0} 
	//	};

	//utils::math::geometry::polygon<true> ref_poly{memory_pool.data(), memory_pool.data() + 3};
	//utils::math::geometry::point  <true> ref_pt  {*(memory_pool.data() + 3)};
	//utils::math::geometry::circle <true> ref_circ{.center{*(memory_pool.data() + 4)}, .radius{(memory_pool.data() + 5)->y}};
	}