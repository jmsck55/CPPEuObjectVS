// Copyright (c) 2022 James Cook
//

#ifndef _ALLDEFS_H
#define _ALLDEFS_H

//#pragma once

// Default Modules:
#define USE_STDARG_H
#define CLEANUP_MOD

// User Modules:

#define USE_STANDARD_LIBRARY
#define USE_MATH_H
#define USE_QUADMATH_H


// On Windows, use Intel's compiler with options: /Qoption,cpp,--extended_float_types /Qlong-double 
// On GCC, use -lquadmath compiler option.

#ifndef __GNUC__
#define EWINDOWS
#endif

#ifdef __cplusplus
#define MY_EXTERN_C extern "C"
#else
#define MY_EXTERN_C extern
#endif

#ifdef BUILDING_DLL
#ifndef DONE_DEBUGGING
#define DONE_DEBUGGING
#endif
#ifdef __GNUC__
#define MY_DLL_SYMBOL
#else
#define MY_DLL_SYMBOL __declspec(dllexport)
#endif
#define MY_DLL_API MY_EXTERN_C MY_DLL_SYMBOL
#else
#ifdef USING_DLL
#ifndef DONE_DEBUGGING
#define DONE_DEBUGGING
#endif
#ifdef __GNUC__
#define MY_DLL_SYMBOL
#else
#define MY_DLL_SYMBOL __declspec(dllimport)
#endif
#define MY_DLL_API MY_EXTERN_C MY_DLL_SYMBOL
#else
#define MY_DLL_API MY_EXTERN_C
#define MY_DLL_SYMBOL
//#define MY_DLL_API
//#define MY_DLL_SYMBOL
#endif // USING_DLL
#endif // BUILDING_DLL

// Use when debugging source code:
#ifndef DONE_DEBUGGING
#define EXTRA_CHECK
#endif

#include <stdint.h>
#include <float.h>

#if INTPTR_MAX == INT64_MAX
#define BITS64
#endif

#if DBL_DIG == LDBL_DIG
#define EUDOUBLE_WIDTH "15"
#else
#define EUDOUBLE_WIDTH "18"
#endif

#ifdef BITS64
#define ELONG long
#define REGISTER
#define ELONG_WIDTH "ll"
#else
#define ELONG
#define REGISTER
// Old compilers can use this:
//#define REGISTER register
#define ELONG_WIDTH "l"
#endif

// For faster code, alignment should be (2 on 16-bit machines), (4 on 32-bit machines), (8 on 64-bit machines)
typedef double eudouble32;
typedef int elong32;
typedef unsigned int eulong32;

typedef ELONG double eudouble;
typedef ELONG long elong;
typedef unsigned ELONG long eulong;

typedef ELONG long int integer;
typedef elong object;
typedef object* object_ptr;

typedef struct s1 {                        /* a sequence header block */
    object_ptr base;               /* pointer to (non-existent) 0th element */
    elong length;                   /* number of elements */
    elong ref;                      /* reference count */
    elong postfill;                 /* number of post-fill objects */
} *s1_ptr; /* total 8*4=32 bytes */

#ifdef USE_QUADMATH_H
typedef __float128 equadmath;
#endif

/*
* float // 4 bytes
* double // 8 bytes
*
* long double // 16 bytes
* float128 // 16 bytes
*/

