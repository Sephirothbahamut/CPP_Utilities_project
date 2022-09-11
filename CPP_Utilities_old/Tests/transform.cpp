#include "pch.h"

#include <vector>
#include <algorithm>
#include <utils/math/vec2.h>
#include <utils/math/Transform2.h>

#include <string>

#include "CppUnitTest.h"


namespace Microsoft
	{
	namespace VisualStudio
		{
		namespace CppUnitTestFramework
			{
			using namespace std::literals;
			template<> static std::wstring ToString<utils::math::vec2<float>>(const class utils::math::vec2<float>& a) { return (std::to_wstring(a.x) + L", "s + std::to_wstring(a.y)); }
			}
		}
	}

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
	{
	TEST_CLASS(Transform)
		{
		public:

			TEST_METHOD(vec2)
				{
				using namespace utils::math::angle::literals;
				using namespace utils::math::angle;
				using namespace utils::math::operators;

				utils::math::vec2f v{0, 5};
				utils::math::transform2 t{{10, 10}, static_cast<rad>(0_deg), 1.f};

				Assert::AreEqual(utils::math::vec2f{10, 15}, v * t);
				}

		};
	}