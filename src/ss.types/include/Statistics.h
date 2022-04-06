#pragma once
#include "pch_types.h"
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
