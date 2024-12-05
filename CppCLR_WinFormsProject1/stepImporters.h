#pragma once
#include "GeomInfo.h"
#include "pch.h"


#include <TopoDS_Shape.hxx>


TopoDS_Shape ImportSTEP(const Standard_CString filename);
STEPInfo InspectShape(const TopoDS_Shape& shape);

