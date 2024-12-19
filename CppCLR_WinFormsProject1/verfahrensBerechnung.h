#pragma once


double calcFraesen(int tol, double tWand, double innen, double volRatio, double profil);
double calcDrehen(double D1, double D2, int tol, double rotSym, double profil, double volRatio, double innen, double zwKrum, double einKrum, double othKrum, double planar);
double calcRohrLaserTeil(double D1, double D2, int tol, double symA, double symB, double rotSym, double profil, double tWand, double wandPercent, double volRatio, double innen);
double calcBlech(int tol, double parallel, double tWand, double wandPercent, double sym1, double sym2, double innen, double volRatio, double biegung, double planar, double ekrum, double zwkrum, double ankrum, double xDim, double yDim, double zDim);
double calcBiegeBlech(int tol, double parallel, double tWand, double wandPercent, double sym1, double sym2, double innen, double volRatio, double biegung, double planar, double BLaenge, double xDim, double yDim, double zDim);
double calcAdditiv(double xDim, double yDim, double zDim, int tol, double innen, double volRatio);
std::string calcPrefVerfahren(double blech, double biege, double rohr, double dreh, double mill, double addi, double threshold);