#pragma once

#include "CubeCoord.h"

CubeCoord CubeCoord::CENTER(0, 0, 0);
std::vector<CubeCoord> CubeCoord::directions = { { 1, -1, 0 }, { +1, 0, -1 }, { 0, +1, -1 }, { -1, +1, 0 }, { -1, 0, +1 }, { 0, -1, +1 } };
