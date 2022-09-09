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
	TEST_CLASS(get_degree_from_radian)
	{
	public:

		BEGIN_TEST_METHOD_ATTRIBUTE(toDegreeFromRadian1)
			TEST_OWNER(L"MCBerberov19")
			TEST_PRIORITY("Low")
			END_TEST_METHOD_ATTRIBUTE()
			TEST_METHOD(toDegreeFromRadian1)
		{
			// Arrange
			double radian = 0;
			double expected = 45;

			// Act
			radian = ss::bll::utils::toDegree(0.785398);

			// Assert
			Assert::AreEqual(std::round(expected), std::round(radian), L"Equality != =");
		}
	};
}
// clang-format on