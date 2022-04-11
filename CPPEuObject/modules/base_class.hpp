// Copyright (c) 2022 James Cook
//
// base_class for all EuObjects.

#pragma once

// base_class for protected data, operators, and functions.

namespace eu
{
    extern "C"
    {
#include "../pch.h"
#include "be_eu.h"
    }

    class base_class
    {
    protected:
        object obj;
    public:
#ifndef DONE_DEBUGGING
        inline static d_ptr lookatd;
        inline static s1_ptr lookats;
#endif
        bool is_initialized() { SET_DEBUG return obj != NOVALUE; }
        object swap(object x) { object ret = obj; obj = x; SET_DEBUG return ret; }

        elong get_etype() { if (obj == NOVALUE) return 0; if (IS_ATOM_INT(obj)) return E_INTEGER; if (IS_ATOM_DBL(obj)) return E_ATOM; if (IS_SEQUENCE(obj)) return E_SEQUENCE; return E_OBJECT; }
        object GetValue() { Ref(obj) return obj; } // Get a copy of the value.
        base_class& operator= (const object& x)
        {
            DeRef(obj)
            obj = x;
            Ref(obj) // Possibly not a memory leak.
            return *this;
        }

        object operator + (const base_class& param) { return Jadd(obj, param.obj); }
        object operator - (const base_class& param) { return Jminus(obj, param.obj); }
        object operator * (const base_class& param) { return Jmultiply(obj, param.obj); }
        object operator / (const base_class& param) { return Jdivide(obj, param.obj); }

        // TODO: Comparing operators, and other operators.


        // Boolean atom functions: (To be made into operator's)
        object equals(object b) { return Jequals(obj, b); }
        object lessThan(object b) { return Jless(obj, b); }
        object greaterThan(object b) { return Jgreater(obj, b); }
        object notEqual(object b) { return Jnoteq(obj, b); }
        object lessThanOrEqual(object b) { return Jlesseq(obj, b); }
        object greaterThanOrEqual(object b) { return Jgreatereq(obj, b); }
        object logicaland(object b) { return Jand(obj, b); }
        object logicalor(object b) { return Jor(obj, b); }
        object logicalxor(object b) { return Jxor(obj, b); }
        object logicalnot() { return Junot(obj); }
        object uminus() { return Juminus(obj); }

        // 32-bit or 64-bit atom functions:
        object not_bits() { return Jnot_bits(obj); }
        object and_bits(object b) { return Jand_bits(obj, b); }
        object or_bits(object b) { return Jor_bits(obj, b); }
        object xor_bits(object b) { return Jxor_bits(obj, b); }


    };

    int Version() { return 1; } // Version 0.1
    void Abort(elong error_level) { UserCleanup(error_level); }

    // Add more built-in and standard library functions here (Euphoria v3.1.1, v4.0.5, v4.1a)

}
