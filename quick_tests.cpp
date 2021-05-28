
#include <iostream>
#include <string>
#include <stdexcept>

#include "include/utils/tracking.h"

struct TrackableClass : public utils::trackable
	{
	TrackableClass(int n) : n(n) {}
	int n = 0;
	void function() {}
	};

struct DerivedTrackable : public TrackableClass {};

struct SomeClass
	{
	SomeClass(int n) : n(n) {}
	int n;
	void function() {}
	};


int main()
	{
	DerivedTrackable a{1};
	DerivedTrackable b{2};

	utils::tracking_ptr<DerivedTrackable> ptr_a{a};
	utils::tracking_ptr<DerivedTrackable> ptr_b{b};

	a = std::move(b);
	/*Expected:
	ptr_a points to nullptr
	ptr_b points to a
	b has a new tracker
	b's previous tracker is now a's tracker
	*/

	std::cout << ptr_a.has_value() << ", " << ptr_b.has_value() << std::endl;
	}