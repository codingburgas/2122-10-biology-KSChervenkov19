// clang-format off
#include "pch_unit_testing.h"
#include "CppUnitTest.h"
#include "graph.h"
#include "statistics_store.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ss::pl::unit_testing
{
	TEST_CLASS(get_highest_sense)
	{
	public:

		BEGIN_TEST_METHOD_ATTRIBUTE(getHighestSense1)
			TEST_OWNER(L"MCBerberov19")
			TEST_PRIORITY("High")
			END_TEST_METHOD_ATTRIBUTE()
			TEST_METHOD(getHighestSense1)
		{
			std::ofstream out(R"(/2122-10-biology-KSChervenkov19\statistics\Test0.json)");
			if (out.is_open())
			{
				out << R"([{"Alive":50,"Traits":[{"Sense":2.5,"Speed":3.4000000953674316},{"Sense":10.0,"Speed":1.100000023841858}]},{"Alive":25,"Traits":[{"Sense":10.0,"Speed":1.100000023841858},{"Sense":2.5,"Speed":3.4000000953674316}]}])";
				out.close();
			}

			// Arrange
			std::vector<ss::types::Cycle> cycles = ss::dal::StatisticsStore::getStatisticFrom("Test0");
			float highestSense = 0.f;
			float expected = 65.0f;

			// Act
			//highestSense = ss::pl::graph::Graph::getHighestSense(cycles);

			// Assert
			Assert::AreEqual(expected, highestSense, L"Equality != =");
		}
	};
}
// clang-format on