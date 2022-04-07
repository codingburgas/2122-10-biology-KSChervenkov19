// clang-format off
#include "pch_unit_testing.h"
#include "CppUnitTest.h"
#include "statistics.h"
#include "statistics_store.h"
// clang-format on

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(statistics_getter)
{
public:

        BEGIN_TEST_METHOD_ATTRIBUTE(getStatisticsFrom1)
        TEST_OWNER(L"MCBerberov19")
        TEST_PRIORITY("High")
        END_TEST_METHOD_ATTRIBUTE()
        TEST_METHOD(getStatisticsFrom1)
        {
            std::ofstream out("Test.json");
            out << "[{\"Alive\":50,\"Traits\":[{\"Sense\":2.5,\"Speed\":3.4000000953674316},{\"Sense\":10.0,\"Speed\":1.100000023841858}]},{\"Alive\":25,\"Traits\":[{\"Sense\":10.0,\"Speed\":1.100000023841858},{\"Sense\":2.5,\"Speed\":3.4000000953674316}]}]";
            out.close();

            //Arrange
            std::vector<ss::types::Cycle> cycles;
            std::vector<ss::types::Cycle> expected = 
            {
                {50, {{2.5f, 3.4f},{10.0f, 1.1f}}},
                {25, {{10.0f, 1.1f},{2.5f, 3.4f}}},
            };

            //Act
            cycles = ss::dal::StatisticStore::getStatisticFrom("Test");
            std::remove("Test.json");

            //Assert
            Assert::IsTrue(expected == cycles, L"Is not equal!");
        }
};