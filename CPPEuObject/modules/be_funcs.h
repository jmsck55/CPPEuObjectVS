// Copyright (c) 2022 James Cook
// be_funcs.h
//
// 32/64-bit using macro BITS64 for 64-bit

#ifndef _BE_FUNCS_H
#define _BE_FUNCS_H

//#include <time.h>

#define USE_STANDARD_LIBRARY
#include "../eu/be_eu.h"

// Uses files from Euphoria v4.0.5

#pragma pack(push)
#pragma pack(4)
typedef union TF
{
	double ieee_double;
	struct dbllong
	{
		unsigned int a;
		unsigned int b;
	} ieee_uint;
	unsigned char ieee_char[8];
} TF32_t;
#pragma pack(pop)

// be_runtime.c

MY_EXTERN_C long MY_DLL_SYMBOL seed1;
MY_EXTERN_C long MY_DLL_SYMBOL seed2;  /* current value of first and second random generators */
MY_EXTERN_C int MY_DLL_SYMBOL rand_was_set;   /* TRUE if user has called set_rand() */

MY_DLL_API object Date(); // This function uses: #include <time.h>

// Random functions (on Windows, it requires the "EWINDOWS" to be defined, such as "#define EWINDOWS")

// 32-bit random number generator:
#ifdef CLEANUP_MOD
object NewDouble32(double d);
#endif
object make_atom32(unsigned long c32);
#ifdef CLEANUP_MOD
object MakeDouble(d_ptr p);
object Convert32(object ob);
object NewDouble64(long double d);
object MakeLongDouble(d_ptr p);
object Convert64(object ob);

#endif // CLEANUP_MOD

MY_DLL_API void setran32();
//static ldiv_t my_ldiv (long int numer, long int denom);

MY_DLL_API unsigned long good_rand32();

/* random number from 1 to a */
/* a is a legal integer value */
MY_DLL_API object Random32(long a);

/* random number from 1 to a (a <= 1.07 billion) */
MY_DLL_API object DRandom32(d_ptr a);


MY_DLL_API object calc_hash32(object a, object b);

// be_machine.c

MY_DLL_API long get_int32(object x);
MY_DLL_API object get_rand32();
MY_DLL_API object set_rand32(object x);


#endif // _BE_FUNCS_H
