
#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>
#include <array>
#include "include/utils/graphics/color.h"

#include "include/utils/containers/buffer.h"
#include "include/utils/math/vec.h"
//#include "include/utils/math/geometry/segment.h"
//#include "include/utils/math/geometry/polygon.h"
//#include "include/utils/math/geometry/transformations.h"

#include "include/utils/index_range.h"

#include "include/utils/containers/handled_container.h"

template <std::forward_iterator T>
void f(T t) {};

int main() 
	{
	namespace utm = utils::math;
	utm::vec<float, 2> a;
	utm::vec<float, 2> b;
	//utm::angle::deg alpha{10};
	(a + b);// +alpha;



	utils::containers::handled_container<std::string> container;

	auto handle_a{container.emplace("hello")};
	auto handle_b{container.emplace(" ")};
	auto handle_c{container.emplace("world")};

	for (const auto& element : container) { std::cout << element; } std::cout << std::endl;

	container.remove(handle_b);

	for (const auto& element : container) { std::cout << element; } std::cout << std::endl;

	handle_b = container.emplace("!");

	for (const auto& element : container) { std::cout << element; } std::cout << std::endl;

	container.remove(handle_c);

	for (const auto& element : container) { std::cout << element; } std::cout << std::endl;
	}