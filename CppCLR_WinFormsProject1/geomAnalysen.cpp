#include "geomAnalysen.h"
#include "pch.h"

#include <BRep_Tool.hxx>
#include <BRepGProp.hxx>
#include <BRepBndLib.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <BRepAlgoAPI_Section.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <BRepAdaptor_Curve.hxx>
#include <BRepIntCurveSurface_Inter.hxx>
#include <BRepExtrema_DistShapeShape.hxx>

#include <IntCurvesFace_ShapeIntersector.hxx>
#include <Bnd_Box.hxx>
#include <GCPnts_UniformAbscissa.hxx>
#include <GCPnts_AbscissaPoint.hxx>
#include <ElCLib.hxx>

#include <GProp_GProps.hxx>
#include <GProp_PrincipalProps.hxx>
#include <GeomLProp_SLProps.hxx>
#include <Geom_Line.hxx>
#include <Geom_Curve.hxx>
#include <Geom_Circle.hxx>


#include <gp_Dir.hxx>
#include <gp_Pnt.hxx>
#include <gp_Pnt2d.hxx>
#include <gp_Vec.hxx>
#include <gp_Ax1.hxx>
#include <gp_Pln.hxx>
#include <gp_Trsf.hxx>

#include <TopoDS.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Compound.hxx>
#include <TopExp_Explorer.hxx>
#include <TopTools_ListOfShape.hxx>
#include <TopAbs_ShapeEnum.hxx>

#include <Standard_Real.hxx>
#include <Standard_Type.hxx>
#include <Precision.hxx>

#include <Geom_Surface.hxx>
#include <Geom_Plane.hxx>
#include <Geom_CylindricalSurface.hxx>
#include <Geom_ConicalSurface.hxx>
#include <Geom_SphericalSurface.hxx>
#include <Geom_ToroidalSurface.hxx>

#include <limits>
#include <numeric>
#include <map>

struct FaceArea {
    TopoDS_Face face;
    double area;
    int index;
    bool operator<(const FaceArea& other) const {
        return area > other.area;
    }
};
struct KurvenInfo {
    double planarAnteil;
    double einfKurvAnteil;
    double zwfKurvAnteil;
    double andAnteil;
};
struct BiegeInfo {
    int potentialBiegungen;
    double BiegeProbability;
    double Radius;
    double Winkel;
    double Laenge;
    double Schenkellange;
};



// Helper function to get faces with their areas
std::vector<FaceArea> GetFacesWithArea(const TopoDS_Shape& shape) {
    std::vector<FaceArea> facesWithArea;
    TopTools_IndexedMapOfShape faceMap;

    // Collect all unique faces in the shape
    TopExp::MapShapes(shape, TopAbs_FACE, faceMap);

    // Iterate over all unique faces
    for (int i = 1; i <= faceMap.Extent(); ++i) {
        TopoDS_Face face = TopoDS::Face(faceMap(i));

        // Compute the area of the face
        GProp_GProps props;
        BRepGProp::SurfaceProperties(face, props);
        double area = props.Mass();

        // Output face information
        std::cout << "Face Index: " << i << ", Address: " << &face << ", Area: " << area << " mm^2" << std::endl;

        // Store the face and its area
        facesWithArea.push_back({ face, area });
    }

    return facesWithArea;
}


// Funktion zur Überprüfung, ob eine Fläche planar ist
bool IsPlaneSurface(const BRepAdaptor_Surface& surface) {
    return surface.GetType() == GeomAbs_Plane;
}

std::vector<gp_Dir> GetPrincipalInertiaAxes(const TopoDS_Shape& shape) {
    GProp_GProps system;
    BRepGProp::VolumeProperties(shape, system);
    GProp_PrincipalProps principalProps = system.PrincipalProperties();
    std::vector<gp_Dir> principalAxes;
    principalAxes.push_back(principalProps.FirstAxisOfInertia());
    principalAxes.push_back(principalProps.SecondAxisOfInertia());
    principalAxes.push_back(principalProps.ThirdAxisOfInertia());

    return principalAxes;
}

// Function to compute the normal of a face at its midpoint
gp_Dir ComputeFaceNormal(const TopoDS_Face& face) {
    // Get the surface of the face
    Handle(Geom_Surface) surface = BRep_Tool::Surface(face);

    // Get the parameter range of the surface
    Standard_Real u1, u2, v1, v2;
    surface->Bounds(u1, u2, v1, v2);

    // Compute the midpoint parameters
    Standard_Real uMid = (u1 + u2) / 2.0;
    Standard_Real vMid = (v1 + v2) / 2.0;

    // Create an object to compute local properties at the midpoint
    GeomLProp_SLProps props(surface, uMid, vMid, 1, Precision::Confusion());

    // Check if the normal is defined at the midpoint
    if (!props.IsNormalDefined()) {
        throw std::runtime_error("Normal is not defined at the midpoint of the face.");
    }

    // Get the normal vector at the midpoint
    gp_Dir normal = props.Normal();
    return normal;
}


// Funktion zur Berechnung der Dimension des Bauteils entlang einer Achse
double ComputeDimensionAlongAxis(const TopoDS_Shape& shape, const gp_Dir& axis) {
    // Berechnung der Bounding Box des Bauteils
    Bnd_Box boundingBox;
    BRepBndLib::Add(shape, boundingBox);

    // Abrufen der Min- und Max-Punkte der Bounding Box
    Standard_Real xmin, ymin, zmin, xmax, ymax, zmax;
    boundingBox.Get(xmin, ymin, zmin, xmax, ymax, zmax);

    // Projektion der Min- und Max-Punkte auf die Achse
    gp_Vec axisVec(axis.X(), axis.Y(), axis.Z());

    gp_Pnt minPoint(xmin, ymin, zmin);
    gp_Pnt maxPoint(xmax, ymax, zmax);

    double minProjection = minPoint.XYZ().Dot(axis.XYZ());
    double maxProjection = maxPoint.XYZ().Dot(axis.XYZ());

    // Berechnung der Dimension entlang der Achse
    double dimension = std::abs(maxProjection - minProjection);

    return dimension;
}

// Function to get the axes perpendicular to the main axis
void GetPerpendicularAxes(const gp_Dir& mainAxis, gp_Dir& axisA, gp_Dir& axisB) {
    // Choose a reference direction that is not parallel to mainAxis
    gp_Dir refDir(1.0, 0.0, 0.0); // Start with global X-axis

    // Check if mainAxis is parallel to refDir
    if (mainAxis.IsParallel(refDir, Precision::Angular())) {
        // If parallel, choose a different reference direction
        refDir = gp_Dir(0.0, 1.0, 0.0); // Use global Y-axis
    }

    // Compute axisA as cross product of mainAxis and refDir
    gp_Dir tempAxis = mainAxis.Crossed(refDir);
    axisA = tempAxis;

    // Compute axisB as cross product of mainAxis and axisA
    axisB = mainAxis.Crossed(axisA);
}

