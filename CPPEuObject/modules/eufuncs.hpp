// Copyright (c) 2022 James Cook
// eufuncs.hpp
// Additional Functions Classfile for EuObject
//
// See also: obj.hpp

// Includes just the date() and random() routines:

#pragma once

#define USE_STANDARD_LIBRARY
//#include "base_class.hpp"

namespace eu
{
    extern "C" {
        #include "be_funcs.h"
    }


    //object Date(); // Already defined in "be_funcs.h" and "be_funcs.c"

    // Random functions (on Windows, it requires the "EWINDOWS" to be defined, such as "#define EWINDOWS")
    
    // get_rand32() and set_rand32() already defined in "be_funcs.c"
    
    object Rand(object a)
    {
        if (IS_ATOM_INT(a))
        {
            return Random32(a);
        }
        if (IS_ATOM_DBL(a))
        {
            return DRandom32(DBL_PTR(a));
        }
        else
        {
            object_ptr sb, cb;
            s1_ptr sp, cp;
            elong len;
            object temp;
            sp = SEQ_PTR(a);
            len = sp->length;
            if (len < 0)
            {
                RTFatal("Expected length to be greater than 0, in Rand()");
            }
            cp = NewS1(len);
            sb = sp->base;
            cb = cp->base;
            while (len-- != 0)
            {
                temp = Rand(*(++sb));
                *(++cb) = temp;
            }
            return MAKE_SEQ(cp);
        }
    }

}
