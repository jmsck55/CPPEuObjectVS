// Copyright (c) 2022 James Cook
// eumath.hpp
// Floating-point Math Classfile for EuObject
// "class Dbl" uses integers and doubles.
// It includes "math.h" library routines for double floating-point numbers.
//
// See also: obj.hpp

// Includes just the math routines:

#pragma once

#define USE_MATH_H
//#include "../eu/Atom.hpp"

namespace eu
{
    extern "C" {
        #include "be_math.h" // See: "eumath.hpp"
    }

    // Regular eudouble functions:
    object A_remainder(object a, object b) { return Jremainder(a, b); }
    object A_power(object a, object b) { return Jpower(a, b); }
    // Unary Ops
    object A_sqrt(object a) { return Je_sqrt(a); }
    object A_sin(object a) { return Je_sin(a); }
    object A_cos(object a) { return Je_cos(a); }
    object A_tan(object a) { return Je_tan(a); }
    object A_arctan(object a) { return Je_arctan(a); }
    object A_log(object a) { return Je_log(a); }
    object A_floor(object a) { return Je_floor(a); }

}