#ifdef BITS64
// CLEANUP_MOD is required on 64-bit, because of 16 byte (128-bit) boundaries.
#define CLEANUP_MOD
typedef struct d {                         /* a long double precision number */
    union {
#ifdef USE_QUADMATH_H
        // USE_QUADMATH_H requires CLEANUP_MOD on 64-bit
        equadmath quad;
#endif // USE_QUADMATH_H
        eudouble dbl;                    /* long double precision value, float64 or float80 */
        struct {
            union {
                double a_dbl; // an actual double on 64-bit platforms, i.e. MSVC compiler.
                void* ptr; // could be a pointer to an array of structures, customize in "cleanup's flags"
            };
            eulong type; // could be length, perhaps high bits could be flags.
        };
        long double ldbl;
    };
    elong ref;                      /* reference count */
#ifdef CLEANUP_MOD
// Both "struct s1" and "struct d" take up 32-bytes on 64-bit platforms.
    object cleanup; // should be set to zero (0), type of data, and pointer for the cleanup routine.
        // IS_ATOM_INT means it is a dbl==0 or "signed type"=1 "unsigned type"==2 or "double a_dbl"==3 or "data of length type"==4 or "float128"==5 (ptr on 32-bit) (other data types can be implimented in the future.)
        // IS_ATOM_DBL means it is a pointer ("ptr") of "type" with pointer to "cleanup" routine.
        // IS_SEQUENCE means it is an array ("ptr") of length "type" (without postfill variable) with pointer to "cleanup" routine.
#endif // CLEANUP_MOD
} /* total 8*3=24 bytes, or 8*4=32 bytes on newer GCC 64-bit */
#else // BITS32:
typedef struct d {                         /* a long double precision number */
    union {
        eudouble dbl;                    /* long double precision value */
        struct {
            union {
                double a_dbl; // an actual double on 64-bit platforms, i.e. MSVC compiler.
                long double* ldbl;
#ifdef USE_QUADMATH_H
                equadmath* qdbl;
#endif // USE_QUADMATH_H
                void* ptr; // could be a pointer to an array of structures, customize in "cleanup's flags"
            };
            eulong type; // could be length, perhaps high bits could be flags.
        };
    };
    elong ref;                      /* reference count */
// CLEANUP_MOD is required on 64-bit, because of 16 byte (128-bit) boundaries.
#ifdef CLEANUP_MOD
// Both "struct s1" and "struct d" take up 32-bytes on 64-bit platforms.
    object cleanup; // should be set to zero (0), type of data, and pointer for the cleanup routine.
        // IS_ATOM_INT means it is a dbl==0 or "signed type"=1 "unsigned type"==2 or "double a_dbl"==3 or "data of length type"==4 or "float128"==5 (ptr on 32-bit) (other data types can be implimented in the future.)
        // IS_ATOM_DBL means it is a pointer ("ptr") of "type" with pointer to "cleanup" routine.
        // IS_SEQUENCE means it is an array ("ptr") of length "type" (without postfill variable) with pointer to "cleanup" routine.
#endif // CLEANUP_MOD
} /* total 8*3=24 bytes, or 8*4=32 bytes on newer GCC 64-bit */
#endif // BITS64
* d_ptr;
#define D_SIZE (sizeof(struct d))

//#ifdef CLEANUP_MOD
typedef void (*TypeCleanupFunc)(void* ptr, elong type, object cleanup);
//#endif // CLEANUP_MOD
#define CLEANUP_DEFAULT 0
#define CLEANUP_SIGNED_TYPE 1
#define CLEANUP_UNSIGNED_TYPE 2
#define CLEANUP_DOUBLE 3
#define CLEANUP_LONGDOUBLE 4
#define CLEANUP_FLOAT128 5
// Data is handled the same as String (non-null terminated string of length 'type')
// CStrings should still terminate their string with a null character value.
// When there are no null characters in a string, it should be null-terminated.
#define CLEANUP_DATA -1
#define CLEANUP_STRING -1
// Non MSVC compilers:
#define USER_CLEANUP_START 32
// Any negative number in 'cleanup' types means that it gets freed by the program.

#define FALSE 0
#define TRUE 1

// execute.h (from Euphoria v4.0.5)

          /* Euphoria object format v1.2 and later */

