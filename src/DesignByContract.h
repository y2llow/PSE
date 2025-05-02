//============================================================================
// Name        : DesignByContract.h
// Author      : Serge Demeyer, modified by Kasper Engelen
// Version     :
// Copyright   : Project Software Engineering - BA1 Informatica - Serge Demeyer - University of Antwerp
// Description : Declarations for design by contract in C++
//============================================================================

#include <assert.h>

//#define REQUIRE(assertion, what) \
//if (!(assertion)) __assert (what, __FILE__, __LINE__)
//
//#define ENSURE(assertion, what) \
//if (!(assertion)) __assert (what, __FILE__, __LINE__)

#define REQUIRE(condition, message) \
if (!(condition)) throw std::invalid_argument(message)

#define ENSURE(condition, message) \
if (!(condition)) throw std::logic_error(message)
