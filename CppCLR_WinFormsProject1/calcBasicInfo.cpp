#include "pch.h"
#include "calcBasicInfo.h"
#include "GeomInfo.h"

#include <BRepGProp.hxx>
#include <BRepBndLib.hxx>
#include <BRepBuilderAPI_Transform.hxx>

#include <GProp_GProps.hxx>
#include <GProp_PrincipalProps.hxx>

#include <TopoDS_Shape.hxx>
#include <Bnd_Box.hxx>



GeomInfo getBasicInfo(const TopoDS_Shape& shape) {

    GeomInfo info;

    // Calculate the surface area
    GProp_GProps surfaceProps;
    BRepGProp::SurfaceProperties(shape, surfaceProps);
    info.surfaceArea = surfaceProps.Mass();

    // Calculate the volume
    GProp_GProps volumeProps;
    BRepGProp::VolumeProperties(shape, volumeProps);
    info.volume = volumeProps.Mass();

    // Berechnung der Bounding Box für die Abmaße
    Bnd_Box boundingBox;
    BRepBndLib::Add(shape, boundingBox);

    Standard_Real xmin, ymin, zmin, xmax, ymax, zmax;
    boundingBox.Get(xmin, ymin, zmin, xmax, ymax, zmax);

    info.xDimension = xmax - xmin;
    info.yDimension = ymax - ymin;
    info.zDimension = zmax - zmin;
    info.boxVolume = info.xDimension * info.yDimension * info.zDimension;

    // Volumenverhältnis berechnen (Bauteilvolumen / Boxvolumen)
    info.volumeRatio = (info.boxVolume != 0) ? (info.volume / info.boxVolume) : 0;




    // Output the results
    std::cout << "Abmaße (X,Y,Z): " << info.xDimension << "x" << info.yDimension << "x" << info.zDimension << "mm" << std::endl;
    std::cout << "Oberfläche: " << info.surfaceArea << " mm^2" << std::endl;
    std::cout << "Volumen: " << info.volume << " mm^3" << std::endl;
    std::cout << "Box-Volumen: " << info.boxVolume << " mm^3" << std::endl;
    std::cout << "Volumenverhältnis (Bauteilvolumen / Boxvolumen): " << info.volumeRatio * 100 << "%" << std::endl;

    return info;

}

TopoDS_Shape CenterShape(const TopoDS_Shape& shape) {
    GProp_GProps gprops;
    BRepGProp::VolumeProperties(shape, gprops);
    gp_Pnt centerOfMass = gprops.CentreOfMass();

    gp_Trsf translateToOrigin;
    translateToOrigin.SetTranslation(centerOfMass, gp_Pnt(0, 0, 0));
    BRepBuilderAPI_Transform transformer(shape, translateToOrigin);

    TopoDS_Shape centeredShape = transformer.Shape();
    return centeredShape;
}