/* an object is represented as a 32-bit value as follows:

        unused  : 011xxxxx xxxxxxxx xxxxxxxx xxxxxxxx
        unused  : 010xxxxx xxxxxxxx xxxxxxxx xxxxxxxx

        TOO_BIG:  01000000 00000000 00000000 00000000   (just too big for INT)

       +ATOM-INT: 001vvvvv vvvvvvvv vvvvvvvv vvvvvvvv   (31-bit integer value)
       +ATOM-INT: 000vvvvv vvvvvvvv vvvvvvvv vvvvvvvv   (31-bit integer value)
       -ATOM-INT: 111vvvvv vvvvvvvv vvvvvvvv vvvvvvvv   (31-bit integer value)
       -ATOM-INT: 110vvvvv vvvvvvvv vvvvvvvv vvvvvvvv   (31-bit integer value)

        NO VALUE: 10111111 11111111 11111111 11111111   (undefined object)

        ATOM-DBL: 101ppppp pppppppp pppppppp pppppppp   (29-bit pointer)

        SEQUENCE: 100ppppp pppppppp pppppppp pppppppp   (29-bit pointer)

   We ensure 8-byte alignment for s1 and dbl blocks - lower 3 bits
   aren't needed - only 29 bits are stored.
*/

/* NO VALUE objects can occur only in a few well-defined places,
   so we can simplify some tests. For speed we first check for ATOM-INT
   since that's what most objects are. */

#define MAKE_UINT32(x) ((object)(( ((unsigned long)x) <= ((unsigned long)0x3FFFFFFFL)) \
                          ? (unsigned long)x : \
                            (unsigned long)NewDouble32((double)(unsigned long)x)))

   /* these are obsolete */
#define INT_VAL32(x)        ((int)(x))
#define MAKE_INT32(x)       ((object)(x))

#ifdef BITS64
#define NOVALUE      ((long long)0xbfffffffffffffffLL)
#define TOO_BIG_INT  ((long long)0x4000000000000000LL)
#define HIGH_BITS    ((long long)0xC000000000000000LL)
#define IS_ATOM_INT(ob)       (((long long)(ob)) > NOVALUE)
#define IS_ATOM_INT_NV(ob)    ((long long)(ob) >= NOVALUE)

#define MAKE_UINT(x) ((object)(( ((unsigned long long)x) <= ((unsigned long long)0x3FFFFFFFFFFFFFFFLL)) \
                          ? (unsigned long long)x : \
                            (unsigned long long)NewDouble((long double)(unsigned long long)x)))

/* these are obsolete */
#define INT_VAL(x)        ((long long int)(x))
#define MAKE_INT(x)       ((object)(x))
#else
#define NOVALUE      ((long)0xbfffffffL)
#define TOO_BIG_INT  ((long)0x40000000L)
#define HIGH_BITS    ((long)0xC0000000L)
#define IS_ATOM_INT(ob)       (((long)(ob)) > NOVALUE)
#define IS_ATOM_INT_NV(ob)    ((long)(ob) >= NOVALUE)
#define MAKE_UINT(x) MAKE_UINT32(x)
#define INT_VAL(x) INT_VAL32(x)
#define MAKE_INT(x) MAKE_INT32(x)
#endif // BITS64

/* N.B. the following distinguishes DBL's from SEQUENCES -
   must eliminate the INT case first */
#ifdef BITS64
#define IS_ATOM_DBL(ob)         (((object)(ob)) >= (long long)0xA000000000000000LL)

#define IS_ATOM(ob)             (((long long)(ob)) >= (long long)0xA000000000000000LL)
#define IS_SEQUENCE(ob)         (((long long)(ob))  < (long long)0xA000000000000000LL)

#define ASEQ(s) (((unsigned long long)s & (unsigned long long)0xE000000000000000LL) == (unsigned long long)0x8000000000000000LL)

#define IS_DBL_OR_SEQUENCE(ob)  (((long long)(ob)) < NOVALUE)
#else
#define IS_ATOM_DBL(ob)         (((object)(ob)) >= (long)0xA0000000L)

