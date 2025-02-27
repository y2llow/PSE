//============================================================================
// Name        : TicTacToe.cpp
// Author      : Serge Demeyer
// Version     :
// Copyright   : Project Software Engineering - BA1 Informatica - Serge Demeyer - University of Antwerp
// Description : TicTactToe in C++, Ansi-style
//============================================================================

#include "TicTacToe.h"
#include <limits>

TicTacToe::TicTacToe() {
	_nrOfMoves = 0;
}

bool TicTacToe::notDone() {
	return _nrOfMoves < 9;
}

void TicTacToe::doMove() {
	_nrOfMoves++;
}

int TicTacToe::nrOfMoves() {
	return _nrOfMoves;
}

int TicTacToe::largest(std::vector<int> list) {
	// Contract: de lijst mag niet leeg zijn
	REQUIRE(!list.empty(), "Input list must not be empty.");

	int max = std::numeric_limits<int>::min();
	for (int num : list) {
		if (num > max) {
			max = num;
		}
	}

	// Contract: de uitvoer moet een geldig maximum zijn
	ENSURE(max != std::numeric_limits<int>::min(), "Output must be a valid maximum value.");
	return max;
}
