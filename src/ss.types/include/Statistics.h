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
}
