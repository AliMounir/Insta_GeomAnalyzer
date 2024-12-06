#include "verfahrensBerechnung.h"
#include "pch.h"

#include <iostream>


double calcFraesen(int tol, double tWand, double innen, double volRatio, double profil) {

	std::cout << "calc Fräsen beginnt " << std::endl;

	double finalScore = 0.0;

	// KO Kriterien
	if (tol > 3 || innen > 0.98 || tWand < 1) {
		std::cout << "KO KRITERIUM! " << std::endl;
		return 0.0;
	}

	finalScore = (volRatio + (1-innen) + (1-profil)) / 3;

	std::cout << "final score rohrlaser: " << finalScore << std::endl;

	return finalScore;
}


double calcRohrLaserTeil (double D1, double D2, int tol, double sym, double rotSym, double profil, double tWand, double wandPercent, double volRatio, double innen) {

	std::cout << "calc Rohlaserteil beginnt " << std::endl;

	double finalScore = 0.0;
	double durchmesser = (D1+D2)/2;
	double allgemeineSym = 0.0;

	// KO Kriterien
	if (durchmesser > 400 || volRatio > 0.5 || tol > 2 || innen > 0.98 || tWand > durchmesser || tWand < 0.2) {
		std::cout << "KO KRITERIUM! " << std::endl;
		return 0.0;
    }

	//muss mindestens eine beider Symmetrien aufweisen
	if (sym < rotSym) {
		allgemeineSym = rotSym;
	} else {
		allgemeineSym = sym;
	}

	std::cout << "durchmesser errechnet: " << durchmesser << " und gewählte symmetrie: " << allgemeineSym * 100 << "%" << std::endl;

	finalScore = (allgemeineSym + profil*2 + wandPercent + (1-volRatio)) / 5 ;

	std::cout << "final score rohrlaser: " << finalScore << std::endl;

	return finalScore;
}