std::vector<gp_Pnt> ComputeIntersectionPoints(const TopoDS_Shape& shape, const gp_Lin& line) {
    std::vector<gp_Pnt> points;

    // Create an intersector
    IntCurvesFace_ShapeIntersector intersector;
    intersector.Load(shape, Precision::Confusion());


    // Define the parameter range for the line
    Standard_Real paramMin = -1e4;
    Standard_Real paramMax = 1e4;

    // Perform the intersection
    intersector.Perform(line, paramMin, paramMax);

    if (intersector.NbPnt() > 0) {
        for (int i = 1; i <= intersector.NbPnt(); ++i) {
            gp_Pnt pnt = intersector.Pnt(i);
            points.push_back(pnt);
            //std::cout << "Schnittpunkt gefunden: (" << pnt.X() << ", " << pnt.Y() << ", " << pnt.Z() << ")" << std::endl;
        }
    }

    return points;
}


// Function to check if two faces are parallel
bool AreFacesParallel(const TopoDS_Face& face1, const TopoDS_Face& face2, double tolerance = 0.01) {
    // Compute the normals of the two faces
    gp_Dir normal1 = ComputeFaceNormal(face1);
    gp_Dir normal2 = ComputeFaceNormal(face2);

    // Calculate the angle between the two normals
    double angle = normal1.Angle(normal2);

    // Check if the angle is close to 0 or pi (180 degrees)
    if (std::abs(angle) < tolerance || std::abs(angle - M_PI) < tolerance) {
        return true; // The faces are parallel
    }
    else {
        return false; // The faces are not parallel
    }
}
double ComputeDistanceBetweenFaces(const TopoDS_Face& face1, const TopoDS_Face& face2) {
    BRepExtrema_DistShapeShape distShapeShape(face1, face2);
    if (distShapeShape.IsDone()) {
        return distShapeShape.Value();
    }
    else {
        // Handle error case
        std::cerr << "Error computing distance between faces." << std::endl;
        return std::numeric_limits<double>::infinity();
    }
}



double FindModeValue(const std::vector<double>& values, int decimalPlaces) {
    if (values.empty()) {
        throw std::runtime_error("The input vector is empty.");
    }

    // Multiplier for rounding
    double multiplier = std::pow(10.0, decimalPlaces);

    // Map to store the frequency of rounded values
    std::map<double, int> frequencyMap;

    for (double value : values) {
        // Round the value to the specified number of decimal places
        double roundedValue = std::round(value * multiplier) / multiplier;
        frequencyMap[roundedValue]++;
    }

    // Find the value with the highest frequency
    double modeValue = values[0];
    int maxCount = 0;
    for (const auto& entry : frequencyMap) {
        if (entry.second > maxCount) {
            maxCount = entry.second;
            modeValue = entry.first;
        }
    }

    return modeValue;
}

std::vector<gp_Pnt> GenerateSamplingPoints(const TopoDS_Shape& shape, const gp_Dir& axis, double length, int numSamples) {
    std::vector<gp_Pnt> samplingPoints;

    // Compute the bounding box of the shape
    Bnd_Box boundingBox;
    BRepBndLib::Add(shape, boundingBox);

    // Get the min projection along the axis
    Standard_Real xmin, ymin, zmin, xmax, ymax, zmax;
    boundingBox.Get(xmin, ymin, zmin, xmax, ymax, zmax);
    gp_Pnt minPnt(xmin, ymin, zmin);

    double minProjection = minPnt.XYZ().Dot(axis.XYZ());

    // Interval between sampling points
    double interval = length / (numSamples - 1);

    // Generate sampling points along the axis
    for (int i = 0; i < numSamples; ++i) {
        double param = minProjection + i * interval;
        gp_Pnt samplingPoint = gp_Pnt(0, 0, 0).Translated(axis.XYZ() * param);
        samplingPoints.push_back(samplingPoint);
    }

    return samplingPoints;
}




