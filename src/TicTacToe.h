//============================================================================
// Name        : TicTacToe.h
// Author      : Serge Demeyer
// Version     :
// Copyright   : Project Software Engineering - BA1 Informatica - Serge Demeyer - University of Antwerp
// Description : TicTactToe in C++, Ansi-style
//============================================================================

#include <vector>
#include <stdexcept>

#define REQUIRE(condition, message) \
if (!(condition)) throw std::invalid_argument(message)

#define ENSURE(condition, message) \
if (!(condition)) throw std::logic_error(message)

class TicTacToe {
public:
	TicTacToe();
	bool notDone();
	void doMove();
	int nrOfMoves();
	int largest(std::vector<int>);

private:
	int _nrOfMoves;
};

