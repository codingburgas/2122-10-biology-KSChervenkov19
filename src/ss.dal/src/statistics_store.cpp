// clang-format off
#include "pch_dal.h"
#include "statistics_store.h"
// clang-format on

void ss::types::to_json(json &j, const ss::types::Trait &trait)
{
    j = json{{"Sense", trait.sense}, {"Speed", trait.speed}};
}

void ss::types::from_json(const json &j, ss::types::Trait &trait)
{
    j.at("Sense").get_to(trait.sense);
    j.at("Speed").get_to(trait.speed);
}

void ss::types::to_json(json &j, const ss::types::Cycle &cycle)
{
    j = json{{"Alive", cycle.lastedEntities}, {"Traits", json(cycle.traitsInfo)}};
}

void ss::types::from_json(const json &j, ss::types::Cycle &cycle)
{
    j.at("Alive").get_to(cycle.lastedEntities);
    j.at("Traits").get_to(cycle.traitsInfo);
}

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
    time_t currentTimeStamp = std::time(0);

    saveStatisticTo(std::to_string(currentTimeStamp), cycles);
}

std::vector<ss::types::Cycle> ss::dal::StatisticStore::getStatisticFrom(const std::string &fileName)
{
    std::ifstream in(fileName + ".json");
    json j;
    in >> j;

    in.close();

    return j.get<std::vector<ss::types::Cycle>>();
}