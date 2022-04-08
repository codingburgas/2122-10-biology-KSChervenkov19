#pragma once
// clang-format off
#include "pch_types.h"
// clang-format on

// TODO: Change name to simulation_data;

namespace ss::types
{
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

struct Trait
{
    float sense;
    float speed;

    auto operator<=>(const Trait &) const = default;
};

struct Cycle
{
    int lastedEntities;
    std::vector<Trait> traitsInfo; // traitsInfo.size() == lastedEntities

    auto operator<=>(const Cycle &) const = default;
};
} // namespace ss::types
