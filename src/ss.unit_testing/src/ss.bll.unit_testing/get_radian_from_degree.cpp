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
			double degree = 0;
			double expected = 0.785398;

			// Act
			degree = ss::bll::utils::toRadian(45);

			// Assert
			Assert::AreEqual(std::round(expected), std::round(degree), L"Equality != =");
		}
	};
}
// clang-format on