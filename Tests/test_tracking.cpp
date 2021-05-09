#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <vector>
#include <algorithm>
#include <utils/tracking.h>


struct TrackableClass : public utils::trackable
	{
	TrackableClass(int n) : value(n) {}
	int value = 0;
	void function() {}
	};

struct DerivedTrackable : public TrackableClass {};

struct SomeClass
	{
	SomeClass(int n) : value(n) {}
	int value;
	void function() {}
	};


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

				Assert::AreEqual(ptr_a->value, b.value);
				Assert::AreEqual(ptr_b->value, a.value);
				}

			TEST_METHOD(access_derived_class_after_move)
				{
				DerivedTrackable a{0};
				DerivedTrackable b{1};

				utils::tracking_ptr<DerivedTrackable> ptr_a{a};
				utils::tracking_ptr<DerivedTrackable> ptr_b{b};

				std::swap(a, b);

				Assert::AreEqual(ptr_a->value, b.value);
				Assert::AreEqual(ptr_b->value, a.value);
				}

			TEST_METHOD(access_derived_class_from_parent_ptr)
				{
				DerivedTrackable a{0};
				DerivedTrackable b{1};

				utils::tracking_ptr<TrackableClass> ptr_a{a};
				utils::tracking_ptr<TrackableClass> ptr_b{b};

				std::swap(a, b);

				Assert::AreEqual(ptr_a->value, b.value);
				Assert::AreEqual(ptr_b->value, a.value);
				}

			TEST_METHOD(access_wrapped_class_after_move)
				{
				utils::trackable_wrapper<SomeClass> a{0};
				utils::trackable_wrapper<SomeClass> b{1};

				utils::tracking_ptr<SomeClass> ptr_a{a};
				utils::tracking_ptr<SomeClass> ptr_b{b};

				std::swap(a, b);

				Assert::AreEqual(ptr_a->value, b->value);
				Assert::AreEqual(ptr_b->value, a->value);
				}

			TEST_METHOD(nullptr_after_trackable_destruction)
				{
				utils::tracking_ptr<TrackableClass> ptr;

				if (true)
					{
					TrackableClass a{1};
					ptr = a;
					Assert::AreEqual(ptr->value, a.value);
					}

				Assert::AreEqual(ptr.has_value(), false);
				}

			TEST_METHOD(nullptr_after_wrapped_destruction)
				{
				utils::tracking_ptr<SomeClass> ptr;

				if (true)
					{
					utils::trackable_wrapper<SomeClass> a{1};
					ptr = a;
					Assert::AreEqual(ptr->value, a->value);
					}

				Assert::AreEqual(ptr.has_value(), false);
				}

			TEST_METHOD(move_around_vector_inheritance)
				{
				std::vector<TrackableClass> vec;

				for (size_t i = 0; i < 10; i++) { vec.emplace_back(i); }

				utils::tracking_ptr<TrackableClass> ptr2 = vec[2];
				utils::tracking_ptr<TrackableClass> ptr4 = vec[4];
				utils::tracking_ptr<TrackableClass> ptr6 = vec[6];

				std::partition(vec.begin(), vec.end(), [](TrackableClass& element) { return (element.value % 2); });

				Assert::AreEqual(vec[0].value % 2, 1);
				Assert::AreEqual(vec[1].value % 2, 1);
				Assert::AreEqual(vec[2].value % 2, 1);
				Assert::AreEqual(vec[3].value % 2, 1);
				Assert::AreEqual(vec[4].value % 2, 1);
				Assert::AreEqual(vec[5].value % 2, 0);
				Assert::AreEqual(vec[6].value % 2, 0);
				Assert::AreEqual(vec[7].value % 2, 0);
				Assert::AreEqual(vec[8].value % 2, 0);
				Assert::AreEqual(vec[9].value % 2, 0);

				Assert::AreEqual(ptr2->value, 2);
				Assert::AreEqual(ptr4->value, 4);
				Assert::AreEqual(ptr6->value, 6);
				}

			TEST_METHOD(move_around_vector_wrapped)
				{
				std::vector<utils::trackable_wrapper<SomeClass>> vec;

				for (size_t i = 0; i < 10; i++) { vec.emplace_back(i); }

				utils::tracking_ptr<SomeClass> ptr2 = vec[2];
				utils::tracking_ptr<SomeClass> ptr4 = vec[4];
				utils::tracking_ptr<SomeClass> ptr6 = vec[6];

				std::partition(vec.begin(), vec.end(), [](SomeClass& element) { return (element.value % 2); });

				Assert::AreEqual(vec[0]->value % 2, 1);
				Assert::AreEqual(vec[1]->value % 2, 1);
				Assert::AreEqual(vec[2]->value % 2, 1);
				Assert::AreEqual(vec[3]->value % 2, 1);
				Assert::AreEqual(vec[4]->value % 2, 1);
				Assert::AreEqual(vec[5]->value % 2, 0);
				Assert::AreEqual(vec[6]->value % 2, 0);
				Assert::AreEqual(vec[7]->value % 2, 0);
				Assert::AreEqual(vec[8]->value % 2, 0);
				Assert::AreEqual(vec[9]->value % 2, 0);

				Assert::AreEqual(ptr2->value, 2);
				Assert::AreEqual(ptr4->value, 4);
				Assert::AreEqual(ptr6->value, 6);
				}

		};
	}
