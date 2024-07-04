#include <iostream>

#include <utils/tracking.h>

struct angry_type
	{
	angry_type(int i, float) : i{i} {}
	int i;
	};

struct child_type : angry_type
	{
	child_type(int i) : angry_type{i, 0.f}, another_i{i * 2} {}
	int another_i;
	};

void tracking_ptr()
	{
	utils::trackable_wrapper<child_type> child1{1};
	utils::trackable_wrapper<angry_type> parent1{1, 2.f};
	
	utils::tracking_ptr<child_type> child_tracker{child1};
	utils::tracking_ptr<angry_type> child_as_parent_tracker{child1};
	utils::tracking_ptr<angry_type> parent_tracker{parent1};
	
	auto child2{std::move(child1)};
	auto parent2{std::move(parent1)};
	
	child1 ->another_i = -1;
	child1 ->        i = -1;
	parent1->        i = -1;
	
	child1 = utils::trackable_wrapper<child_type>{2};
	
	std::cout << child_tracker          ->another_i << std::endl;
	std::cout << child_as_parent_tracker->        i << std::endl;
	std::cout <<          parent_tracker->        i << std::endl;
	}