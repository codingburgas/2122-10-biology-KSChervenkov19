// clang-format off
#include "pch_dal.h"
#include "statistics_store.h"
// clang-format on

void ss::dal::StatisticStore::to_json(json &j, const ss::types::Trait &trait)
{
}

void ss::dal::StatisticStore::from_json(const json &j, ss::types::Trait &trait)
{
}

void ss::dal::StatisticStore::to_json(json &j, const ss::types::Cycle &cycle)
{
}

void ss::dal::StatisticStore::from_json(const json &j, ss::types::Cycle &cycle)
{
}

std::string ss::dal::StatisticStore::generateTimeStamp(const time_t &time)
{
    return std::string();
}

void ss::dal::StatisticStore::saveStatisticTo(const std::string &fileName, const std::vector<ss::types::Cycle> &cycles)
{
    return;
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