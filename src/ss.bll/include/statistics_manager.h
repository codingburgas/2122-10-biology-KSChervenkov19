#pragma once
// clang-format off
#include "pch_bll.h"
#include "simulation_data.h"
// clang-format on

namespace ss::bll::statistics
{
class StatisticsManager
{
  public:
    static std::vector<std::string> getStatisticsNames();

    static void saveStatistic(const std::string &fileName, const std::vector<ss::types::Cycle> &cycles);

    static void saveStatistic(const std::vector<ss::types::Cycle> &cycles);

    static std::vector<ss::types::Cycle> getStatisticFrom(const std::string &fileName);
};
} // namespace ss::bll::statistics