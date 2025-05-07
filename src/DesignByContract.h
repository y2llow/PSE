//============================================================================
// Name        : DesignByContract.h
// Author      : Serge Demeyer, modified by Kasper Engelen
// Version     :
// Copyright   : Project Software Engineering - BA1 Informatica - Serge Demeyer - University of Antwerp
// Description : Declarations for design by contract in C++
//============================================================================

// DesignByContract.h
#ifndef DESIGN_BY_CONTRACT_H
#define DESIGN_BY_CONTRACT_H

#include <stdexcept>
#include <string>

// Globale variabele om contracten aan/uit te zetten
extern bool g_enforceContracts;

// Aangepaste macros die rekening houden met de globale schakelaar
#define REQUIRE(condition, message) \
    if (g_enforceContracts && !(condition)) throw std::invalid_argument(message)

#define ENSURE(condition, message) \
    if (g_enforceContracts && !(condition)) throw std::logic_error(message)

#endif // DESIGN_BY_CONTRACT_H

/*
#include <assert.h>

#define REQUIRE(assertion, what) \
if (!(assertion)) __assert (what, __FILE__, __LINE__)

#define ENSURE(assertion, what) \
if (!(assertion)) __assert (what, __FILE__, __LINE__)


#define REQUIRE(condition, message) \
if (!(condition)) throw std::invalid_argument(message)

#define ENSURE(condition, message) \
if (!(condition)) throw std::logic_error(message)
*/