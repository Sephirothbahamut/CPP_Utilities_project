#include "pch.h"

#include <vector>
#include <algorithm>
#include <utils/containers/buffer.h>

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
	TEST_CLASS(Buffer)
		{
		public:

			TEST_METHOD(operations)
				{
				utils::containers::buffer buffer;

				float a = 5;
				char b = 'a';
				uint32_t c = 123;

				Assert::IsTrue(buffer.empty());

				buffer.push(a);
				buffer.push(b);
				buffer.push(c);

				Assert::IsTrue(!buffer.empty());

				Assert::IsTrue(buffer.has<float>());
				Assert::AreEqual(a, buffer.get<float>());

				Assert::IsTrue(buffer.has<char>());
				Assert::AreEqual(b, buffer.get<char>());

				Assert::IsTrue(!buffer.has<uint64_t>());
				Assert::IsTrue(buffer.has<uint32_t>());
				Assert::AreEqual(c, buffer.get<uint32_t>());

				Assert::IsTrue(buffer.empty());
				}
		};
	}