#include "verfahrensBerechnung.h"
#include "pch.h"

#include <map>
#include <string>
#include <vector>
#include <iostream>

double calcFraesen(int tol, double tWand, double innen, double volRatio, double profil) {

	std::cout << "calc Fräsen beginnt " << std::endl;

	double finalScore = 0.0;

	// KO Kriterien
	if (tol > 3 || innen > 0.98 || tWand < 1) {
		std::cout << "K.O. KRITERIUM! " << std::endl;
		return 0.0;
	}

	if (tol > 1) {
		tol = 1;
	}

	finalScore = (volRatio*2 + (1-innen) + (1-profil) + tol) / 5;

	std::cout << "final score fräsen: " << finalScore << std::endl;

	return finalScore;
}






double calcDrehen(double D1, double D2, int tol, double rotSym, double profil, double volRatio, double innen, double zwKrum, double einKrum, double othKrum, double planar) {

	std::cout << "calc Drehen beginnt " << std::endl;

	double finalScore = 0.0;
	double durchmesser = (D1 + D2) / 2;
	double dRatio = (std::min(D1, D2))/(std::max(D1, D2));

	// KO Kriterien
	if (dRatio < 0.9 || durchmesser > 900 || zwKrum > 0.7 || othKrum > 0.8 || planar > 0.9 || einKrum < 0.1 || tol > 3 || innen > 0.98) {
		std::cout << "K.O. KRITERIUM! " << std::endl;
		return 0.0;
	}


	std::cout << "durchmesser errechnet: " << durchmesser << "mit D Ratio: " << dRatio * 100 << "%" << std::endl;

	finalScore = (dRatio + einKrum + rotSym*2) / 4;

	std::cout << "final score drehen: " << finalScore << std::endl;

	return finalScore;
}




double calcBlech(int tol, double parallel, double tWand, double wandPercent, double sym1, double sym2, double innen, double volRatio, double biegung, double planar, double ekrum, double zwkrum, double ankrum, double xDim, double yDim, double zDim) {

	std::cout << "calc Blech beginnt " << std::endl;

	double finalScore = 0.0;
	double selectedSym = std::max(sym1, sym2);
	double dickeV = 1.0;

	double krum = std::max({ ekrum, zwkrum, ankrum});


	if (std::fabs(tWand - xDim) <= 1 || std::fabs(tWand - yDim) <= 1 || std::fabs(tWand - zDim) <= 1) {
		dickeV = 1.0;
	}
	else {
		std::cout << "twand stimmt nicht mit abmaße ein " << std::endl;
		dickeV = 0.0;
	}

	// KO Kriterien																 || dickeV == 0
	if (tol > 2 || innen > 0.98 || tWand > 50 || tWand < 0.25 || planar < 0.5 || parallel == 0) {
		std::cout << "K.O. KRITERIUM! " << std::endl;
		return 0.0;
	}


	/*if (std::abs(std::fmod(tWand, 0.25)) > epsilon) {
	*	std::cout << "wandstärke ist nicht standard (0.25x) " << std::endl;
		standardAb = 0.0;
	}*/


	finalScore = (wandPercent * 2 + parallel + (1 - biegung) + selectedSym + dickeV + planar + (1-krum)) / 8;

	std::cout << "final score blech: " << finalScore << std::endl;

	return finalScore;
}



double calcBiegeBlech(int tol, double parallel, double tWand, double wandPercent, double sym1, double sym2, double innen, double volRatio, double biegung, double planar, double BLaenge, double xDim, double yDim, double zDim) {

	std::cout << "calc Biege-Blech beginnt " << std::endl;

	double finalScore = 0.0;
	double selectedSym = std::max(sym1,sym2);
	double biegelängV = 1.0;

	if (std::fabs(BLaenge-xDim) <= 1 || std::fabs(BLaenge - yDim) <= 1 || std::fabs(BLaenge - zDim) <= 1) {
		biegelängV = 1.0;
	} else {
		std::cout << "biegelänge stimmt nicht ein " << std::endl;
		biegelängV = 0.0;
	}

	// KO Kriterien																				// parallel==0 || biegelängV == 0
	if (tol > 2 || innen > 0.98 || tWand > 20 || tWand < 1 || biegung < 0.2 || planar < 0.1) {
		std::cout << "K.O. KRITERIUM! " << std::endl;
		return 0.0;
	}


	finalScore = (wandPercent + biegung*5 + selectedSym + biegelängV + parallel) / 9;

	std::cout << "final score Biegeblech: " << finalScore << std::endl;

	return finalScore;
}





