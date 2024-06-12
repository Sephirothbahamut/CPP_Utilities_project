#include <vector>
#include <deque>
#include <list>
#include <algorithm>
#include <sstream> 
#include "../../include/utils/storage.h"

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
	TEST_CLASS(storage)
		{
		public:
			TEST_METHOD(general)
				{
				#define nocst /**/
				nocst utils::storage::multiple<nocst float, 3> array {1.f, 2.f, 3.f};
				const utils::storage::multiple<nocst float, 3> carray{1.f, 2.f, 3.f};
				nocst utils::storage::multiple<const float, 3> arrayc{1.f, 2.f, 3.f};
				nocst utils::storage::multiple<const float, 4> erray{array};
				nocst utils::storage::multiple<const float, 2> brray{array};

				nocst float ff{1.f};
				const float cf{2.f};

				nocst utils::storage::multiple<nocst float&, 3, false> oarray_f      {ff, ff, ff};
				nocst utils::storage::multiple<const float&, 3, false> oarrayc_cf    {ff, cf, ff};
				nocst utils::storage::multiple<nocst float&, 2, false> oarray_array  {array};
				const utils::storage::multiple<nocst float&, 3, false> coarray_array {array };
				nocst utils::storage::multiple<const float&, 3, false> ocarray_array {array };
				nocst utils::storage::multiple<const float&, 3, false> ocarray_carray{carray};
				nocst utils::storage::multiple<const float&, 3, false> ocarray_arrayc{arrayc};
				#undef nocst
				}
		};
	}