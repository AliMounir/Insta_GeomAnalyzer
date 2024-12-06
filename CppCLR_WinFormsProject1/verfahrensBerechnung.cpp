#include "verfahrensBerechnung.h"
#include "pch.h"

#include <iostream>


double calcFraesen(int tol, double tWand, double innen, double volRatio, double profil) {

	std::cout << "calc Fräsen beginnt " << std::endl;

	double finalScore = 0.0;

	// KO Kriterien
	if (tol > 3 || innen > 0.98 || tWand < 1) {
		std::cout << "K.O. KRITERIUM! " << std::endl;
		return 0.0;
	}

	finalScore = (volRatio + (1-innen) + (1-profil) + tol) / 4;

	std::cout << "final score fräsen: " << finalScore << std::endl;

	return finalScore;
}






double calcDrehen(double D1, double D2, int tol, double rotSym, double profil, double volRatio, double innen, double zwKrum, double einKrum, double othKrum, double planar) {

	std::cout << "calc Rohlaserteil beginnt " << std::endl;

	double finalScore = 0.0;
	double durchmesser = (D1 + D2) / 2;
	double dRatio = (D1/D1);

	// KO Kriterien
	if (dRatio < 0.9 || durchmesser > 900 || zwKrum > 0.7 || othKrum > 0.7 || planar > 0.5 || einKrum < 0.2 || tol > 3 || innen > 0.98) {
		std::cout << "K.O. KRITERIUM! " << std::endl;
		return 0.0;
	}


	std::cout << "durchmesser errechnet: " << durchmesser << "mit D Ratio: " << dRatio * 100 << "%" << std::endl;

	finalScore = (dRatio + einKrum + rotSym) / 3;

	std::cout << "final score drehen: " << finalScore << std::endl;

	return finalScore;
}





double calcRohrLaserTeil (double D1, double D2, int tol, double sym, double rotSym, double profil, double tWand, double wandPercent, double volRatio, double innen) {

	std::cout << "calc Rohlaserteil beginnt " << std::endl;

	double finalScore = 0.0;
	double durchmesser = (D1+D2)/2;
	double dRatio = (D1 / D1);
	double allgemeineSym = 0.0;

	// KO Kriterien
	if (dRatio < 0.9 || durchmesser > 400 || volRatio > 0.5 || tol > 2 || innen > 0.98 || tWand > durchmesser || tWand < 0.2) {
		std::cout << "K.O. KRITERIUM! " << std::endl;
		return 0.0;
    }

	//muss mindestens eine beider Symmetrien aufweisen
	if (sym < rotSym) {
		allgemeineSym = rotSym;
	} else {
		allgemeineSym = sym;
	}

	std::cout << "durchmesser errechnet: " << durchmesser << "mit D Ratio: " << dRatio << " und gewählte symmetrie: " << allgemeineSym * 100 << "%" << std::endl;

	finalScore = (allgemeineSym + profil*2 + wandPercent + (1-volRatio)) / 5 ;

	std::cout << "final score rohrlaser: " << finalScore << std::endl;

	return finalScore;
}