#define IS_ATOM(ob)             (((long)(ob)) >= (long)0xA0000000L)
#define IS_SEQUENCE(ob)         (((long)(ob))  < (long)0xA0000000L)

#define ASEQ(s) (((unsigned long)s & (unsigned long)0xE0000000L) == (unsigned long)0x80000000L)

#define IS_DBL_OR_SEQUENCE(ob)  (((long)(ob)) < NOVALUE)
#endif // BITS64
#define MIN_INT32     (long)0xC0000000L
#define MAX_INT32     (long)0x3FFFFFFFL
#define MININT_VAL32 MIN_INT32
#define MININT_DBL32 ((double)MININT_VAL32)
#define MAXINT_VAL32 MAX_INT32
#define MAXINT_DBL32 ((double)MAXINT_VAL32)
#define INT23      (long)0x003FFFFFL
#define EINT16      (long)0x00007FFFL
#define INT15      (long)0x00003FFFL
#define ATOM_M1    -1
#define ATOM_0     0
#define ATOM_1     1
#define ATOM_2     2
#ifdef BITS64
#undef MININT
#define MININT     (long long)0xC000000000000000LL
#define MAXINT     (long long)0x3FFFFFFFFFFFFFFFLL
#define MININT_VAL MININT
#define MININT_DBL ((long double)MININT_VAL)
#define MAXINT_VAL MAXINT
#define MAXINT_DBL ((long double)MAXINT_VAL)
#define INT55      (long long)0x003FFFFFFFFFFFFFLL
#define INT47      (long long)0x00003FFFFFFFFFFFLL
#define INT31      (long long)0x000000003FFFFFFFLL
#else
#undef MININT
#define MININT MIN_INT32
#define MAXINT MAX_INT32
#define MININT_VAL MININT_VAL32
#define MININT_DBL MININT_DBL32
#define MAXINT_VAL MAXINT_VAL32
#define MAXINT_DBL MAXINT_DBL32
#endif

#ifdef BITS64
#define MAKE_DBL(x) ( (object) (((unsigned long long)(x) >> 3) + (long long)0xA000000000000000LL) )
#define DBL_PTR(ob) ( (d_ptr)  (((long long)(ob)) << 3) )
#define MAKE_SEQ(x) ( (object) (((unsigned long long)(x) >> 3) + (long long)0x8000000000000000LL) )
#define SEQ_PTR(ob) ( (s1_ptr) (((long long)(ob)) << 3) )
   /* ref a double or a sequence (both need same 3 bit shift) */
#define RefDS(a) ++(DBL_PTR(a)->ref)

/* ref a general object */
#define Ref(a) if (IS_DBL_OR_SEQUENCE(a)) { RefDS(a); }

/* de-ref a double or a sequence */
#define DeRefDS(a) if (--(DBL_PTR(a)->ref) == 0 ) { de_reference((s1_ptr)(a)); }
/* de-ref a double or a sequence in x.c and set tpc (for time-profile) */
#define DeRefDSx(a) if (--(DBL_PTR(a)->ref) == 0 ) {tpc=pc; de_reference((s1_ptr)(a)); }

/* de_ref a sequence already in pointer form */
#define DeRefSP(a) if (--((s1_ptr)(a))->ref == 0 ) { de_reference((s1_ptr)MAKE_SEQ(a)); }

/* de-ref a general object */
#define DeRef(a) if (IS_DBL_OR_SEQUENCE(a)) { DeRefDS(a); }
/* de-ref a general object in x.c and set tpc (for time-profile) */
#define DeRefx(a) if (IS_DBL_OR_SEQUENCE(a)) { DeRefDSx(a); }

