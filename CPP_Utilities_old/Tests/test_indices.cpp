#include "pch.h"

#include <vector>
#include <algorithm>
#include <utils/index_range.h>

#include "CppUnitTest.h"


namespace Microsoft
	{
	namespace VisualStudio
		{
		namespace CppUnitTestFramework
			{
			}
		}
	}

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
	{
	TEST_CLASS(Indices)
		{
		public:

			TEST_METHOD(iteration)
				{
				std::vector<int> a{1, 2, 3};
				std::vector<int> b(a);

				for (size_t i : utils::indices(a))
					{
					Assert::AreEqual(a[i], b[i]);
					}
				}
		};
	}