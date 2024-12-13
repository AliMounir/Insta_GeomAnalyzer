#include "pch.h"
#include "stepImporters.h"
#include "GeomInfo.h"



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

STEPInfo InspectShape(const TopoDS_Shape& shape) {

    STEPInfo infos;
    infos.numVertices = 0;
    infos.numEdges = 0;
    infos.numFaces = 0;
    infos.numShells = 0;
    infos.numSolids = 0;

    // Zählen der Vertices
    for (TopExp_Explorer exp(shape, TopAbs_VERTEX); exp.More(); exp.Next()) {
        infos.numVertices++;
    }

    // Zählen der Edges
    for (TopExp_Explorer exp(shape, TopAbs_EDGE); exp.More(); exp.Next()) {
        infos.numEdges++;
    }

    // Zählen der Faces
    for (TopExp_Explorer exp(shape, TopAbs_FACE); exp.More(); exp.Next()) {
        infos.numFaces++;
    }

    // Zählen der Shells
    for (TopExp_Explorer exp(shape, TopAbs_SHELL); exp.More(); exp.Next()) {
        infos.numShells++;
    }

    // Zählen der Solids
    for (TopExp_Explorer exp(shape, TopAbs_SOLID); exp.More(); exp.Next()) {
        infos.numSolids++;
    }


    infos.numEdges = infos.numEdges / 2;

    std::cout << "Das Shape enthält:" << std::endl;
    std::cout << "- Vertices: " << infos.numVertices << std::endl;
    std::cout << "- Edges: " << infos.numEdges << std::endl;
    std::cout << "- Faces: " << infos.numFaces << std::endl;
    std::cout << "- Shells: " << infos.numShells << std::endl;
    std::cout << "- Solids: " << infos.numSolids << std::endl;

    return infos;
}

