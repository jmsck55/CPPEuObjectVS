// Copyright (c) 2022 James Cook
// be_math.h
//
// Functions that use the math library: #include <math.h>
//
// 32/64-bit using macro BITS64 for 64-bit
//
// Updated be_math.c for when Atom is a long double.

#ifndef _BE_MATH_H
#define _BE_MATH_H

//#include <math.h>

#define USE_MATH_H
#include "../eu/be_eu.h"

// be_runtime.c

// all use: #include <math.h>
MY_DLL_API object Dremainder(d_ptr a, d_ptr b);
MY_DLL_API object Jremainder(object a, object b);
MY_DLL_API object Epower(elong a, elong b);
MY_DLL_API object Dpower(d_ptr a, d_ptr b);
MY_DLL_API object Jpower(object a, object b);
MY_DLL_API object e_sqrt(elong a);
MY_DLL_API object De_sqrt(d_ptr a);
MY_DLL_API object Je_sqrt(object a);
MY_DLL_API object e_sin(elong a);
MY_DLL_API object De_sin(d_ptr a);
MY_DLL_API object Je_sin(object a);
MY_DLL_API object e_cos(elong a);
MY_DLL_API object De_cos(d_ptr a);
MY_DLL_API object Je_cos(object a);
MY_DLL_API object e_tan(elong a);
MY_DLL_API object De_tan(d_ptr a);
MY_DLL_API object Je_tan(object a);
MY_DLL_API object e_arctan(elong a);
MY_DLL_API object De_arctan(d_ptr a);
MY_DLL_API object Je_arctan(object a);
MY_DLL_API object e_log(elong a);
MY_DLL_API object De_log(d_ptr a);
MY_DLL_API object Je_log(object a);
MY_DLL_API object e_floor(elong a); // not used anymore
MY_DLL_API object De_floor(d_ptr a);
MY_DLL_API object Je_floor(object a);

#endif // _BE_MATH_H
