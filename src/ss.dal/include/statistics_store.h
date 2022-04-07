#pragma once
// clang-format off
#include "pch_dal.h"
#include "statistics.h"
// clang-format on

namespace ss::dal
{
class StatisticStore
{
  private:
    static void to_json(json& j, const ss::types::Trait& trait);

    static void from_json(const json& j, ss::types::Trait& trait);

    static void to_json(json& j, const ss::types::Cycle& cycle);

    static void from_json(const json& j, ss::types::Cycle& cycle);

    static std::string generateTimeStamp(const time_t &time);

  public:
    static void saveStatisticTo(const std::string &fileName, const std::vector<ss::types::Cycle> &cycles);

    static void saveStatisticTo(const std::vector<ss::types::Cycle> &cycles);

    static std::vector<ss::types::Cycle> getStatisticFrom(const std::string &fileName);
};
} // namespace ss::dal