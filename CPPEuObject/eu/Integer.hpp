// Copyright (c) 2022 James Cook
//
// An Object is either an Atom or a Sequence.
// A Sequence is an array of Objects.
// An Atom is either a [long] double or an Integer.
// An Integer is a 31-bit (on 32-bit) or 63-bit (on 64-bit) signed number.

#pragma once

#include "base_class.hpp"

#define TYPE_CHECK_INTEGER(val) (((val) > NOVALUE) && ((val) < TOO_BIG_INT))

namespace eu
{
    class Integer : public base_class // Integer inherits base_class data and functions.
    {
    private:
        //object obj;
    protected:
        //object obj;
    public:
// #ifndef DONE_DEBUGGING
//         inline static d_ptr lookatd;
//         inline static s1_ptr lookats;
// #endif
        Integer() { obj = NOVALUE; SET_DEBUG } // default constructor
        ~Integer() { SET_DEBUG DeRef(obj) obj = NOVALUE; } // default destructor
        Integer(const Integer& x) { obj = x.obj; Ref(obj) SET_DEBUG } // copy constructor
        Integer& operator= (const Integer& x) { DeRef(obj) obj = x.obj; Ref(obj) SET_DEBUG return *this; } // copy assignment
    // On newer compilers:
        Integer(Integer&& x) noexcept { obj = x.obj; x.obj = NOVALUE; SET_DEBUG } // move constructor
        Integer& operator= (Integer&& x) noexcept { DeRef(obj) obj = x.obj; x.obj = NOVALUE; SET_DEBUG return *this; } // move assignment
    // End On newer compilers.
        //Integer(d_ptr a) { obj = MAKE_DBL(a); SET_DEBUG }
        //Integer(s1_ptr a) { obj = MAKE_SEQ(a); SET_DEBUG }
        Integer(object a) { obj = a; SET_DEBUG }

        void NewInteger(elong val) { DeRef(obj); obj = TYPE_CHECK_INTEGER(val) ? val : NOVALUE; SET_DEBUG }
        elong GetInteger() { SET_DEBUG return TYPE_CHECK_INTEGER(obj) ? obj : NOVALUE; }


        void print(int ascii = 0, const char int_format[] = "%" ELONG_WIDTH "d", int char_min = 32, int char_max = 127) {
            if (!IS_ATOM_INT(obj)) {
                RTFatal("Expected an integer in Integer::print().");
            }
            printf(int_format, obj);
            if (ascii == 1) {
                if (obj >= char_min && obj <= char_max) {
                    printf("'%c'", (int)obj);
                }
            }
        }
    };

}
