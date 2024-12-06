#include "innenStruktur.h"
#include "pch.h"

#include <IntCurvesFace_ShapeIntersector.hxx>
#include <BRepClass3d_SolidClassifier.hxx>
#include <TopoDS_Solid.hxx>
#include <vector>
#include <TopoDS_Shape.hxx>
#include <iostream>
#include <set>
#include <gp_Lin.hxx>
#include <gp_Dir.hxx>
#include <gp_Pnt.hxx>
#include <cmath>


bool IsPointInsideShape(const TopoDS_Shape& shape, const gp_Pnt& point) {
    // Erstelle einen SolidClassifier
    BRepClass3d_SolidClassifier classifier(shape);

    // Führe die Klassifizierung durch
    classifier.Perform(point, Precision::Confusion());

    // Überprüfe das Ergebnis
    TopAbs_State state = classifier.State();

    // Wenn der Punkt innerhalb oder auf der Oberfläche liegt, gilt er als innen
    return (state == TopAbs_IN || state == TopAbs_ON);
}


std::vector<gp_Pnt> ComputeIntersectionPointsss(const TopoDS_Shape& shape, const gp_Lin& line) {
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



std::vector<gp_Lin> CreateSunRays(const gp_Pnt& origin, int numSamplings) {
    std::vector<gp_Lin> sunRays;

    // Erzeuge gleichmäßig verteilte Richtungen auf der Einheitssphäre
    int numTheta = static_cast<int>(std::sqrt(numSamplings));
    int numPhi = numSamplings / numTheta;

    for (int i = 0; i < numTheta; ++i) {
        double theta = M_PI * (static_cast<double>(i) + 0.5) / numTheta;
        for (int j = 0; j < numPhi; ++j) {
            double phi = 2 * M_PI * static_cast<double>(j) / numPhi;

            // Berechne die Richtungskoordinaten
            double x = std::sin(theta) * std::cos(phi);
            double y = std::sin(theta) * std::sin(phi);
            double z = std::cos(theta);

            gp_Dir direction(x, y, z);
            gp_Lin line(origin, direction);
            sunRays.push_back(line);
        }
    }

    return sunRays;
}




double checkHohlraum(const TopoDS_Shape& shape, int numSamplings, const gp_Pnt& startPoint, std::vector<gp_Pnt>& visitedPoints, std::vector<gp_Lin>& samplingLines, int recursionDepth = 0) {
    
    
    const int maxRecursionDepth = 8;
    if (recursionDepth > maxRecursionDepth) {
        return 1.0;
    }
    recursionDepth++;
    // Überprüfe, ob der Startpunkt bereits besucht wurde, um Zyklen zu vermeiden
    for (const auto& p : visitedPoints) {
        if (startPoint.IsEqual(p, Precision::Confusion())) {
            // Punkt wurde bereits besucht
            recursionDepth--;
            return 0.0;
        }
    }

    // Füge den aktuellen Startpunkt zur Liste der besuchten Punkte hinzu
    visitedPoints.push_back(startPoint);

    // Überprüfe, ob der Startpunkt innerhalb des Bauteils liegt
    bool isInside = IsPointInsideShape(shape, startPoint);
    
    std::string insideOutside = isInside ? "innerhalb" : "außerhalb";
    std::cout << "Untersuchungspunkt " << recursionDepth << ": (" << startPoint.X() << ", " << startPoint.Y() << ", " << startPoint.Z() << ") " << "liegt " << insideOutside << " des Bauteils." << std::endl;


    // Erstelle die Sampling-Linien ("Sonne")
    std::vector<gp_Lin> sunRays = CreateSunRays(startPoint, numSamplings);

    int lineIndex = 0;
    double totalScore = 0.0;
    int validLines = 0;

    bool highFragment = false;
    int noIntersectCount = 0;

    for (const auto& line : sunRays) {
        lineIndex++;
        samplingLines.push_back(line);
        // Berechne die Schnittpunkte
        std::vector<gp_Pnt> intersectionPoints = ComputeIntersectionPointsss(shape, line);

        // Sortiere die Schnittpunkte nach Distanz vom Startpunkt
        std::sort(intersectionPoints.begin(), intersectionPoints.end(),
            [&startPoint](const gp_Pnt& a, const gp_Pnt& b) {
                return startPoint.Distance(a) < startPoint.Distance(b);
            });

        int numIntersections = static_cast<int>(intersectionPoints.size());
        std::cout << "Sampling-Linie " << lineIndex << ": " << numIntersections << " Schnittpunkte gefunden." << std::endl;

        if (numIntersections == 0) {
            noIntersectCount++;
        }
        // Falls mehr als 20 Schnittpunkte gefunden wurden, abbrechen und 1.0 zurückgeben
        if (numIntersections > 20) {
            //min. 15% der Rays können raus gehen
            highFragment = true;
            break;
            //return 1.0;
        }

        if (isInside) {
            // Startpunkt ist innerhalb des Bauteils
            if (numIntersections == 0 || numIntersections == 1 || numIntersections == 2) {
                // Keine internen Hohlräume in dieser Richtung
                totalScore += 0.0;
                validLines++;
            }
            else if (numIntersections > 1 && numIntersections % 2 == 0) {
                // gerade Anzahl von Schnittpunkten (>1), Hohlraum gefunden
                // Nimm die zwei nächsten Schnittpunkte
                gp_Pnt firstPoint = intersectionPoints[0];
                gp_Pnt secondPoint = intersectionPoints[1];

                // Berechne den Mittelpunkt
                gp_Pnt midPoint((firstPoint.XYZ() + secondPoint.XYZ()) * 0.5);

                // Rekursiver Aufruf
                double innerScore = checkHohlraum(shape, numSamplings, midPoint, visitedPoints, samplingLines);
                totalScore += innerScore;
                validLines++;
            }
        }
        else {
            // Startpunkt ist außerhalb des Bauteils (in einem Hohlraum)
            if (numIntersections == 0 || numIntersections == 1 || numIntersections == 2) {
                // kein Hohlraum ODER ist nicht geschlossen
                totalScore += 0.0;
                validLines++;
            }
            if (numIntersections == 3 || numIntersections == 4) {
                // es gibt einen hohlraum und wir befinden uns gerade in dem (3 als face fail safe)
                totalScore += 1.0;
                validLines++;
            }
            else if ((numIntersections % 2 == 0 && numIntersections > 4) || (numIntersections % 2 == 1 && numIntersections > 3)) {
                // Gerade Anzahl von Schnittpunkten (>4), "anderer" geschlossener Hohlraum gefunden
                // Nimm den zweiten und dritten Schnittpunkt
                gp_Pnt secondPoint = intersectionPoints[1];
                gp_Pnt thirdPoint = intersectionPoints[2];

                // Berechne den Mittelpunkt
                gp_Pnt midPoint((secondPoint.XYZ() + thirdPoint.XYZ()) * 0.5);

                // Rekursiver Aufruf
                double innerScore = checkHohlraum(shape, numSamplings, midPoint, visitedPoints, samplingLines);
                totalScore += innerScore;
                validLines++;
            }
        }
    }

    // After processing all lines or breaking out
    if (highFragment) {
        // Check the 15% condition
        double fractionNoIntersect = (double)noIntersectCount / (double)numSamplings;
        if (fractionNoIntersect >= 0.15) {
            return 0.0;
        }
        else {
            return 1.0;
        }
    }

    // Verhindere Division durch Null
    if (validLines == 0) {
        return 0.0;
    }

    // Durchschnittlichen Score berechnen
    double averageScore = totalScore / validLines;
    return averageScore;
}