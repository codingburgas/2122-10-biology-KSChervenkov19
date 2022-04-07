#pragma once
// clang-format off
#include "pch_dal.h"
#include "statistics.h"
// clang-format on

namespace ss::dal
{
class StatisticStore
{
  public:
    static void saveStatisticTo(const std::string &fileName, const std::vector<ss::types::Cycle> &cycles);

    static void saveStatisticTo(const std::vector<ss::types::Cycle> &cycles);

    static std::vector<ss::types::Cycle> getStatisticFrom(const std::string &fileName);
};
} // namespace ss::dal