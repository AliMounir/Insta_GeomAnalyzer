#include "stepImporters.h"
#include "pch.h"


// Notwendige Includes
#include <STEPControl_Reader.hxx>
#include <Interface_Static.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Shell.hxx>
#include <TopoDS_Solid.hxx>
#include <IFSelect_ReturnStatus.hxx>
#include <iostream>



TopoDS_Shape ImportSTEP(const Standard_CString filename) {
    STEPControl_Reader reader;
    IFSelect_ReturnStatus status = reader.ReadFile(filename);
    if (status != IFSelect_RetDone) {
        std::cout << "Fehler beim Lesen der STEP-Datei." << std::endl;
        return TopoDS_Shape(); // Leeres Shape zurückgeben
    }
    Interface_Static::SetCVal("read.step.product.mode", "MANIFOLD_SOLID_BREP");
    // Den gesamten Inhalt der STEP-Datei in ein Shape übertragen
    reader.TransferRoots();
    TopoDS_Shape shape = reader.OneShape();
    return shape;


}

void InspectShape(const TopoDS_Shape& shape) {
    int numVertices = 0;
    int numEdges = 0;
    int numFaces = 0;
    int numShells = 0;
    int numSolids = 0;

    // Zählen der Vertices
    for (TopExp_Explorer exp(shape, TopAbs_VERTEX); exp.More(); exp.Next()) {
        numVertices++;
    }

    // Zählen der Edges
    for (TopExp_Explorer exp(shape, TopAbs_EDGE); exp.More(); exp.Next()) {
        numEdges++;
    }

    // Zählen der Faces
    for (TopExp_Explorer exp(shape, TopAbs_FACE); exp.More(); exp.Next()) {
        numFaces++;
    }

    // Zählen der Shells
    for (TopExp_Explorer exp(shape, TopAbs_SHELL); exp.More(); exp.Next()) {
        numShells++;
    }

    // Zählen der Solids
    for (TopExp_Explorer exp(shape, TopAbs_SOLID); exp.More(); exp.Next()) {
        numSolids++;
    }

    std::cout << "Das Shape enthält:" << std::endl;
    std::cout << "- Vertices: " << numVertices << std::endl;
    std::cout << "- Edges: " << numEdges << std::endl;
    std::cout << "- Faces: " << numFaces << std::endl;
    std::cout << "- Shells: " << numShells << std::endl;
    std::cout << "- Solids: " << numSolids << std::endl;
}

