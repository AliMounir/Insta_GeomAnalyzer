#pragma once
#include "GeomInfo.h"
#include <TopoDS_Shape.hxx>

GeomInfo getBasicInfo(const TopoDS_Shape& shape);
TopoDS_Shape CenterShape(const TopoDS_Shape& shape);