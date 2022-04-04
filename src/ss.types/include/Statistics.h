#pragma once
#include "pch.h"

struct Trait
{
	float speed;
	float sense;
};

struct Cycle
{
	int lastedEntities;
	std::vector<Trait> traitsInfo; // traitsInfo.size() == lastedEntities
};