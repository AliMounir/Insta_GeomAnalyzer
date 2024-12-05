#pragma once
#include "pch.h"


#include <TopoDS_Shape.hxx>


TopoDS_Shape ImportSTEP(const Standard_CString filename);
void InspectShape(const TopoDS_Shape& shape);

