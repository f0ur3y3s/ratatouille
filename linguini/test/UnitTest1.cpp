#include "pch.h"
#include "CppUnitTest.h"
extern "C"
{
#include "calc.h"
}

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(UnitTestCalc)
	{
	public:
		
		TEST_METHOD(TestAddFunc)
		{ 
			int result = add(1, 2);
			Assert::AreEqual(3, result);
			// logger the result as digit using logger

			Logger::WriteMessage(std::to_wstring(result).c_str());

		}

		TEST_METHOD(TestSubtractFunc)
        {
            int result = subtract(5, 3);
            Assert::AreEqual(2, result);
            // logger the result as digit using logger

            Logger::WriteMessage(
                std::to_wstring(result).c_str());
        }
	};

}

