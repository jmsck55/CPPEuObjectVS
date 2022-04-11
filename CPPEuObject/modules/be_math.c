// Copyright (c) 2022 James Cook
// be_math.c
//
// 32/64-bit using macro BITS64 for 64-bit

//#include "pch.h"
#include "be_math.h"

// all use: #include <math.h>

#ifdef BITS64
#define MATH_FUNC(T) T ## l
#else
#define MATH_FUNC(T) T
#endif

object Dremainder(d_ptr a, d_ptr b)
/* double remainder of a divided by b */
{
    if (b->dbl == 0.0)
        RTFatal("can't get remainder of a number divided by 0");
    return (object)NewDouble(MATH_FUNC(fmod)(a->dbl, b->dbl)); /* for now */
}

object Jremainder(object a, object b)
{
	return e_op2(a, b, &Jremainder, &eremainder, &Dremainder);
}

object Epower(elong a, elong b)
/* integer a to the power b */
{
    elong i, p;

#ifdef BITS64
    if (a == 2 && b >= 0 && b <= 61) {
#else
    if (a == 2 && b >= 0 && b <= 29) {
#endif
        /* positive power of 2 */
        return MAKE_INT(1 << b);
    }
    else if (a == 0 && b <= 0) {
        RTFatal("can't raise 0 to power <= 0");
        return 0;
    }
    else if (b == 0) {
        return ATOM_1;
    }
    else if (b >= 1 && b <= 4 && a >= -178 && a <= 178) {
        p = a;
        for (i = 2; i <= b; i++)
            p = p * a;
        return MAKE_INT(p);
    }
    else
        return (object)NewDouble(MATH_FUNC(pow)((eudouble)a, (eudouble)b));
}

object Dpower(d_ptr a, d_ptr b)
/* double power */
{
    if (a->dbl == 0.0 && b->dbl <= 0.0)
        RTFatal("can't raise 0 to power <= 0");
    if (a->dbl < 0.0 && floor(b->dbl) != b->dbl)
        RTFatal("can't raise negative number to non-integer power");
    return (object)NewDouble(MATH_FUNC(pow)(a->dbl, b->dbl));
}

object Jpower(object a, object b)
{
	return e_op2(a, b, &Jpower, &Epower, &Dpower);
}

object e_sqrt(elong a)
/* integer square_root(a) */
{
    if (a < 0)
        RTFatal("attempt to take square root of a negative number");
    return (object)NewDouble(MATH_FUNC(sqrt)((eudouble)a));
}

object De_sqrt(d_ptr a)
/* eudouble square root(a) */
{
    if (a->dbl < 0)
        RTFatal("attempt to take square root of a negative number");
    return (object)NewDouble(MATH_FUNC(sqrt)(a->dbl));
}

object Je_sqrt(object a)
{
	return e_op1(a, &Je_sqrt, &e_sqrt, &De_sqrt);
}

object e_sin(elong a)
/* sin of an angle a (radians) */
{
    return (object)NewDouble(MATH_FUNC(sin)((eudouble)a));
}

object De_sin(d_ptr a)
/* eudouble sin of a */
{
    return (object)NewDouble(MATH_FUNC(sin)(a->dbl));
}

object Je_sin(object a)
{
	return e_op1(a, &Je_sin, &e_sin, &De_sin);
}

object e_cos(elong a)
/* cos of an angle a (radians) */
{
    return (object)NewDouble(MATH_FUNC(cos)((eudouble)a));
}

object De_cos(d_ptr a)
/* eudouble cos of a */
{
    return (object)NewDouble(MATH_FUNC(cos)(a->dbl));
}

object Je_cos(object a)
{
	return e_op1(a, &Je_cos, &e_cos, &De_cos);
}

object e_tan(elong a)
/* tan of an angle a (radians) */
{
    return (object)NewDouble(MATH_FUNC(tan)((eudouble)a));
}

object De_tan(d_ptr a)
/* eudouble tan of a */
{
    return (object)NewDouble(MATH_FUNC(tan)(a->dbl));
}

object Je_tan(object a)
{
	return e_op1(a, &Je_tan, &e_tan, &De_tan);
}

object e_arctan(elong a)
/* arctan of an angle a (radians) */
{
    return (object)NewDouble(MATH_FUNC(atan)((eudouble)a));
}

object De_arctan(d_ptr a)
/* eudouble arctan of a */
{
    return (object)NewDouble(MATH_FUNC(atan)(a->dbl));
}

object Je_arctan(object a)
{
	return e_op1(a, &Je_arctan, &e_arctan, &De_arctan);
}

object e_log(elong a)
/* natural log of a (integer) */
{
    if (a <= 0)
        RTFatal("may only take log of a positive number");
    return (object)NewDouble(MATH_FUNC(log)((eudouble)a));
}

object De_log(d_ptr a)
/* natural log of a (eudouble) */
{
    if (a->dbl <= 0.0)
        RTFatal("may only take log of a positive number");
    return (object)NewDouble(MATH_FUNC(log)(a->dbl));
}

object Je_log(object a)
{
	return e_op1(a, &Je_log, &e_log, &De_log);
}

object e_floor(elong a)  // not used anymore
/* floor of a number - no op since a is already known to be an int */
{
    return a; 
}

object De_floor(d_ptr a)
/* floor of a number */
{
    eudouble temp;

    temp = MATH_FUNC(floor)(a->dbl);
#ifndef ERUNTIME    
    if (MATH_FUNC(fabs)(temp) < MAXINT_DBL)
        return MAKE_INT((elong)temp);
    else 
#endif      
        return (object)NewDouble(temp);
}

object Je_floor(object a)
{
	return e_op1(a, &Je_floor, &e_floor, &De_floor);
}

