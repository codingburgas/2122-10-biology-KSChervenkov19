#pragma once
// clang-format off
#include "pch_types.h"
// clang-format on

namespace ss::types
{
/// A struct holding the data for a simulation.
struct SimulationInfo
{
    int width;
    int height;
    int cyclesCount;
    int foodCount;
    int startingEntityCount;
    float initialSpeed;
    float initialSense;
};
/// A struct holding the data for a trait.
struct Trait
{
    float sense;
    float speed;

    auto operator<=>(const Trait &) const = default;
};
/// A struct holding the data for a cycle.
struct Cycle
{
    int lastedEntities;
    std::vector<Trait> traitsInfo; // traitsInfo.size() == lastedEntities

    auto operator<=>(const Cycle &) const = default;
};
} // namespace ss::types
