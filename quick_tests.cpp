
#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>
#include <array>

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

class Overweight
	{
	public:
		Overweight(char c) noexcept { set_value(c); }
		char get_value() const noexcept { return weight[0]; }
		void set_value(char c) noexcept { weight[0] = c; }

#pragma optimize("", off)
		void do_stuff() {}
#pragma optimize("", on)

	private:
		std::array<char, 64> weight;
	};
#pragma optimize("", off)
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

	constexpr size_t length = 1000;

	{//Expected: decent growth, fastest iteration, not trackable
	std::vector<Overweight> vec_int;

	//grow
	for (size_t i = 0; i < length; i++) { vec_int.emplace_back(i); }

	//iterate
	for (auto& element : vec_int) { element.do_stuff(); }

	//track
	Overweight* observer = std::addressof(vec_int[length / 2]);
	Overweight new_location = std::move(vec_int[length / 2]);
	new_location.set_value(0);

	std::cout << static_cast<int>(observer->get_value()) << " == " << 0 << "?" << std::endl;
	}

	std::cout << "\n\n";

	{//Expected: fastest growth, slowest iteration, trackable
	std::vector<std::unique_ptr<Overweight>> vec_int;

	//grow
	for (size_t i = 0; i < length; i++) { vec_int.emplace_back(std::make_unique<Overweight>(i)); }

	//iterate
	for (auto& element : vec_int) { element->do_stuff(); }

	//track
	Overweight* observer = vec_int[length / 2].get();
	std::unique_ptr<Overweight> new_location = std::move(vec_int[length / 2]);
	new_location->set_value(0);
	std::cout << static_cast<int>(observer->get_value()) << " == " << 0 << "?" << std::endl;
	}

	std::cout << "\n\n";

	{//Expected: slowest growth, fast iteration, trackable
	std::vector<utils::trackable_wrapper<Overweight>> vec_int;

	//grow
	for (size_t i = 0; i < length; i++) { vec_int.emplace_back(i); }

	//iterate
	for (auto& element : vec_int) { element->do_stuff(); }

	//track
	utils::tracking_ptr<Overweight> observer{vec_int[length / 2]};
	utils::trackable_wrapper<Overweight> new_location = std::move(vec_int[length / 2]);
	new_location->set_value(0);
	std::cout << static_cast<int>(observer->get_value()) << " == " << 0 << "?" << std::endl;
	}

	std::cout << "\n\n";

	}