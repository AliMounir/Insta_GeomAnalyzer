#pragma once


double calcFraesen(int tol, double tWand, double innen, double volRatio, double profil);
double calcDrehen(double D1, double D2, int tol, double rotSym, double profil, double volRatio, double innen, double zwKrum, double einKrum, double othKrum, double planar);
double calcRohrLaserTeil(double D1, double D2, int tol, double sym, double rotSym, double profil, double tWand, double wandPercent, double volRatio, double innen);