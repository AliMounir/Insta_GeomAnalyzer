#pragma once
// GeomInfo.h
#ifndef GEOMINFO_H
#define GEOMINFO_H

struct GeomInfo {
    double surfaceArea;
    double volume;
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

#endif // GEOMINFO_H