#include <cmath> //NaN check

#include <utils/math/rect.h>

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
	{
	TEST_CLASS(math_rect)
		{
		public:
			TEST_METHOD(all)
				{
				const utils::math::rect<float> source{.ll{1}, .up{1}, .rr{2}, .dw{2}};
				utils::math::rect<float> r{.ll{1}, .up{1}, .rr{2}, .dw{2}};

				r.x() += 5;

				Assert::AreEqual(r.ll, source.ll + 5);
				Assert::AreEqual(r.up, source.up);
				Assert::AreEqual(r.rr, source.rr + 5);
				Assert::AreEqual(r.dw, source.dw);

				r.position() += 5;

				Assert::AreEqual(r.ll, source.ll + 5 + 5);
				Assert::AreEqual(r.up, source.up + 5);
				Assert::AreEqual(r.rr, source.rr + 5 + 5);
				Assert::AreEqual(r.dw, source.dw + 5);

				r.size() -= 5;

				Assert::AreEqual(r.ll, source.ll + 5 + 5);
				Assert::AreEqual(r.up, source.up + 5);
				Assert::AreEqual(r.rr, source.rr + 5);
				Assert::AreEqual(r.dw, source.dw);
				}
		};
	}
