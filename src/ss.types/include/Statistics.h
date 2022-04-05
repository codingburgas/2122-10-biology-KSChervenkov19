#pragma once
#include "pch.h"

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