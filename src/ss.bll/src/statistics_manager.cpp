// clang-format off
#include "pch_bll.h"
#include "statistics_manager.h"
#include "statistics_store.h"
// clang-format on

std::vector<std::string> ss::bll::statistics::StatisticManager::getStatisticsNames()
{
	return ss::dal::StatisticStore::getStatisticsNames();
}

void ss::bll::statistics::StatisticManager::saveStatistic(const std::string& fileName,
	const std::vector<ss::types::Cycle>& cycles)
{
	ss::dal::StatisticStore::saveStatisticTo(fileName, cycles);
}

void ss::bll::statistics::StatisticManager::saveStatistic(const std::vector<ss::types::Cycle>& cycles)
{
	ss::dal::StatisticStore::saveStatisticTo(cycles);
}

std::vector<ss::types::Cycle> ss::bll::statistics::StatisticManager::getStatisticFrom(const std::string& fileName)
{
	return ss::dal::StatisticStore::getStatisticFrom(fileName);
}