bool IsSymmetric(double distancePositive, double distanceNegative, double tolerance = 1e-6) {
    return std::abs(distancePositive - distanceNegative) < tolerance;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// SYMMETRIEN ETC.


double CheckLinSymmetry(const TopoDS_Shape& shape, const gp_Dir& mainAxisDirection, const gp_Dir& secondaryAxisDirection, int numSamplings, std::vector<gp_Lin>& samplingLines) {
    
    std::vector<double> scores;
    std::cout << "SYMMETRIE CHECK BEGINNT... "<< std::endl;

    try {
        // Step 1: Compute the center of mass as the origin point
        GProp_GProps shapeProps;
        BRepGProp::VolumeProperties(shape, shapeProps);
        gp_Pnt origin = shapeProps.CentreOfMass();

        // Step 2: Construct the plane perpendicular to the main axis, containing the secondary axis
        gp_Dir normal = mainAxisDirection;
        gp_Dir xDir = secondaryAxisDirection;
        gp_Ax1 secondaryAxis(origin, xDir);
        gp_Ax3 planeAx3(origin, normal, xDir);
        gp_Pln symmetryPlane(planeAx3);

        // Compute the bounding box of the shape
        Bnd_Box boundingBox;
        BRepBndLib::Add(shape, boundingBox);
        gp_Pnt minPnt = boundingBox.CornerMin();
        gp_Pnt maxPnt = boundingBox.CornerMax();
        //boundingBox.Get(minPnt, maxPnt);

        // Project the min and max points onto the mainAxis
        double minParam = minPnt.XYZ().Dot(xDir.XYZ());
        double maxParam = maxPnt.XYZ().Dot(xDir.XYZ());
        double exclusionPercentage = 0.01;
        double totalParamRange = maxParam - minParam;
        double adjustedMinParam = minParam + exclusionPercentage * totalParamRange;
        double adjustedMaxParam = maxParam - exclusionPercentage * totalParamRange;

        // Ensure minProjection is less than maxProjection
        if (adjustedMinParam > adjustedMaxParam) {
            std::swap(adjustedMinParam, adjustedMaxParam);
        }

        // Step 5: Generate sampling points
        double interval = (adjustedMaxParam - adjustedMinParam) / (numSamplings - 1);

        for (int i = 0; i < numSamplings; ++i) {
            double param = adjustedMinParam + i * interval;
            gp_Pnt samplingPoint = ElCLib::Value(param, secondaryAxis);

            // Step 5: Create the sampling line along the main axis direction
            gp_Lin samplingLine(samplingPoint, mainAxisDirection);
            samplingLines.push_back(samplingLine);

            // Schritt 6: Schneide die Sampling-Linie mit dem Shape
            std::vector<gp_Pnt> intersectionPoints = ComputeIntersectionPoints(shape, samplingLine);

            std::cout << "Sampling-Punkt " << i + 1 << ": (" << samplingPoint.X() << ", " << samplingPoint.Y() << ", " << samplingPoint.Z() << ")" << std::endl;
            std::cout << "Anzahl der Schnittpunkte: " << intersectionPoints.size() << std::endl;

            if (intersectionPoints.empty()) {
                // Keine Schnittpunkte, Symmetrie kann nicht bestimmt werden
                std::cout << "Keine Schnittpunkte gefunden! " << std::endl;
                //scores.push_back(0.0);
                continue;
            }
            // Initialize distances
            double maxPositiveDistance = 0.0;
            double maxNegativeDistance = 0.0;

            for (const auto& point : intersectionPoints) {
                Standard_Real paramOnLine = ElCLib::Parameter(samplingLine, point);
                double distance = std::abs(paramOnLine);
                if (paramOnLine >= 0) {
                    if (distance > maxPositiveDistance) maxPositiveDistance = distance;
                }
                else {
                    if (distance > maxNegativeDistance) maxNegativeDistance = distance;
                }
            }

            // Berechne das Verhältnis der Distanzen
            if (maxPositiveDistance > 0 && maxNegativeDistance > 0) {
                double minDistance = std::min(maxPositiveDistance, maxNegativeDistance);
                double maxDistance = std::max(maxPositiveDistance, maxNegativeDistance);
                double ratio = minDistance / maxDistance;

                // Ausgabe des Verhältnisses
                std::cout << "Verhältnis der Distanzen an Sampling-Punkt " << i + 1 << ": " << ratio << std::endl;
                scores.push_back(ratio);
            }
            else if (maxPositiveDistance == 0 && maxNegativeDistance == 0) {
                scores.push_back(1.0);
            }
            else {
                // Symmetrie kann nicht bestimmt werden
                //std::cout << "Symmetrie kann nicht bestimmt werden an Sampling-Punkt " << i + 1 << std::endl;
                scores.push_back(0.0);
            }
        }

        // Compute the average symmetry score
        if (scores.empty()) {
            throw std::runtime_error("No valid sampling points for symmetry evaluation.");
        }
        double sumScores = std::accumulate(scores.begin(), scores.end(), 0.0);
        double symmetryScore = sumScores / scores.size();
        //double symmetryScore = sumScores / numSamplings;

        std::cout << "Symmetry Score: " << symmetryScore *100 << "%" << std::endl;

        return symmetryScore;

    }
    catch (const Standard_Failure& e) {
        std::cerr << "Error in CheckLinSymmetry: " << e.GetMessageString() << std::endl;
        return 0.0;
    }
    catch (const std::exception& e) {
        std::cerr << "Error in CheckLinSymmetry: " << e.what() << std::endl;
        return 0.0;
    }
}



double CheckRotSymmetry(const TopoDS_Shape& shape, const gp_Dir& mainAxisDirection, const gp_Dir& perpAxisDirection, gp_Pnt originPoint, int numSamplings, std::vector<gp_Lin>& samplingLines) {
    try {
        // Schritt 2: Bestimme einen Anfangsvektor, der senkrecht zur Hauptachse ist
        gp_Dir initialDir = perpAxisDirection;

        // Schritt 3: Erzeuge die Sampling-Linien
        const double angleIncrement = M_PI / numSamplings;
        std::vector<double> lengths;

        for (int i = 0; i < numSamplings; ++i) {
            double angle = i * angleIncrement;

            // Rotiere initialDir um die Hauptachse
            gp_Ax1 rotationAxis(originPoint, mainAxisDirection);
            gp_Dir rotatedDir = initialDir.Rotated(rotationAxis, angle);

            // Erstelle die Sampling-Linie
            gp_Lin samplingLine(originPoint, rotatedDir);
            samplingLines.push_back(samplingLine);

            // Schnittberechnung
            std::vector<gp_Pnt> intersectionPoints = ComputeIntersectionPoints(shape, samplingLine);

            if (intersectionPoints.size() < 2) {
                // Not enough points to compute length
                lengths.push_back(0.0);
                continue;
            }

            std::vector<double> paramsOnLine;
            for (size_t idx = 0; idx < intersectionPoints.size(); ++idx) {
                gp_Pnt& pnt = intersectionPoints[idx];
                double param = ElCLib::Parameter(samplingLine, pnt);
                paramsOnLine.push_back(param);
                std::cout << "  Schnittpunkt " << idx + 1 << ": (" << pnt.X() << ", " << pnt.Y() << ", " << pnt.Z() << "), Parameter: " << param << std::endl;
            }
            std::sort(paramsOnLine.begin(), paramsOnLine.end());
            double length = std::abs(paramsOnLine.back() - paramsOnLine.front());

            std::cout << "Sampling Line " << i + 1 << " Length: " << length << std::endl;
            lengths.push_back(length);
        }

        // Step 4: Compute the mode value of the lengths
        //double minLength = *std::min_element(lengths.begin(), lengths.end());
        //double maxLength = *std::max_element(lengths.begin(), lengths.end());
        //double lengthRange = maxLength - minLength;
        //std::cout << "maxLength " << maxLength << " MINLength: " << minLength << std::endl;

        //if (lengthRange == 0.0) {
        //    // All lengths are equal, perfect symmetry
        //    if (maxLength == 0.0) {
        //        // All lengths are zero, symmetry cannot be determined
        //        return 0.0;
        //    }
        //    else {
        //        // All lengths are equal and non-zero
        //        return 1.0;
        //    }
        //}

        //double modeValue = FindModeValue(lengths, 2);
        //std::cout << "Mode Length: " << modeValue << std::endl;

        //std::sort(lengths.begin(), lengths.end());

        // Step 5: Compare each length to the MEDIAN value
        double medianValue = 0.0;
        if (lengths.size() == 0) {
            // no lengths, no symmetry
            return 0.0;
        }
        else if (lengths.size() % 2 == 1) {
            // Odd number of elements
            medianValue = lengths[lengths.size() / 2];
        }
        else {
            // Even number of elements
            medianValue = (lengths[(lengths.size() / 2) - 1] + lengths[lengths.size() / 2]) / 2.0;
        }

        std::cout << "Median Length: " << medianValue << std::endl;

        std::vector<double> scores;
        for (double length : lengths) {
            if (medianValue == 0.0) {
                scores.push_back(0.0); // If median is zero, can't compute ratio meaningfully
                continue;
            }
            double ratio = std::min(length, medianValue) / std::max(length, medianValue);
            double diff = std::abs(length - medianValue);
            double relativeDiff = diff / medianValue; // difference as a fraction of median
            double factor = 8.0; // adjust to penalize more
            double score = 1.0 / (1.0 + relativeDiff * factor);
            scores.push_back(score);
        }

        // Step 6: Compute the overall symmetry score
        double sumScores = std::accumulate(scores.begin(), scores.end(), 0.0);
        double symmetryScore = sumScores / scores.size();

        std::cout << "Rotational Symmetry: " << symmetryScore *100 << "%" << std::endl;
        return symmetryScore;

    }
    catch (const Standard_Failure& e) {
        std::cerr << "Fehler in CheckRotSymmetry: " << e.GetMessageString() << std::endl;
        return 0.0;
    }
    catch (const std::exception& e) {
        std::cerr << "Fehler in CheckRotSymmetry: " << e.what() << std::endl;
        return 0.0;
    }
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// WANDSTÄRKE - KONSTANTE DICKE




std::vector<double> FindWandstarke(const TopoDS_Shape& shape, const gp_Dir& mainAxis, int numSamples, std::vector<gp_Lin>& samplingLines) {
    std::vector<double> wandStärke;
    
    // Step 1: Determine axes perpendicular to mainAxis
    gp_Dir axisA, axisB;
    GetPerpendicularAxes(mainAxis, axisA, axisB);

    // Step 2: Compute dimensions along perpendicular axes
    double dimensionMain = ComputeDimensionAlongAxis(shape, mainAxis);
    double dimensionA = ComputeDimensionAlongAxis(shape, axisA);
    double dimensionB = ComputeDimensionAlongAxis(shape, axisB);

    // Step 3: Determine minAxis
    gp_Dir minAxis = (dimensionA < dimensionB) ? axisA : axisB;
    std::cout << "Dimension along mainAxis: " << dimensionMain << std::endl;
    std::cout << "dimensionA: " << dimensionA << std::endl;
    std::cout << "dimensionB: " << dimensionB << std::endl;

    // Compute the bounding box of the shape
    Bnd_Box boundingBox;
    BRepBndLib::Add(shape, boundingBox);
    gp_Pnt minPnt = boundingBox.CornerMin();
    gp_Pnt maxPnt = boundingBox.CornerMax();
    //boundingBox.Get(minPnt, maxPnt);

    // Project the min and max points onto the mainAxis
    double minProjection = minPnt.XYZ().Dot(mainAxis.XYZ());
    double maxProjection = maxPnt.XYZ().Dot(mainAxis.XYZ());
    double exclusionPercentage = 0.01;
    double totalParamRange = maxProjection - minProjection;
    double adjustedMinParam = minProjection + exclusionPercentage * totalParamRange;
    double adjustedMaxParam = maxProjection - exclusionPercentage * totalParamRange;

    // Ensure minProjection is less than maxProjection
    if (adjustedMinParam > adjustedMaxParam) {
        std::swap(adjustedMinParam, adjustedMaxParam);
    }

    // Step 5: Generate sampling points
    double totalLength = adjustedMaxParam - adjustedMinParam;
    double interval = totalLength / (numSamples - 1);

    std::vector<gp_Pnt> samplingPoints;

    for (int i = 0; i < numSamples; ++i) {
        double param = adjustedMinParam + i * interval;
        // Compute the point along the mainAxis
        gp_Pnt samplingPoint = gp_Pnt(0,0,0).Translated(mainAxis.XYZ() * param);
        samplingPoints.push_back(samplingPoint);

        gp_Lin line(samplingPoint, minAxis);
        samplingLines.push_back(line);
    }

    // Step 6: At each sampling point, compute minimal distances
    std::vector<double> minimalDistances;

    for (const auto& samplingPoint : samplingPoints) {
        // Define a line at the sampling point in the direction of minAxis
        gp_Lin line(samplingPoint, minAxis);

        // Intersect the line with the shape
        std::vector<gp_Pnt> intersectionPoints = ComputeIntersectionPoints(shape, line);

        // Compute the minimal non-zero distance between intersection points
        if (intersectionPoints.size() < 2) {
            //return std::numeric_limits<double>::infinity();
            continue;
        }
        // Sort points along the line direction
        std::vector<double> projections;
        for (const auto& pnt : intersectionPoints) {
            double projection = pnt.XYZ().Dot(minAxis.XYZ());
            projections.push_back(projection);
        }
        std::sort(projections.begin(), projections.end());
        // Compute distances between consecutive points
        double minimalDistance = std::numeric_limits<double>::infinity();
        for (size_t i = 1; i < projections.size(); ++i) {
            double distance = projections[i] - projections[i - 1];
            if (distance > 1e-6 && distance < minimalDistance) { // Exclude zero distances
                minimalDistance = distance;
            }
        }

        if (minimalDistance != std::numeric_limits<double>::infinity()) {
            minimalDistances.push_back(minimalDistance);
        }
    }

    // Step 7: Exclude zero values (already handled in ComputeMinimalDistance)

    // Step 8: Find the mode value among the distances
    double modeValue = FindModeValue(minimalDistances, 2);

    // Step 9: Compute the percentage of occurrence of the mode value
    int modeCount = 0;
    for (double distance : minimalDistances) {
        // Round the distance to the specified number of decimal places
        double roundedDistance = std::round(distance * std::pow(10.0, 2)) / std::pow(10.0, 2);

        if (roundedDistance == modeValue) {
            modeCount++;
        }
    }
    double percentage = (static_cast<double>(modeCount) / minimalDistances.size());

    wandStärke.push_back(modeValue);
    wandStärke.push_back(percentage);

    // Output the results
    std::cout << "Minimal wall thickness (mode value): " << modeValue << std::endl;
    std::cout << "Consistency percentage: " << percentage * 100.0 << "%" << std::endl;

    return wandStärke;
}




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ZWEI GRÖßTE FLÄCHEN PARALLELITÄT 


// Function to find the two largest planar faces in a shape
std::pair<TopoDS_Face, TopoDS_Face> GetTwoLargestFaces(const TopoDS_Shape& shape) {
    // Get the list of faces with their areas
    std::vector<FaceArea> facesWithArea = GetFacesWithArea(shape);

    // Check if there are any faces
    if (facesWithArea.size() < 2) {
        throw std::runtime_error("The shape has less than two faces.");
    }

    // Sort the faces in descending order of area
    std::sort(facesWithArea.begin(), facesWithArea.end());

    // Now, filter for planarity and find the two largest distinct planar faces
    TopoDS_Face largestFace;
    TopoDS_Face secondLargestFace;
    bool foundFirstFace = false;
    bool foundSecondFace = false;

    for (size_t i = 0; i < facesWithArea.size(); ++i) {
        TopoDS_Face face = facesWithArea[i].face;

        std::cout << "Face found at index " << i + 1 << std::endl;

        if (!foundFirstFace) {
            largestFace = face;
            foundFirstFace = true;
            std::cout << "Selected as largest face: Index " << i + 1 << std::endl;
        }
        else if (!largestFace.IsSame(face)) {
            secondLargestFace = face;
            foundSecondFace = true;
            std::cout << "Selected as second largest face: Index " << i + 1 << std::endl;
            break;
        }
    }

    if (!foundSecondFace) {
        throw std::runtime_error("Cannot find two distinct largest planar faces.");
    }

    // Return the two largest faces
    return { largestFace, secondLargestFace };
}


// Main function to find the two largest faces and check if they are parallel
double FindLargestFacesAndCheckParallel(const TopoDS_Shape& shape) {

    double score = 0.0;

    try {
        auto largestFaces = GetTwoLargestFaces(shape);
        TopoDS_Face face1 = largestFaces.first;
        TopoDS_Face face2 = largestFaces.second;

        if (!IsPlaneSurface(face1) || !IsPlaneSurface(face2)) {
            std::cout << "largest face is not planar. (for one or both)" << std::endl;
            return score = 0.0;
        }

        if (AreFacesParallel(face1, face2)) {
            score = 1.0;
            std::cout << "The two largest faces are parallel." << std::endl;
        }
        else {
            score = 0.0;
            std::cout << "The two largest faces are not parallel." << std::endl;
        }


        std::cout << "Insgesamt für die Parallelität der zwei Hauptflächen: " << score * 100 << "%" << std::endl;

        return score;
    }
    catch (const std::exception& e) {
        // Handle any errors that occurred during processing
        std::cerr << "Error: " << e.what() << std::endl;
        return false;
    }

    return score;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//KRÜMMUNGS UND KURVEN BERECHNUNG

KurvenInfo checkCurvePercentage(const TopoDS_Shape& shape) {
    std::cout << "Beginn KURVENERKENNUNG..." << std::endl;
    //std::vector<double> curvaturePercentages;
    // 
    KurvenInfo curvInfo;
    // Get all faces with their areas
    std::vector<FaceArea> facesWithArea = GetFacesWithArea(shape);

    // Initialize total area and curved area
    double totalArea = 0.0;
    double planarArea = 0.0;
    double singleCurvedArea = 0.0;
    double doubleCurvedArea = 0.0;
    double otherArea = 0.0;

    std::cout << "Amount of faces identified: " << facesWithArea.size() << std::endl;

    // Iterate over all faces
    for (size_t i = 0; i < facesWithArea.size(); ++i) {
        const FaceArea& faceArea = facesWithArea[i];
        Handle(Geom_Surface) surface = BRep_Tool::Surface(faceArea.face);
        

        // Update total area
        GProp_GProps props;
        BRepGProp::SurfaceProperties(faceArea.face, props);
        double area = props.Mass();
        totalArea += area;

        const Handle(Standard_Type)& surfaceType = surface->DynamicType();

        // Check if the face is planar
        if (surfaceType == STANDARD_TYPE(Geom_Plane)) {
            planarArea += area;
            std::cout << "planar face at index " << i + 1 << " with area " << area << " mm^2" << std::endl;
        }
        else if (surfaceType == STANDARD_TYPE(Geom_CylindricalSurface) ||
            surfaceType == STANDARD_TYPE(Geom_ConicalSurface)) {
            singleCurvedArea += area;
            std::cout << "single curved face at index " << i + 1 << " with area " << area << " mm^2" << std::endl;
        }
        else if (surfaceType == STANDARD_TYPE(Geom_SphericalSurface) ||
            surfaceType == STANDARD_TYPE(Geom_ToroidalSurface)) {
            doubleCurvedArea += area;
            std::cout << "double curved face at index " << i + 1 << " with area " << area << " mm^2" << std::endl;
        }
        else {
            otherArea += area;
            std::cout << "other face at index " << i + 1 << " with area " << area << " mm^2" << std::endl;
        }
    }

    // Compute percentage of curved area
    double planarPercentage = 0.0;
    double singleCurvedPercentage = 0.0;
    double doubleCurvedPercentage = 0.0;
    double otherPercentage = 0.0;
    if (totalArea > 0.0) {
        planarPercentage = planarArea / totalArea;
        singleCurvedPercentage = singleCurvedArea / totalArea;
        doubleCurvedPercentage = doubleCurvedArea / totalArea;
        otherPercentage = otherArea / totalArea;

    }

    curvInfo.planarAnteil = planarPercentage;
    curvInfo.einfKurvAnteil = singleCurvedPercentage;
    curvInfo.zwfKurvAnteil = doubleCurvedPercentage;
    curvInfo.andAnteil = otherPercentage;

    // Output total curvature percentage
    std::cout << "Gesamtfläche: " << totalArea << " mm²" << std::endl;
    std::cout << "Planare Fläche: " << planarArea << " mm² (" << planarPercentage * 100.0 << "%)" << std::endl;
    std::cout << "Einfach gekrümmte Fläche: " << singleCurvedArea << " mm² (" << singleCurvedPercentage * 100.0 << "%)" << std::endl;
    std::cout << "Doppelt gekrümmte Fläche: " << doubleCurvedArea << " mm² (" << doubleCurvedPercentage * 100.0 << "%)" << std::endl;
    std::cout << "Andere Fläche: " << otherArea << " mm² (" << otherPercentage * 100.0 << "%)" << std::endl;

    return curvInfo;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PROFIL ERKENNUNG


// Function to extract the end face along a specified axis
TopoDS_Face GetEndFace(const TopoDS_Shape& shape, const gp_Ax1& axis, bool positiveDirection) {
    TopoDS_Face endFace;
    double extremeValue = positiveDirection ? -std::numeric_limits<double>::max() : std::numeric_limits<double>::max();

    for (TopExp_Explorer exp(shape, TopAbs_FACE); exp.More(); exp.Next()) {
        TopoDS_Face face = TopoDS::Face(exp.Current());

        // Compute the center of mass of the face
        GProp_GProps props;
        BRepGProp::SurfaceProperties(face, props);
        gp_Pnt center = props.CentreOfMass();

        // Project the center onto the axis
        double param = ElCLib::Parameter(axis, center);
        gp_Pnt projectedPoint = ElCLib::Value(param, axis);

        // Compare the parameter along the axis
        if (positiveDirection) {
            if (param > extremeValue) {
                extremeValue = param;
                endFace = face;
            }
        }
        else {
            if (param < extremeValue) {
                extremeValue = param;
                endFace = face;
            }
        }
    }

    return endFace;
}

// Funktion zur Erzeugung einer Drahtprofile an einem bestimmten Punkt entlang der Achse
TopoDS_Wire GetCrossSection(const TopoDS_Shape& shape, const gp_Ax1& axis, double param) {
    // Erstelle eine Ebene senkrecht zur Achse an der gegebenen Position
    gp_Pnt origin = ElCLib::Value(param, axis);
    gp_Dir dir = axis.Direction();
    gp_Ax3 planeAx3(origin, dir);
    gp_Pln plane(planeAx3);

    // Berechne die Schnittfläche zwischen dem Shape und der Ebene
    BRepAlgoAPI_Section section(shape, plane, Standard_False);
    section.ComputePCurveOn1(Standard_True);
    section.Approximation(Standard_True);
    section.Build();

    if (!section.IsDone()) {
        throw std::runtime_error("Schnittberechnung ist fehlgeschlagen.");
    }

    // Sammle die resultierenden Kanten in einem Draht
    TopoDS_Compound resultCompound = TopoDS::Compound(section.Shape());
    TopoDS_Wire crossSectionWire;
    BRepBuilderAPI_MakeWire wireMaker;

    bool hasEdges = false;
    int edgeCount = 0;
    for (TopExp_Explorer exp(resultCompound, TopAbs_EDGE); exp.More(); exp.Next()) {
        TopoDS_Edge edge = TopoDS::Edge(exp.Current());

        // Überprüfe, ob die Kante gültig ist
        if (edge.IsNull()) {
            continue;
        }

        wireMaker.Add(edge);
        hasEdges = true;
        edgeCount++;
    }
    std::cout << "Anzahl der gefundenen Kanten: " << edgeCount << std::endl;

    if (!hasEdges) {
        throw std::runtime_error("Keine Schnittkanten gefunden.");
    }

   ///////////// crossSectionWire = wireMaker.Wire();

    if (crossSectionWire.IsNull()) {
        throw std::runtime_error("Fehler beim Erstellen des Drahtprofils.");
    }

    return crossSectionWire;
}


// Vergleich der Profile durch Sampling von Punkten entlang der Kanten
double CompareProfiles(const TopoDS_Wire& wire1, const TopoDS_Wire& wire2) {
    // Sample points entlang wire1
    std::vector<gp_Pnt> points1;
    for (TopExp_Explorer exp(wire1, TopAbs_EDGE); exp.More(); exp.Next()) {
        TopoDS_Edge edge = TopoDS::Edge(exp.Current());
        if (edge.IsNull()) {
            continue;
        }
        BRepAdaptor_Curve curve(edge);
        double first = curve.FirstParameter();
        double last = curve.LastParameter();
        GCPnts_UniformAbscissa abscissa(curve, 10); // 10 Punkte pro Kante

        for (int i = 1; i <= abscissa.NbPoints(); ++i) {
            double param = abscissa.Parameter(i);
            gp_Pnt pnt = curve.Value(param);
            points1.push_back(pnt);
        }
    }

    // Sample points entlang wire2
    std::vector<gp_Pnt> points2;
    for (TopExp_Explorer exp(wire2, TopAbs_EDGE); exp.More(); exp.Next()) {
        TopoDS_Edge edge = TopoDS::Edge(exp.Current());
        if (edge.IsNull()) {
            continue;
        }
        BRepAdaptor_Curve curve(edge);
        double first = curve.FirstParameter();
        double last = curve.LastParameter();
        GCPnts_UniformAbscissa abscissa(curve, 10); // 10 Punkte pro Kante

        for (int i = 1; i <= abscissa.NbPoints(); ++i) {
            double param = abscissa.Parameter(i);
            gp_Pnt pnt = curve.Value(param);
            points2.push_back(pnt);
        }
    }

    // Sicherstellen, dass beide Profile Punkte zum Vergleich haben
    size_t size1 = points1.size();
    size_t size2 = points2.size();
    size_t minSize = std::min(size1, size2);
    if (minSize == 0) {
        throw std::runtime_error("Keine Punkte zum Vergleich.");
    }

    // Vergleich der Punkte
    double totalDistance = 0.0;
    for (size_t i = 0; i < minSize; ++i) {
        totalDistance += points1[i].Distance(points2[i]);
    }

    // Durchschnittliche Distanz berechnen
    double averageDistance = totalDistance / minSize;
    return averageDistance;
}



// Main function to check for profile consistency
double checkForProfile(const TopoDS_Shape& shape, const gp_Dir& mainAxisDirection, int numSamplings, std::vector<gp_Lin>& samplingLines) {
    std::cout << "Beginn PROFILERKENNUNG..." << std::endl;
    try {
        // Berechne das Zentrum der Masse als Referenzpunkt
        GProp_GProps shapeProps;
        BRepGProp::VolumeProperties(shape, shapeProps);
        gp_Pnt origin = shapeProps.CentreOfMass();

        // Erhalte die Bounding Box des Bauteils
        Bnd_Box boundingBox;
        BRepBndLib::Add(shape, boundingBox);
        boundingBox.SetGap(0);

        gp_Pnt minPnt = boundingBox.CornerMin();
        gp_Pnt maxPnt = boundingBox.CornerMax();

        // Projiziere die Min- und Max-Punkte auf die Hauptachse
        gp_Ax1 mainAxis(origin, mainAxisDirection);
        Standard_Real minParam = ElCLib::Parameter(mainAxis, minPnt);
        Standard_Real maxParam = ElCLib::Parameter(mainAxis, maxPnt);

        // Bestimme den Startpunkt entlang der Hauptachse
        Standard_Real startParam = std::min(minParam, maxParam);

        // Berechne den Startpunkt auf der Hauptachse
        gp_Pnt startPoint = ElCLib::Value(startParam, mainAxis);

        // Erstelle eine Ebene senkrecht zur Hauptachse am Startpunkt
        gp_Pln frontPlane(startPoint, mainAxisDirection);

        // Sammlung von Kanten an der Stirnfläche
        std::vector<TopoDS_Edge> frontEdges;

        // Durchsuche alle Flächen des Bauteils
        for (TopExp_Explorer faceExplorer(shape, TopAbs_FACE); faceExplorer.More(); faceExplorer.Next()) {
            TopoDS_Face face = TopoDS::Face(faceExplorer.Current());

            // Prüfe, ob die Fläche auf der Stirnfläche liegt
            BRepAdaptor_Surface surface(face);
            if (surface.GetType() == GeomAbs_Plane) {
                gp_Pln plane = surface.Plane();

                // Berechne den Abstand der Fläche zur frontPlane
                double distance = plane.Location().Distance(startPoint);
                if (distance < 1e-3) {
                    // Die Fläche liegt auf der Stirnfläche
                    // Extrahiere die Kanten
                    for (TopExp_Explorer edgeExplorer(face, TopAbs_EDGE); edgeExplorer.More(); edgeExplorer.Next()) {
                        TopoDS_Edge edge = TopoDS::Edge(edgeExplorer.Current());
                        frontEdges.push_back(edge);
                    }
                }
            }
        }

        if (frontEdges.empty()) {
            std::cout << "Keine Stirnflächenkanten gefunden." << std::endl;
            throw std::runtime_error("Keine Stirnflächenkanten gefunden.");
        }

        // Sammlung von Punkten auf den Umrissen
        std::vector<gp_Pnt> samplingPoints;

        // Anzahl der Punkte pro Kante
        int pointsPerEdge = numSamplings / static_cast<int>(frontEdges.size());
        if (pointsPerEdge == 0) pointsPerEdge = 1;

        std::cout << "frontal plane edge points found: " << pointsPerEdge << std::endl;

        double offsetDistance = 0.1;

        // Für jede Kante Punkte generieren
        for (const auto& edge : frontEdges) {
            // Extrahiere die Kurve der Kante
            Standard_Real firstParam, lastParam;
            Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, firstParam, lastParam);
            if (curve.IsNull()) continue;

            TopAbs_Orientation edgeOrientation = edge.Orientation();
            bool isOuterEdge = (edgeOrientation == TopAbs_FORWARD);

            // Anzahl der Sampling-Punkte entlang dieser Kante
            int numPoints = pointsPerEdge;

            // Berechne die Parameterinkremente
            Standard_Real paramIncrement = (lastParam - firstParam) / numPoints;

            for (int i = 0; i <= numPoints; ++i) {
                Standard_Real param = firstParam + i * paramIncrement;
                gp_Pnt point;
                gp_Vec tangent;
                curve->D1(param, point, tangent);

                gp_Dir planeNormal = mainAxisDirection;
                gp_Dir normal = planeNormal.Crossed(tangent);
                //normal = normal.Normalized();

                double sign = (isOuterEdge) ? 1.0 : -1.0;
                gp_Vec offsetVec = gp_Vec(normal) * (sign * offsetDistance);
                gp_Pnt offsetPoint = point.Translated(offsetVec);

                samplingPoints.push_back(offsetPoint);
            }
        }

        // Erstelle für jeden Sampling-Punkt eine Linie entlang der Hauptachse
        for (const auto& point : samplingPoints) {
            gp_Lin line(point, mainAxisDirection);
            samplingLines.push_back(line);
            std::cout << "testpunkt an stirnfläche mit offset (" << point.X() << "/" << point.Y() << "/" << point.Z() << std::endl;
        }

        // Berechne die Schnittpunkte und zähle die Linien, die das Bauteil schneiden
        double linesWithIntersections = 0;

        for (size_t i = 0; i < samplingLines.size(); ++i) {
            const gp_Lin& line = samplingLines[i];

            // Berechne die Schnittpunkte
            std::vector<gp_Pnt> intersectionPoints = ComputeIntersectionPoints(shape, line);

            // Wenn es Schnittpunkte gibt, erhöhe den Zähler
            if (!intersectionPoints.empty()) {
                linesWithIntersections += 1.5;
            }
            std::cout << "Sampling-Linie " << i + 1 << ": ";
            if (!intersectionPoints.empty()) {
                std::cout << "Schnittpunkte gefunden." << std::endl;
            }
            else {
                std::cout << "Keine Schnittpunkte." << std::endl;
            }
        }

        // Berechne den Prozentsatz
        double profileConsistency = 1 - ((linesWithIntersections) / samplingLines.size());

        return profileConsistency;

    }
    catch (const Standard_Failure& e) {
        std::cerr << "Fehler in checkForProfile: " << e.GetMessageString() << std::endl;
        return 0.0;
    }
    catch (const std::exception& e) {
        std::cerr << "Fehler in checkForProfile: " << e.what() << std::endl;
        return 0.0;
    }
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// BIEGUNG

double calcUmfang(const TopoDS_Shape& shape, double t) {
    double totalLength = 0.0;
    double eps = 0.9;
    int edgeCount = 0;

    TopExp_Explorer edgeExplorer(shape, TopAbs_EDGE);
    for (; edgeExplorer.More(); edgeExplorer.Next()) {
        TopoDS_Edge edge = TopoDS::Edge(edgeExplorer.Current());

        edgeCount++;
        std::cout << "Processing Edge #" << edgeCount << std::endl;

        Standard_Real first, last;
        Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, first, last);
        if (curve.IsNull()) {
            std::cout << "Edge #" << edgeCount << " has no associated curve. Skipping..." << std::endl;

            continue;
        }

        GeomAdaptor_Curve adaptorCurve(curve, first, last);
        Standard_Real length = GCPnts_AbscissaPoint::Length(adaptorCurve);

        std::cout << "Edge #" << edgeCount << ": Computed length = " << length << std::endl;


        if ((std::min(length, t)/(std::max(length, t)) < eps)) {
            totalLength += length;
        }
    }

    // Da jede Kante ein counterpart auf der anderen seite hat, hier durch 2 (und dann nochmal durch 2 wegen non-uniquness beim explorer)
    double umfang = totalLength / 8;
    return umfang;
}


BiegeInfo checkBiegung(const TopoDS_Shape& shape) {
    std::cout << "checkBiegung aufgerufen..." << std::endl;
    BiegeInfo info;
    info.potentialBiegungen = 0;
    info.BiegeProbability = 0.0;
    info.Radius = 0.0;
    info.Winkel = 0.0;
    info.Laenge = 0.0;
    double score = 0.0;

    std::vector<FaceArea> faces = GetFacesWithArea(shape);

    // Hilfsfunktion für Kantenlänge
    auto edgeLength = [](const TopoDS_Edge& edge) {
        Standard_Real f, l;
        Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, f, l);
        if (curve.IsNull()) {
            std::cout << "Edge ohne Kurve, Länge = 0.0" << std::endl;
            return 0.0;
        }
        GeomAdaptor_Curve adaptorCurve(curve, f, l);
        Standard_Real length = 0.0;
        try {
            length = GCPnts_AbscissaPoint::Length(adaptorCurve, f, l);
        }
        catch (...) {
            std::cout << "Fehler bei Längenberechnung dieser Edge, Länge = 0.0" << std::endl;
            length = 0.0;
        }
        std::cout << "Edge-Laenge berechnet: " << length << std::endl;
        return length;
    };

    // 1) Alle zylindrischen Flächen sammeln
    std::vector<std::pair<TopoDS_Face, BRepAdaptor_Surface>> cylinderFaces;
    int faceIndex = 0;
    for (auto& fa : faces) {
        faceIndex++;
        BRepAdaptor_Surface surf(fa.face, Standard_True);
        if (surf.GetType() == GeomAbs_Cylinder) {
            // Check ob nicht voll 2π
            Standard_Real uMin = surf.FirstUParameter();
            Standard_Real uMax = surf.LastUParameter();
            Standard_Real uRange = uMax - uMin;
            double twoPi = 2.0 * M_PI;
            // Wenn voll 2π dann ignorieren => hier aber wollen wir erst nachher filtern
            // Also jetzt noch nicht rausfiltern, nur speichern.
            cylinderFaces.push_back(std::make_pair(fa.face, surf));
            std::cout << "Face #" << faceIndex << ": Ist Zylindrisch. uRange=" << uRange << std::endl;
        }
        else {
            std::cout << "Face #" << faceIndex << ": Nicht zylindrisch, ignoriere." << std::endl;
        }
    }

    // 2) Geschlossene Zylinder entfernen (uRange ~ 2π)
    std::vector<std::pair<TopoDS_Face, BRepAdaptor_Surface>> filteredCylinders;
    for (auto& cpair : cylinderFaces) {
        auto& face = cpair.first;
        auto& surf = cpair.second;
        Standard_Real uMin = surf.FirstUParameter();
        Standard_Real uMax = surf.LastUParameter();
        Standard_Real uRange = uMax - uMin;
        if (std::fabs(uRange - (2.0 * M_PI)) < 1e-6 || std::fabs(uRange - (M_PI)) < 1e-6) {
            std::cout << "Zylindrische Face mit vollem 2π/π Umfang gefunden, ignorieren." << std::endl;
            continue;
        }
        filteredCylinders.push_back(cpair);
    }

    // Jetzt ist die Anzahl der remaining Zylindrischen Flächen unsere potentialBiegungen
    info.potentialBiegungen = (int)filteredCylinders.size();
    std::cout << "Anzahl potentieller Biegungen (nach Filter): " << info.potentialBiegungen << std::endl;
    //score = score + std::max(0.0, 1.0 - 0.015 * info.potentialBiegungen);

    if (filteredCylinders.empty()) {
        std::cout << "Keine potentiellen Biegungen gefunden, Probability=0" << std::endl;
        score = 0.0;
        return info; // Probability=0 etc. sind schon gesetzt
    }
    
    // 3) Aus den verbleibenden zylindrischen Flächen jene mit größtem Betrag von vMin oder vMax wählen
    // Wir bestimmen für jede Face max(|vMin|,|vMax|) und wählen die größte.
    double maxVValue = -1.0;
    int bestIndex = -1;
    for (int i = 0; i < (int)filteredCylinders.size(); i++) {
        auto& surf = filteredCylinders[i].second;
        Standard_Real vMin = surf.FirstVParameter();
        Standard_Real vMax = surf.LastVParameter();
        double candidateVal = std::max(std::fabs((double)vMin), std::fabs((double)vMax));
        std::cout << "Face " << i << ": vMin=" << vMin << ", vMax=" << vMax << ", candidateVal=" << candidateVal << std::endl;
        if (candidateVal >= maxVValue) {
            maxVValue = candidateVal;
            bestIndex = i;
        }
    }//SEHR WICHTIG HIER. candidateVal >= maxVValue wird der letzte größte Wert gewählt => innenRadius --- mit candidateVal >= maxVValue wird der erste größte Wert gewählt => außenradius
    
    if (bestIndex == -1) {
        std::cout << "Konnte keine Face nach vMin/vMax-Kriterium wählen. Probability=0" << std::endl;
        score = 0.0;
        return info;
    }
    score++;

    // Unsere ausgewählte Face:
    TopoDS_Face chosenFace = filteredCylinders[bestIndex].first;
    BRepAdaptor_Surface chosenSurf = filteredCylinders[bestIndex].second;
    Standard_Real uMin = chosenSurf.FirstUParameter();
    Standard_Real uMax = chosenSurf.LastUParameter();
    Standard_Real vMin = chosenSurf.FirstVParameter();
    Standard_Real vMax = chosenSurf.LastVParameter();

    // 4) Edges der chosenFace untersuchen
    TopTools_IndexedMapOfShape wires;
    TopExp::MapShapes(chosenFace, TopAbs_WIRE, wires);
    if (wires.Extent() == 0) {
        std::cout << "Gewählte Face hat keine Wires, Probability=0." << std::endl;
        score = 0.0;
        return info; // Probability=0
    }
    TopoDS_Wire outerWire = TopoDS::Wire(wires(1));

    std::vector<TopoDS_Edge> edges;
    for (TopExp_Explorer eExp(outerWire, TopAbs_EDGE); eExp.More(); eExp.Next()) {
        edges.push_back(TopoDS::Edge(eExp.Current()));
    }

    std::cout << "Gewählte Face Edges-Anzahl: " << edges.size() << std::endl;
    score = score + std::max(0.0, 1.0 - 0.02 * edges.size());

    // Mindestens 2 gerade und mindestens 2 gekrümmte Edges
    std::vector<TopoDS_Edge> straightEdges;
    std::vector<TopoDS_Edge> curvedEdges;

    for (auto& ed : edges) {
        Standard_Real f, l;
        Handle(Geom_Curve) curve = BRep_Tool::Curve(ed, f, l);
        if (curve.IsNull()) continue;
        Handle(Geom_Line) line = Handle(Geom_Line)::DownCast(curve);
        Handle(Geom_Circle) circle = Handle(Geom_Circle)::DownCast(curve);

        if (!line.IsNull()) {
            straightEdges.push_back(ed);
        }
        else if (!circle.IsNull()) {
            curvedEdges.push_back(ed);
        }
    }

    std::cout << "Anzahl gerade Edges: " << straightEdges.size() << ", Anzahl gekrümmte Edges: " << curvedEdges.size() << std::endl;
    if (straightEdges.size() < 2 || curvedEdges.size() < 2) {
        std::cout << "Kriterium nicht erfüllt (mind. 2 gerade, 2 gekrümmte). Probability=0." << std::endl;
        score = 0.0;
        return info;
    }

    // 5) info.Laenge = max(|vMin|, |vMax|)
    double laenge = std::max(std::fabs((double)vMin), std::fabs((double)vMax));

    // 6) Winkel = (uMax - uMin) in Grad
    double winkelRad = std::fabs((double)(uMax - uMin));
    double winkelDeg = winkelRad * 180.0 / M_PI;

    // 7) Radius = surf.Cylinder().Radius()
    double radius = chosenSurf.Cylinder().Radius();

    // Da wir mindestens eine gültige Face gefunden haben:
    // BiegeProbability=1.0 (wir haben ja potentialBiegungen = size gefiltert)
    info.BiegeProbability = score / 2;
    info.Radius = radius;
    info.Winkel = winkelDeg;
    info.Laenge = laenge;

    std::cout << "Ergebnisse:" << std::endl;
    std::cout << "potentialBiegungen: " << info.potentialBiegungen << std::endl;
    std::cout << "BiegeProbability: " << info.BiegeProbability << std::endl;
    std::cout << "Radius: " << info.Radius << std::endl;
    std::cout << "Winkel (deg): " << info.Winkel << std::endl;
    std::cout << "Laenge: " << info.Laenge << std::endl;

    return info;
}