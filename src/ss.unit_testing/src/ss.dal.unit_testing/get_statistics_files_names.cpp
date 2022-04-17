// clang-format off
#include "pch_unit_testing.h"
#include "CppUnitTest.h"
#include "statistics_store.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ss::dal::unit_testing
{
	/// Test case for the getStatisticsNames function.
	/// 
	/// The function stored in the dal.
	TEST_CLASS(statistics_files_names_getter)
	{
	public:

		BEGIN_TEST_METHOD_ATTRIBUTE(getStatisticsNames1)
			TEST_OWNER(L"MCBerberov19")
			TEST_PRIORITY("High")
			END_TEST_METHOD_ATTRIBUTE()
			TEST_METHOD(getStatisticsNames1)
		{
			// Arrange
			std::ofstream json0(R"(/2122-10-biology-KSChervenkov19\statistics\Test0.json)");
			std::ofstream json1(R"(/2122-10-biology-KSChervenkov19\statistics\Test1.json)");
			std::ofstream json2(R"(/2122-10-biology-KSChervenkov19\statistics\Test2.json)");
			std::ofstream json3(R"(/2122-10-biology-KSChervenkov19\statistics\Test3.json)");
			std::vector<std::string> filesNames{};
			std::vector<std::string> expected{};
			expected.reserve(4);

			// Act
			filesNames = ss::dal::StatisticsStore::getStatisticsNames();
			expected = { "Test0", "Test1", "Test2", "Test3" };
			//std::remove(R"(/2122-10-biology-KSChervenkov19\statistics\Test1.json)");
			//std::remove(R"(/2122-10-biology-KSChervenkov19\statistics\Test2.json)");
			//std::remove(R"(/2122-10-biology-KSChervenkov19\statistics\Test3.json)");

			// Assert
			Assert::IsTrue(expected == filesNames, L"Equality != =");
		}
	};
}
// clang-format on