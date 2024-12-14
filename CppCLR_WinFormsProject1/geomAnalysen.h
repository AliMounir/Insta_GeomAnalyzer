#pragma once


#include <TopoDS_Shape.hxx>
#include <TopoDS_Face.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <TopTools_ListOfShape.hxx>


struct FaceArea {
    TopoDS_Face face;
    double area;
    int index;
    bool operator<(const FaceArea& other) const {
        return area > other.area;
    }
};

bool AreFacesParallel(const TopoDS_Face& face1, const TopoDS_Face& face2, double tolerance = 0.01);
bool IsPlaneSurface(const BRepAdaptor_Surface& surface);

std::vector<gp_Dir> GetPrincipalInertiaAxes(const TopoDS_Shape& shape);


double CheckLinSymmetry(const TopoDS_Shape& shape, const gp_Dir& mainAxisDirection, const gp_Dir& secondaryAxisDirection, int numSamplings, std::vector<gp_Lin>& samplingLines);
double CheckRotSymmetry(const TopoDS_Shape& shape, const gp_Dir& mainAxisDirection, const gp_Dir& perpAxisDirection, gp_Pnt originPoint, int numSamplings, std::vector<gp_Lin>& samplingLines);



std::vector<FaceArea> GetFacesWithArea(const TopoDS_Shape& shape);
std::vector<double> FindWandstarke(const TopoDS_Shape& shape, const gp_Dir& mainAxis, int numSamples, std::vector<gp_Lin>& samplingLines);
double ComputeDimensionAlongAxis(const TopoDS_Shape& shape, const gp_Dir& axis);

gp_Dir ComputeFaceNormal(const TopoDS_Face& face);
std::pair<TopoDS_Face, TopoDS_Face> GetTwoLargestFaces(const TopoDS_Shape& shape);
std::pair<double, double> FindLargestFacesAndCheckParallel(const TopoDS_Shape& shape);

//double checkCurvePercentage(const TopoDS_Shape& shape);
KurvenInfo checkCurvePercentage(const TopoDS_Shape& shape);

double calcUmfang(const TopoDS_Shape& shape, double t);
BiegeInfo checkBiegung(const TopoDS_Shape& shape);

double checkForProfile(const TopoDS_Shape& shape, const gp_Dir& axisDirection, int numSampling, std::vector<gp_Lin>& samplingLines);