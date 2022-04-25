#pragma once
// clang-format off
#include "pch_types.h"
// clang-format on

namespace ss::types
{
struct fVec2
{
    float x;
    float y;

    auto operator<=>(const fVec2 &) const = default;
};

/// A struct holding the data for a trait.
struct Trait
{
    float sense;
    float speed;

    auto operator<=>(const Trait &) const = default;
};

/// A struct holding the data for a simulation.
struct SimulationInfo
{
    size_t worldSize;
    size_t cyclesCount;
    size_t foodCount;
    size_t startingEntityCount;
	Trait initialTraits = {1.0f, 1.0f};
};

/// A struct holding the data for a cycle.
struct Cycle
{
    int lastedEntities;
    std::vector<Trait> traitsInfo; // traitsInfo.size() == lastedEntities

    auto operator<=>(const Cycle &) const = default;
};
} // namespace ss::types
