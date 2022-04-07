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

        /*BEGIN_TEST_METHOD_ATTRIBUTE(getStatisticsFrom)
        TEST_OWNER(L"MCBerberov19")
        TEST_PRIORITY("High")
        END_TEST_METHOD_ATTRIBUTE()*/
        TEST_METHOD(getStatisticsFrom)
        {
            //Arrange
            std::vector<ss::types::Cycle> cycles;
            std::vector<ss::types::Cycle> expected = 
            {
                {50, {{2.5f, 3.4f},{10.0f, 1.1f}}},
                {25, {{10.0f, 1.1f},{2.5f, 3.4f}}},
            };

            //Act
            cycles = ss::dal::StatisticStore::getStatisticFrom("json_test");

            //Assert
           /* for (size_t i = 0; i < cycles.size(); i++)
            {
                Assert::AreEqual(cycles[i].lastedEntities, expected[i].lastedEntities);

                for (size_t j = 0; j < cycles[i].traitsInfo.size(); j++)
                {
                    Assert::AreEqual(expected[i].traitsInfo[j].sense, cycles[i].traitsInfo[j].sense, L"No");
                    Assert::AreEqual(expected[i].traitsInfo[j].speed, cycles[i].traitsInfo[j].speed, L"No");
                }
            }*/
            //Assert::AreEqual(1, 1);
            Assert::AreSame(expected, cycles);
        }
};