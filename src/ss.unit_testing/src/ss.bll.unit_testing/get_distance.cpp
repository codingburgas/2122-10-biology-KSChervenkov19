// clang-format off
#include "pch_unit_testing.h"
#include "CppUnitTest.h"
#include "utils.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ss::bll::unit_testing
{
	/// Test case for getDistance method in bll.
	/// 
	/// Testing if the function gets the appropriate result from bll utils method.
	TEST_CLASS(get_distance_between_two_points)
	{
	public:

		BEGIN_TEST_METHOD_ATTRIBUTE(get_distance1)
			TEST_OWNER(L"MCBerberov19")
			TEST_PRIORITY("High")
			END_TEST_METHOD_ATTRIBUTE()
			TEST_METHOD(get_distance1)
		{
			// Arrange
			float distance = 0.f;
			float expected = 2.0f;

			// Act
			distance = ss::bll::utils::getDistance({3, 4}, {2, 2});

			// Assert
			Assert::AreEqual(std::round(expected), std::round(distance), L"Equality != =");
		}
	};
}
// clang-format on