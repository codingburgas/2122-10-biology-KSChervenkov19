#pragma once
// clang-format off
#include "pch_dal.h"
#include "simulation_data.h"
// clang-format on

using json = nlohmann::json;

namespace ss::dal
{
class StatisticsStore
{
  public:
    static std::vector<std::string> getStatisticsNames();

    static void saveStatisticTo(const std::string &fileName, const std::vector<ss::types::Cycle> &cycles);

    static void saveStatisticTo(const std::vector<ss::types::Cycle> &cycles);

    static std::vector<ss::types::Cycle> getStatisticFrom(const std::string &fileName);
};
} // namespace ss::dal

namespace ss::types
{
void to_json(json &j, const ss::types::Trait &trait);

void from_json(const json &j, ss::types::Trait &trait);

void to_json(json &j, const ss::types::Cycle &cycle);

void from_json(const json &j, ss::types::Cycle &cycle);
} // namespace ss::types
