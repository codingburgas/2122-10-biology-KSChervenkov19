// clang-format off
#include "pch_bll.h"
#include "utils.h"
// clang-format on

/// Converts degrees to radians.
///
/// @param degrees The angle to be converted from.
///
/// @return Angle in radians.
double ss::bll::utils::toRadian(double degrees)
{
    return (degrees * M_PI) / 180.0;
}

/// Converts radians to degrees.
///
/// @param radians The radian to be converted from.
///
/// @return Angle in degrees.
double ss::bll::utils::toDegree(double radians)
{
    return (radians * 180.0) / M_PI;
}

float ss::bll::utils::getDistance(const types::fVec2 &p1, const types::fVec2 &p2)
{
    return sqrt(pow((p2.x - p1.x), 2) + pow(p2.y - p1.y, 2));
}

float ss::bll::utils::getAngle(const types::fVec2 &p1, const types::fVec2 &p2)
{
    return toDegree(atan2(p2.y - p1.y, p2.x - p1.x));
}
