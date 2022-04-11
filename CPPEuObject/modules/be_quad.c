// Copyright (c) 2022 James Cook
// be_quad.c
//
// Uses the math library: quadmath.h

//#include "pch.h"
#include "be_quad.h"

#ifdef BITS64
#define IS_QUAD(dptr) (dptr->cleanup == CLEANUP_FLOAT128)
#define GET_QUAD(dptr) ((equadmath)(dptr->quad))
#define GET_QUAD_PTR(dptr) (&dptr->quad)
#else
#define IS_QUAD(dptr) (dptr->cleanup == CLEANUP_FLOAT128)
#define GET_QUAD(dptr) ((equadmath)(*dptr->qdbl))
#define GET_QUAD_PTR(dptr) ((equadmath*)dptr->qdbl)
#endif // BITS64

object NewFloat128(equadmath q)
/* allocate space for a new double value */
{
    REGISTER d_ptr n;

    n = (d_ptr)EMalloc((elong)D_SIZE);
    n->ref = 1;
#ifdef BITS64
    n->cleanup = CLEANUP_FLOAT128; // jjc
    n->quad = q;
#else
    n->qdbl = (equadmath*)EMalloc((elong)sizeof(equadmath));
    n->type = C_FLOAT128; // 32-bit uses 'type', 64-bit uses 'cleanup'
#ifdef CLEANUP_MOD
    n->cleanup = CLEANUP_FLOAT128;
#endif
    *n->qdbl = q;
#endif
    return MAKE_DBL(n);
}

equadmath GetFloat128(object obj)
{
    if (IS_ATOM_INT(obj))
    {
        return (equadmath)obj;
    }
    else if(IS_ATOM_DBL(obj))
    {
        d_ptr p = DBL_PTR(obj);
        if (IS_QUAD(p))
        {
            return GET_QUAD(p);
        }
    }
    RTFatal("Expected a float128, double, or integer, in 'GetQuadmath()'");
    return 0.0q;
}

