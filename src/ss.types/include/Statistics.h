#pragma once
// clang-format off
#include "pch_types.h"
// clang-format on
using json = nlohmann::json;

namespace ss::types
{
struct Trait
{
    float sense;
    float speed;
};

struct Cycle
{
    int lastedEntities;
    std::vector<Trait> traitsInfo; // traitsInfo.size() == lastedEntities
};

void to_json(json &j, const ss::types::Trait &trait);

void from_json(const json &j, ss::types::Trait &trait);

void to_json(json &j, const ss::types::Cycle &cycle);

void from_json(const json &j, ss::types::Cycle &cycle);
} // namespace ss::types
