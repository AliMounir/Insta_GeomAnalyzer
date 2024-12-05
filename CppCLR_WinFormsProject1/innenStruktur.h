#pragma once
//#include "pch.h"
#include <set>

double checkHohlraum(const TopoDS_Shape& shape, int numSamplings, const gp_Pnt& startPoint, std::vector<gp_Pnt>& visitedPoints, std::vector<gp_Lin>& samplingLines, int recursionDepth = 0);