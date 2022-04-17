// clang-format off
#include "pch_unit_testing.h"
#include "CppUnitTest.h"
#include "simulation_data.h"
#include "statistics_store.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ss::dal::unit_testing
{
	TEST_CLASS(statistics_getter)
	{
	public:

		BEGIN_TEST_METHOD_ATTRIBUTE(getStatisticsFrom1)
			TEST_OWNER(L"MCBerberov19")
			TEST_PRIORITY("High")
			END_TEST_METHOD_ATTRIBUTE()
			TEST_METHOD(getStatisticsFrom1)
		{
			std::ofstream out(R"(/2122-10-biology-KSChervenkov19\statistics\Test0.json)");
			if (out.is_open())
			{
				out << "[{\"Alive\":50,\"Traits\":[{\"Sense\":2.5,\"Speed\":3.4000000953674316},{\"Sense\":10.0,\"Speed\":1."
					"100000023841858}]},{\"Alive\":25,\"Traits\":[{\"Sense\":10.0,\"Speed\":1.100000023841858},{\"Sense\":2.5,"
					"\"Speed\":3.4000000953674316}]}]";
				out.close();
			}

			// Arrange
			std::vector<ss::types::Cycle> cycles;
			std::vector<ss::types::Cycle> expected = {
				{50, {{2.5f, 3.4f}, {10.0f, 1.1f}}},
				{25, {{10.0f, 1.1f}, {2.5f, 3.4f}}},
			};

			// Act
			cycles = ss::dal::StatisticsStore::getStatisticFrom("Test0");
			//std::remove("Test0.json");

			// Assert
			Assert::IsTrue(expected == cycles, L"Equality != =");
		}
	};
}
// clang-format on