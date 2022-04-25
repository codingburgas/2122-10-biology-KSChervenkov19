#pragma once
// clang-format off
#include "pch_bll.h"
// clang-format on

#include "simulation_data.h"

namespace ss::bll::utils
{
double toRadian(double degrees);
double toDegree(double radians);
float getDistance(const types::fVec2& p1, const types::fVec2& p2);
} // namespace ss::bll::utils