double calcRohrLaserTeil (double D1, double D2, int tol, double sym, double rotSym, double profil, double tWand, double wandPercent, double volRatio, double innen) {

	std::cout << "calc Rohlaserteil beginnt " << std::endl;

	double finalScore = 0.0;
	double durchmesser = (D1+D2)/2;
	double dRatio = (std::min(D1, D2)) / (std::max(D1, D2));

	// KO Kriterien
	if (dRatio < 0.9 || durchmesser > 400 || volRatio > 0.9 || tol > 2 || innen > 0.98 || tWand > durchmesser || tWand < 0.2 || profil < 0.2) {
		std::cout << "K.O. KRITERIUM! " << std::endl;
		return 0.0;
    }

	
	double allgemeineSym = std::max(rotSym, sym);
	

	std::cout << "durchmesser errechnet: " << durchmesser << "mit D Ratio: " << dRatio << " und gewählte symmetrie: " << allgemeineSym * 100 << "%" << std::endl;

	finalScore = (allgemeineSym + profil*2 + wandPercent + (1-volRatio)) / 5 ;

	std::cout << "final score rohrlaser: " << finalScore << std::endl;

	return finalScore;
}


double calcAdditiv(double xDim, double yDim, double zDim, int tol, double innen, double volRatio) {

	std::cout << "calc Additves Verfahren beginnt " << std::endl;

	double finalScore = 0.0;
	double maxDimension = std::max(xDim, std::max(yDim, zDim));

	// KO Kriterien
	if (tol > 1 || maxDimension > 700) {
		std::cout << "K.O. KRITERIUM! " << std::endl;
		return 0.0;
	}

	if (innen < 0.5) {
		innen = 1 - innen;
	}

	finalScore = (innen + 3 + (1-volRatio)) / 5;

	std::cout << "final score additive verfahren: " << finalScore << std::endl;

	return finalScore;
}


std::string calcPrefVerfahren(double blech, double biege, double rohr, double dreh, double mill, double addi, double threshold) {

	std::map<std::string, double> processScores = {
		{"CNC-Blech", blech},
		{"Biegeblech", biege},
		{"Rohrlaserteil", rohr},
		{"CNC-Drehen", dreh},
		{"CNC-Fräsen", mill},
		{"Additive Fertigung", addi}
	};

	//cost order (cheapest first)
	static const std::vector<std::string> order = {
		"CNC-Blech",
		"Biegeblech",
		"Rohrlaserteil",
		"CNC-Drehen",
		"CNC-Fräsen",
		"Additive Fertigung"
	};

	// Filter and find the first valid process based on the order
	for (const auto& process : order) {
		auto it = processScores.find(process);
		if (it != processScores.end()) {
			double score = it->second;
			if (score >= threshold) {
				// This process meets the criteria
				std::cout << "Cheapest valid process: " << process << std::endl;
				return process;
			}
		}
	}


	double newThreshold = threshold * 0.8;
	std::cout << "Didnt find pref.Verfahren. New threshold is: " << newThreshold << std::endl;
	for (const auto& process : order) {
		auto it = processScores.find(process);
		if (it != processScores.end()) {
			double score = it->second;
			if (score >= newThreshold) {
				// This process meets the criteria
				std::cout << "Cheapest valid process (new): " << process << std::endl;
				return process;
			}
		}
	}

	// If we get here, no suitable process was found
	return "N/V";
}