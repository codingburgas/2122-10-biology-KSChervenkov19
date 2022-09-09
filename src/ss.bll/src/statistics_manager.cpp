// clang-format off
#include "pch_bll.h"
#include "statistics_manager.h"
#include "statistics_store.h"
// clang-format on

/// Getter files names' function.
///
/// The function searches for a file with a .json extension and gets its name.
/// @return An array of names
std::vector<std::string> ss::bll::statistics::StatisticsManager::getStatisticsNames()
{
    return ss::dal::StatisticsStore::getStatisticsNames();
}

/// Save generation data into a .json file.
///
/// The functions saves a generation data into a .json file using a user chosen name as a name of the file.
/// @param fileName A file name which the data will be saved into.
/// @param cycles A generation data.
void ss::bll::statistics::StatisticsManager::saveStatistic(const std::string &fileName,
                                                           const std::vector<ss::types::Cycle> &cycles)
{
    ss::dal::StatisticsStore::saveStatisticTo(fileName, cycles);
}

/// Save generation data into a .json file.
///
/// The functions saves a generation data into a .json file using a timestamp as a name of the file.
/// @param cycles A generation data.
void ss::bll::statistics::StatisticsManager::saveStatistic(const std::vector<ss::types::Cycle> &cycles)
{
    ss::dal::StatisticsStore::saveStatisticTo(cycles);
}

/// Getter a statistic data function.
///
/// The function gets the generation data from saved in the .json file.
/// @param fileName The file which holds needed generation data.
/// @return An array of cycles which is a generation.
std::vector<ss::types::Cycle> ss::bll::statistics::StatisticsManager::getStatisticFrom(const std::string &fileName)
{
    return ss::dal::StatisticsStore::getStatisticFrom(fileName);
}