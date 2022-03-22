#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <vector>
#include <algorithm>
#include <utils/tracking.h>
//#include <../Beta/include/utils/tracking.h>


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

using WrappedClass = utils::trackable_wrapper<SomeClass>;

namespace Tests
	{
	TEST_CLASS(Tracking)
		{
		public:

			TEST_METHOD(access_trackable_class_after_move)
				{
				TrackableClass a{0};
				TrackableClass b{1};

				utils::tracking_ptr<TrackableClass> ptr_a{a};
				utils::tracking_ptr<TrackableClass> ptr_b{b};

				std::swap(a, b);

				Assert::AreEqual(ptr_a->n, b.n);
				Assert::AreEqual(ptr_b->n, a.n);
				}

			TEST_METHOD(access_derived_class_after_move)
				{
				DerivedTrackable a{0};
				DerivedTrackable b{1};

				utils::tracking_ptr<DerivedTrackable> ptr_a{a};
				utils::tracking_ptr<DerivedTrackable> ptr_b{b};

				std::swap(a, b);

				Assert::AreEqual(ptr_a->n, b.n);
				Assert::AreEqual(ptr_b->n, a.n);
				}

			TEST_METHOD(access_derived_class_from_parent_ptr)
				{
				DerivedTrackable a{0};
				DerivedTrackable b{1};

				utils::tracking_ptr<TrackableClass> ptr_a{a};
				utils::tracking_ptr<TrackableClass> ptr_b{b};

				std::swap(a, b);

				Assert::AreEqual(ptr_a->n, b.n);
				Assert::AreEqual(ptr_b->n, a.n);
				}

			TEST_METHOD(access_wrapped_class_after_move)
				{
				WrappedClass a{0};
				WrappedClass b{1};

				utils::tracking_ptr<SomeClass> ptr_a{a};
				utils::tracking_ptr<SomeClass> ptr_b{b};

				std::swap(a, b);

				Assert::AreEqual(ptr_a->n, b->n);
				Assert::AreEqual(ptr_b->n, a->n);
				}

			TEST_METHOD(not_access_moved_from_derived_class)
				{
				DerivedTrackable a{0};
				DerivedTrackable b{1};

				utils::tracking_ptr<DerivedTrackable> ptr_a{a};
				utils::tracking_ptr<DerivedTrackable> ptr_b{b};

				a = std::move(b);

				Assert::AreEqual(ptr_b->n, a.n);
				Assert::IsFalse(ptr_a.has_value());
				}

			TEST_METHOD(not_access_moved_from_wrapped_class)
				{
				WrappedClass a{0};
				WrappedClass b{1};

				utils::tracking_ptr<SomeClass> ptr_a{a};
				utils::tracking_ptr<SomeClass> ptr_b{b};

				a = std::move(b);

				Assert::AreEqual(ptr_b->n, a->n);
				Assert::IsFalse(ptr_a.has_value());
				}

			TEST_METHOD(nullptr_after_trackable_destruction)
				{
				utils::tracking_ptr<TrackableClass> ptr;

				if (true)
					{
					TrackableClass a{1};
					ptr = a;
					Assert::AreEqual(ptr->n, a.n);
					}

				Assert::AreEqual(static_cast<bool>(ptr), false);
				}

			TEST_METHOD(nullptr_after_wrapped_destruction)
				{
				utils::tracking_ptr<SomeClass> ptr;

				if (true)
					{
					WrappedClass a{1};
					ptr = a;
					Assert::AreEqual(ptr->n, a->n);
					}

				Assert::AreEqual(static_cast<bool>(ptr), false);
				}

			TEST_METHOD(move_around_vector_inheritance)
				{
				std::vector<TrackableClass> vec;

				for (size_t i = 0; i < 10; i++) { vec.emplace_back(i); }

				utils::tracking_ptr<TrackableClass> ptr2 = vec[2];
				utils::tracking_ptr<TrackableClass> ptr4 = vec[4];
				utils::tracking_ptr<TrackableClass> ptr6 = vec[6];

				std::partition(vec.begin(), vec.end(), [](TrackableClass& element) { return (element.n % 2); });

				Assert::AreEqual(vec[0].n % 2, 1);
				Assert::AreEqual(vec[1].n % 2, 1);
				Assert::AreEqual(vec[2].n % 2, 1);
				Assert::AreEqual(vec[3].n % 2, 1);
				Assert::AreEqual(vec[4].n % 2, 1);
				Assert::AreEqual(vec[5].n % 2, 0);
				Assert::AreEqual(vec[6].n % 2, 0);
				Assert::AreEqual(vec[7].n % 2, 0);
				Assert::AreEqual(vec[8].n % 2, 0);
				Assert::AreEqual(vec[9].n % 2, 0);

				Assert::AreEqual(ptr2->n, 2);
				Assert::AreEqual(ptr4->n, 4);
				Assert::AreEqual(ptr6->n, 6);
				}

			TEST_METHOD(track_from_emplace)
				{
				std::vector<TrackableClass> vec;
				utils::tracking_ptr<TrackableClass> ptr0{vec.emplace_back(0)};
				utils::tracking_ptr<TrackableClass> ptr1{vec.emplace_back(1)};
				utils::tracking_ptr<TrackableClass> ptr2{vec.emplace_back(2)};
				utils::tracking_ptr<TrackableClass> ptr3{vec.emplace_back(3)};
				utils::tracking_ptr<TrackableClass> ptr4{vec.emplace_back(4)};
				utils::tracking_ptr<TrackableClass> ptr5{vec.emplace_back(5)};
				utils::tracking_ptr<TrackableClass> ptr6{vec.emplace_back(6)};
				utils::tracking_ptr<TrackableClass> ptr7{vec.emplace_back(7)};
				utils::tracking_ptr<TrackableClass> ptr8{vec.emplace_back(8)};
				utils::tracking_ptr<TrackableClass> ptr9{vec.emplace_back(9)};


				std::partition(vec.begin(), vec.end(), [](TrackableClass& element) { return (element.n % 2); });

				Assert::AreEqual(vec[0].n % 2, 1);
				Assert::AreEqual(vec[1].n % 2, 1);
				Assert::AreEqual(vec[2].n % 2, 1);
				Assert::AreEqual(vec[3].n % 2, 1);
				Assert::AreEqual(vec[4].n % 2, 1);
				Assert::AreEqual(vec[5].n % 2, 0);
				Assert::AreEqual(vec[6].n % 2, 0);
				Assert::AreEqual(vec[7].n % 2, 0);
				Assert::AreEqual(vec[8].n % 2, 0);
				Assert::AreEqual(vec[9].n % 2, 0);

				Assert::AreEqual(ptr2->n, 2);
				Assert::AreEqual(ptr4->n, 4);
				Assert::AreEqual(ptr6->n, 6);
				}

			TEST_METHOD(move_around_vector_wrapped)
				{
				std::vector<WrappedClass> vec;

				for (size_t i = 0; i < 10; i++) { vec.emplace_back(i); }

				utils::tracking_ptr<SomeClass> ptr2 = vec[2];
				utils::tracking_ptr<SomeClass> ptr4 = vec[4];
				utils::tracking_ptr<SomeClass> ptr6 = vec[6];

				std::partition(vec.begin(), vec.end(), [](SomeClass& element) { return (element.n % 2); });

				Assert::AreEqual(vec[0]->n % 2, 1);
				Assert::AreEqual(vec[1]->n % 2, 1);
				Assert::AreEqual(vec[2]->n % 2, 1);
				Assert::AreEqual(vec[3]->n % 2, 1);
				Assert::AreEqual(vec[4]->n % 2, 1);
				Assert::AreEqual(vec[5]->n % 2, 0);
				Assert::AreEqual(vec[6]->n % 2, 0);
				Assert::AreEqual(vec[7]->n % 2, 0);
				Assert::AreEqual(vec[8]->n % 2, 0);
				Assert::AreEqual(vec[9]->n % 2, 0);

				Assert::AreEqual(ptr2->n, 2);
				Assert::AreEqual(ptr4->n, 4);
				Assert::AreEqual(ptr6->n, 6);
				}

		};
	}
