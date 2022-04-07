// clang-format off
#include "pch_dal.h"
#include "statistics_store.h"
// clang-format on

void ss::dal::StatisticStore::saveStatisticTo(const std::string &fileName, const std::vector<ss::types::Cycle> &cycles)
{
    json j(cycles);

    std::ofstream out(fileName + ".json");
    out << j;

    out.close();
}

// Uses current time as a timestamp
void ss::dal::StatisticStore::saveStatisticTo(const std::vector<ss::types::Cycle> &cycles)
{
    time_t currentTime;
    time(&currentTime);

    saveStatisticTo(generateTimeStamp(currentTime), cycles);
}

std::vector<ss::types::Cycle> ss::dal::StatisticStore::getStatisticFrom(const std::string &fileName)
{
    return std::vector<ss::types::Cycle>();
}