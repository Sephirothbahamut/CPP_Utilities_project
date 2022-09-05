
#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>
#include <array>
#include "include/utils/graphics/color.h"

#include "include/utils/containers/buffer.h"
#include "include/utils/math/vec.h"
#include "include/utils/math/vec2.h"
//#include "include/utils/math/Transform2.h"
//#include "include/utils/math/geometry/segment.h"
//#include "include/utils/math/geometry/polygon.h"
//#include "include/utils/math/geometry/transformations.h"

#include "include/utils/index_range.h"

#include "include/utils/containers/handled_container.h"

#include "include/utils/cout_utilities.h"
#include "include/utils/cout_containers.h"

template <std::forward_iterator T>
void f(T t) {};

int main() 
	{
	using namespace utils::cout;
	namespace utm = utils::math;
	utm::vec<float, 2> a{1.f,1.f};
	utm::vec<float, 2> b{};
	utm::angle::deg alpha{10};
	utils::beta::graphics::colors::rgb<float, 4> ca, cb;
	utils::beta::graphics::colors::hsv<float, true> cc{1, 2, 3}, cd{1, 2, 3, 4};
	ca + cb;

	std::cout << ((a + b) + alpha) << std::endl;
	-a;
	a.angle();

	//utils::containers::handled_container<std::string> container;
	//
	//auto handle_a{container.emplace("hello")};
	//auto handle_b{container.emplace(" ")};
	//auto handle_c{container.emplace("world")};
	//
	//for (const auto& element : container) { std::cout << element; } std::cout << std::endl;
	//
	//container.remove(handle_b);
	//
	//for (const auto& element : container) { std::cout << element; } std::cout << std::endl;
	//
	//handle_b = container.emplace("!");
	//
	//for (const auto& element : container) { std::cout << element; } std::cout << std::endl;
	//
	//container.remove(handle_c);
	//
	//for (const auto& element : container) { std::cout << element; } std::cout << std::endl;
	}