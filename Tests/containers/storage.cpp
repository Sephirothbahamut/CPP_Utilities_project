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
					  utils::storage::multiple<utils::storage::type::owner,       float, 3> array {1.f, 2.f, 3.f};
				const utils::storage::multiple<utils::storage::type::owner,       float, 3> carray{1.f, 2.f, 3.f};
					  utils::storage::multiple<utils::storage::type::owner, const float, 3> arrayc{1.f, 2.f, 3.f};

					  float ff{1.f};
				const float cf{2.f};
	
					  utils::storage::multiple<utils::storage::type::observer,       float, 3, false> oarray_f      {ff, ff, ff};
					  utils::storage::multiple<utils::storage::type::observer, const float, 3, false> oarray_cf     {ff, cf, ff};
				const utils::storage::multiple<utils::storage::type::observer,       float, 3, false> coarray_array {array };
					  utils::storage::multiple<utils::storage::type::observer, const float, 3, false> ocarray_array {array };
					  utils::storage::multiple<utils::storage::type::observer, const float, 3, false> ocarray_carray{carray};
					  utils::storage::multiple<utils::storage::type::observer, const float, 3, false> ocarray_arrayc{arrayc};
				}
		};
	}