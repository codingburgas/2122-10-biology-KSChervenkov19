// clang-format off
#include "pch_types.h"
#include "statistics.h"
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