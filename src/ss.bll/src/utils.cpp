// clang-format off
#include "pch_bll.h"
#include "utils.h"
// clang-format on

double ss::bll::utils::toRadian(double degrees)
{
	return (degrees * M_PI) / 180.0;
}

double ss::bll::utils::toDegree(double radians)
{
	return (radians * 180.0) / M_PI;
}
