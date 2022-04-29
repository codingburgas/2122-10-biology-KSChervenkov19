// clang-format off
#include "pch_unit_testing.h"
#include "CppUnitTest.h"
#include "utils.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ss::bll::unit_testing
{
	/// Test case for toRadian method in bll.
	/// 
	/// Testing if the function gets the appropriate result from bll utils method.
	TEST_CLASS(get_radian_from_degree)
	{
	public:

		BEGIN_TEST_METHOD_ATTRIBUTE(toRadianFromDegree1)
			TEST_OWNER(L"MCBerberov19")
			TEST_PRIORITY("Low")
			END_TEST_METHOD_ATTRIBUTE()
			TEST_METHOD(toRadianFromDegree1)
		{
			// Arrange
			double degree = 45.0;
			double expected;

			// Act
			expected = ss::bll::utils::toRadian(degree);

			// Assert
			Assert::AreEqual(std::round(expected), std::round(0.785398), L"Equality != =");
		}
	};
}
// clang-format on