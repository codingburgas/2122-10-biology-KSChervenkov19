#pragma once
#include "pch.h"

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
