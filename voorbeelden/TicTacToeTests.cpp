//============================================================================
// Name        : TicTacToeTests.cpp
// Author      : Serge Demeyer
// Version     :
// Copyright   : Project Software Engineering - BA1 Informatica - Serge Demeyer - University of Antwerp
// Description : TicTactToe in C++, Ansi-style
//============================================================================

#include <iostream>
#include <gtest/gtest.h>
#include "TicTacToe.h"

class TicTactToeTest: public ::testing::Test {
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}

	TicTacToe ttt_;
};

// Test: Default Constructor
TEST_F(TicTactToeTest, DefaultConstructor) {
	EXPECT_EQ(0, ttt_.nrOfMoves());
	EXPECT_TRUE(ttt_.notDone());
}

// Test: Happy Day scenario
TEST_F(TicTactToeTest, HappyDay) {
	EXPECT_EQ(0, ttt_.nrOfMoves());
	EXPECT_TRUE(ttt_.notDone());
	while (ttt_.notDone()) {
		ttt_.doMove();
	}
	EXPECT_FALSE(ttt_.notDone());
	EXPECT_EQ(9, ttt_.nrOfMoves());
}

// Test: LargestFunction
TEST_F(TicTactToeTest, LargestFunction) {
	std::vector<int> values = {10, 20, 30, 25, 5};
	EXPECT_EQ(30, ttt_.largest(values));

	values = {-10, -20, -5, -30};
	EXPECT_EQ(-5, ttt_.largest(values));

	values = {42};
	EXPECT_EQ(42, ttt_.largest(values));

	EXPECT_THROW(ttt_.largest({}), std::invalid_argument);
}

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
