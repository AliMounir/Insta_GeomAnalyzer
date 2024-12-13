#pragma once
// GeomInfo.h
#include <gp_Pnt.hxx>

#ifndef GEOMINFO_H
#define GEOMINFO_H

struct GeomInfo {
    double surfaceArea;
    double volume;
    gp_Pnt centerMass;
    gp_Pnt centerGeom;
    double xDimension;
    double yDimension;
    double zDimension;
    double boxVolume;
    double volumeRatio;
};

struct STEPInfo {
    int numVertices;
    int numEdges;
    int numFaces;
    int numShells;
    int numSolids;
};

struct BiegeInfo {
    int potentialBiegungen;
    double BiegeProbability;
    double Radius;
    double Winkel;
    double Laenge;
    double Schenkellange;
};


struct KurvenInfo {
    double planarAnteil;
    double einfKurvAnteil;
    double zwfKurvAnteil;
    double andAnteil;
};

#endif // GEOMINFO_H