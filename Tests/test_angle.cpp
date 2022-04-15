#include "pch.h"

#include <vector>
#include <algorithm>
#include <utils/math/angle.h>

#include "CppUnitTest.h"


namespace Microsoft
	{
	namespace VisualStudio
		{
		namespace CppUnitTestFramework
			{
			template<> static std::wstring ToString<utils::math::angle::deg>(const class utils::math::angle::deg& a) { return std::to_wstring(a.value); }
			template<> static std::wstring ToString<utils::math::angle::rad>(const class utils::math::angle::rad& a) { return std::to_wstring(a.value); }
			}
		}
	}

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace Tests
	{
	TEST_CLASS(Angles)
		{
		public:

			TEST_METHOD(operations)
				{
				using namespace utils::math::angle::literals;
				using namespace utils::math::angle;
				
				auto ad{ 90_deg};
				auto bd{ 90_deg};
				auto cd{270_deg};

				Assert::AreEqual(90.f, ad.value);

				Assert::AreEqual(180.f, (ad + bd).value);
				Assert::AreEqual(0.f, (ad - bd).value);

				Assert::AreEqual(false, (ad + cd).value ==    0.f);
				Assert::AreEqual(true,  (ad + cd).value ==  360.f);
				Assert::AreEqual(false, (ad + cd).value == -360.f);
				Assert::AreEqual(false, (ad + cd).value ==  720.f);
				}

			TEST_METHOD(equality_over_0_to_360)
				{
				using namespace utils::math::angle::literals;
				using namespace utils::math::angle;
				
				auto ad{ 90_deg};
				auto bd{ 90_deg};
				auto cd{270_deg};

				Assert::AreEqual((ad + cd), 0_deg);
				Assert::AreEqual((ad + cd), 360_deg);
				Assert::AreEqual((ad + cd), -360_deg);
				Assert::AreEqual((ad + cd), 720_deg);
				}

			TEST_METHOD(casts)
				{
				using namespace utils::math::angle::literals;
				using namespace utils::math::angle;

				auto ad{ 90_deg};
				auto bd{ 90_deg};
				auto cd{270_deg};

				rad ar{ad};
				rad br{bd};
				rad cr{cd};

				Assert::AreEqual(PIf / 2.f,       ar .value);
				Assert::AreEqual(180.f,     (bd + ar).value);
				}
		};
	}