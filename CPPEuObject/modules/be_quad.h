// Copyright (c) 2022 James Cook
// be_quad.h
//
// Functions that use the math library: #include <quadmath.h>
//
// 32/64-bit GCC using macro BITS64 for 64-bit

#ifndef _BE_QUAD_H
#define _BE_QUAD_H

//#include <quadmath.h>

// To use this file, define USE_QUADMATH_H for the compiler.
#define USE_QUADMATH_H
#include "../eu/be_eu.h"

// all use: #include <quadmath.h>
MY_DLL_API object NewFloat128(equadmath q);
MY_DLL_API equadmath GetFloat128(object obj);



#endif // _BE_QUAD_H
