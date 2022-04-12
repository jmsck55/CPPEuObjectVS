// Copyright (c) 2022 James Cook
//
// base_class for all EuObjects.

#pragma once

// base_class for protected data, operators, and functions.

namespace eu
{
    extern "C"
    {
//#include "../pch.h"
#include "be_eu.h"
    }

    int Version() { return 1; } // Version v0.0.1

    class base_class
    {
    protected:
        object obj;
    public:
#ifndef DONE_DEBUGGING
        inline static d_ptr lookatd;
        inline static s1_ptr lookats;
#endif
        base_class() { obj = NOVALUE; SET_DEBUG } // default constructor
        ~base_class() { SET_DEBUG DeRef(obj) obj = NOVALUE; } // default destructor
        base_class(const base_class& x) { obj = x.obj; SET_DEBUG Ref(obj) } // copy constructor
        base_class& operator= (const base_class& x) { SET_DEBUG DeRef(obj) obj = x.obj; SET_DEBUG Ref(obj) return *this; } // copy assignment
#ifndef __WATCOMC__
        base_class(base_class&& x) { SET_DEBUG obj = x.obj; SET_DEBUG x.obj = NOVALUE; } // move constructor
        base_class& operator= (base_class&& x) { SET_DEBUG DeRef(obj) obj = x.obj; SET_DEBUG x.obj = NOVALUE; return *this; } // move assignment
#endif
        //base_class(const object& x) { obj = x; RefObj(); }
        //base_class(object& x) { obj = x; RefObj(); }
        //base_class(const object x) { obj = x; RefObj(); }
        base_class(object x) { SET_DEBUG obj = x; SET_DEBUG } // Increase the reference count before calling this function.
        base_class& operator= (const object& x)
        {
            SET_DEBUG
            DeRef(obj)
            obj = x;
            SET_DEBUG
            Ref(obj) // Possibly not a memory leak.
            return *this;
        }

        //object Copy() { SET_DEBUG Ref(obj) return obj; }
        //void Delete() { SET_DEBUG DeRef(obj) }
        elong get_etype() { SET_DEBUG if (obj == NOVALUE) return 0; if (IS_ATOM_INT(obj)) return E_INTEGER; if (IS_ATOM_DBL(obj)) return E_ATOM; if (IS_SEQUENCE(obj)) return E_SEQUENCE; return E_OBJECT; }
        bool is_initialized() { SET_DEBUG return obj != NOVALUE; }
        object GetValue() { SET_DEBUG Ref(obj) return obj; } // Get a copy of the value.
        object swap(object x) { object ret = obj; obj = x; SET_DEBUG return ret; }

        // For Debugging:
        elong ViewCount() { if (IS_DBL_OR_SEQUENCE(obj)) return DBL_PTR(obj)->ref; return -1; }

        // Math operators:
        base_class operator- () { base_class temp(Juminus(obj)); return temp; }
        base_class operator+ (const base_class& param) { base_class temp(Jadd(obj, param.obj)); return temp; }
        base_class operator- (const base_class& param) { base_class temp(Jminus(obj, param.obj)); return temp; }
        base_class operator* (const base_class& param) { base_class temp(Jmultiply(obj, param.obj)); return temp; }
        base_class operator/ (const base_class& param) { base_class temp(Jdivide(obj, param.obj)); return temp; }

        // Logical operators:
        base_class operator! () { base_class temp(Junot(obj)); return temp; }
        base_class operator== (const base_class& param) { base_class temp(Jequals(obj, param.obj)); return temp; }
        base_class operator< (const base_class& param) { base_class temp(Jless(obj, param.obj)); return temp; }
        base_class operator> (const base_class& param) { base_class temp(Jgreater(obj, param.obj)); return temp; }
        base_class operator!= (const base_class& param) { base_class temp(Jnoteq(obj, param.obj)); return temp; }
        base_class operator<= (const base_class& param) { base_class temp(Jlesseq(obj, param.obj)); return temp; }
        base_class operator>= (const base_class& param) { base_class temp(Jgreatereq(obj, param.obj)); return temp; }
        base_class operator&& (const base_class& param) { base_class temp(Jand(obj, param.obj)); return temp; }
        base_class operator|| (const base_class& param) { base_class temp(Jor(obj, param.obj)); return temp; }

        // Bit-wise operators:
        base_class operator~ () { base_class temp(Jnot_bits(obj)); return temp; }
        base_class operator& (const base_class& param) { base_class temp(Jand_bits(obj, param.obj)); return temp; }
        base_class operator| (const base_class& param) { base_class temp(Jor_bits(obj, param.obj)); return temp; }
        base_class operator^ (const base_class& param) { base_class temp(Jxor_bits(obj, param.obj)); return temp; }
        
        /*
        Expression	Operator	                                Member function 	Non-member function
        @a	        + - * & ! ~ ++ --	                        A::operator@()  	operator@(A)
        a@	        ++ --	                                        A::operator@(int)	operator@(A,int)
        a@b	        + - * / % ^ & | < > == != <= >= << >> && || ,	A::operator@(B) 	operator@(A,B)
        a@b	        = += -= *= /= %= ^= &= |= <<= >>= []    	A::operator@(B) 	-
        a(b,c...)	()	                                        A::operator()(B,C...)	-
        a->b	        ->	                                        A::operator->() 	-
        (TYPE) a	TYPE	                                        A::operator TYPE()	-
        */

        // Math functions:
        object uminus() { return Juminus(obj); }
        object add(object b) { return Jadd(obj, b); }
        object minus(object b) { return Jminus(obj, b); }
        object multiply(object b) { return Jmultiply(obj, b); }
        object divide(object b) { return Jdivide(obj, b); }

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

        // 32-bit or 64-bit atom functions:
        object not_bits() { return Jnot_bits(obj); }
        object and_bits(object b) { return Jand_bits(obj, b); }
        object or_bits(object b) { return Jor_bits(obj, b); }
        object xor_bits(object b) { return Jxor_bits(obj, b); }


    };

    void Abort(elong error_level) { UserCleanup(error_level); }

    // Add more built-in and standard library functions here (Euphoria v3.1.1, v4.0.5, v4.1a)

}
