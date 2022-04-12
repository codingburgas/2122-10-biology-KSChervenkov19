// clang-format off
#include "pch_unit_testing.h"
#include "CppUnitTest.h"
#include "statistics_manager.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(statistics_files_names_getter_2)
{
public:

	BEGIN_TEST_METHOD_ATTRIBUTE(getStatisticsNames2)
		TEST_OWNER(L"MCBerberov19")
		TEST_PRIORITY("Low")
		END_TEST_METHOD_ATTRIBUTE()
		TEST_METHOD(getStatisticsNames2)
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
		filesNames = ss::bll::statistics::StatisticsManager::getStatisticsNames();
		expected = { "Test0", "Test1", "Test2", "Test3" };
		//std::remove(R"(/2122-10-biology-KSChervenkov19\statistics\Test1.json)");
		//std::remove(R"(/2122-10-biology-KSChervenkov19\statistics\Test2.json)");
		//std::remove(R"(/2122-10-biology-KSChervenkov19\statistics\Test3.json)");

		// Assert
		Assert::IsTrue(expected == filesNames, L"Equality != =");
	}
};
// clang-format on