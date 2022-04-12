// clang-format off
#include "pch_bll.h"
#include "statistics_manager.h"
#include "statistics_store.h"
// clang-format on

std::vector<std::string> ss::bll::statistics::StatisticsManager::getStatisticsNames()
{
    return ss::dal::StatisticsStore::getStatisticsNames();
}

void ss::bll::statistics::StatisticsManager::saveStatistic(const std::string &fileName,
                                                           const std::vector<ss::types::Cycle> &cycles)
{
    ss::dal::StatisticsStore::saveStatisticTo(fileName, cycles);
}

void ss::bll::statistics::StatisticsManager::saveStatistic(const std::vector<ss::types::Cycle> &cycles)
{
    ss::dal::StatisticsStore::saveStatisticTo(cycles);
}

std::vector<ss::types::Cycle> ss::bll::statistics::StatisticsManager::getStatisticFrom(const std::string &fileName)
{
    return ss::dal::StatisticsStore::getStatisticFrom(fileName);
}