#define UNIQUE(seq) (((s1_ptr)(seq))->ref == 1)
#else
#define MAKE_DBL(x) ( (object) (((unsigned long)(x) >> 3) + (long)0xA0000000L) )
#define DBL_PTR(ob) ( (d_ptr)  (((long)(ob)) << 3) )
#define MAKE_SEQ(x) ( (object) (((unsigned long)(x) >> 3) + (long)0x80000000L) )
#define SEQ_PTR(ob) ( (s1_ptr) (((long)(ob)) << 3) )
   /* ref a double or a sequence (both need same 3 bit shift) */
#define RefDS(a) ++(DBL_PTR(a)->ref)

/* ref a general object */
#define Ref(a) if (IS_DBL_OR_SEQUENCE(a)) { RefDS(a); }

/* de-ref a double or a sequence */
#define DeRefDS(a) if (--(DBL_PTR(a)->ref) == 0 ) { de_reference((s1_ptr)(a)); }
/* de-ref a double or a sequence in x.c and set tpc (for time-profile) */
#define DeRefDSx(a) if (--(DBL_PTR(a)->ref) == 0 ) {tpc=pc; de_reference((s1_ptr)(a)); }

/* de_ref a sequence already in pointer form */
#define DeRefSP(a) if (--((s1_ptr)(a))->ref == 0 ) { de_reference((s1_ptr)MAKE_SEQ(a)); }

/* de-ref a general object */
#define DeRef(a) if (IS_DBL_OR_SEQUENCE(a)) { DeRefDS(a); }
/* de-ref a general object in x.c and set tpc (for time-profile) */
#define DeRefx(a) if (IS_DBL_OR_SEQUENCE(a)) { DeRefDSx(a); }

#define UNIQUE(seq) (((s1_ptr)(seq))->ref == 1)
#endif // BITS64

#define TEMP_SIZE 1040  // size of TempBuff - big enough for 1024 image

#define EXTRA_EXPAND(x) (4 + (x) + ((x) >> 2))

#define MAX_BITWISE_DBL32 ((double)(unsigned long)0xFFFFFFFFL)
#define MIN_BITWISE_DBL32 ((double)(signed long)  0x80000000L)
#ifdef BITS64
#define MAX_BITWISE_DBL ((long double)(unsigned long long)0xFFFFFFFFFFFFFFFFLL)
#define MIN_BITWISE_DBL ((long double)(signed long long)  0x8000000000000000LL)
#else
#define MAX_BITWISE_DBL MAX_BITWISE_DBL32
#define MIN_BITWISE_DBL MIN_BITWISE_DBL32
#endif // BITS64

/* .dll argument & return value types */
#define C_TYPE     0x0F000000
#define C_DOUBLE   0x03000008
#define C_FLOAT    0x03000004
#define C_CHAR     0x01000001
#define C_UCHAR    0x02000001
#define C_SHORT    0x01000002
#define C_USHORT   0x02000002
#define E_INTEGER  0x06000004
#define E_ATOM     0x07000004
#define E_SEQUENCE 0x08000004
#define E_OBJECT   0x09000004
#define C_INT      0x01000004
#define C_UINT     0x02000004
#define C_LONG     0x01000008
#define C_ULONG    0x02000008
#define C_POINTER  0x03000001
#define C_LONGLONG 0x03000002

#define C_FLOAT80  0x0300000A
#define C_FLOAT128 0x03000010
#define E_FLOAT128 0x0A000010

#define C_STDCALL 0
#define C_CDECL 1

// alloc.h
#define FreeD(p) free(p)

// be_alloc.c
#define EFree(p) free(p)

#ifndef DONE_DEBUGGING
#define SET_DEBUG \
    lookatd = NULL; \
    lookats = NULL; \
    if (IS_DBL_OR_SEQUENCE(obj)) \
    { \
        if (IS_ATOM_DBL(obj)) \
        { \
            lookatd = DBL_PTR(obj); \
        } \
        else \
        { \
            lookats = SEQ_PTR(obj); \
        } \
    }
#else
#define SET_DEBUG
#endif

#endif // _ALLDEFS_H