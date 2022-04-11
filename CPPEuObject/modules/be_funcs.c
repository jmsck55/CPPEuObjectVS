// Copyright (c) 2022 James Cook
// be_funcs.c
//
// 32/64-bit using macro BITS64 for 64-bit

//#include "pch.h"
#include "be_funcs.h"

#ifdef __GNUC__
#define LOCALTIME(ret, ptr) ret = localtime(ptr);
#else
#define LOCALTIME(ret, ptr) errno_t err = localtime_s(ret, ptr); if (err) RTFatal("Error with 'localtime_s()'");
#endif

// From be_runtime.c

long seed1, seed2;  /* current value of first and second random generators */
int rand_was_set = FALSE;   /* TRUE if user has called set_rand() */

// Date() function:

// This function uses: #include <time.h>
object Date()
/* returns year, month, day, hour, minute, second */
{
    object_ptr obj_ptr;
    s1_ptr result;
    time_t time_of_day;
    struct tm *local = NULL;

    time_of_day = time(NULL);
    LOCALTIME(local, &time_of_day)
    //local = localtime(&time_of_day);
    result = NewS1(8);
    obj_ptr = result->base;
    obj_ptr[1] = MAKE_INT(local->tm_year);
    obj_ptr[2] = MAKE_INT(local->tm_mon+1);
    obj_ptr[3] = MAKE_INT(local->tm_mday);
    obj_ptr[4] = MAKE_INT(local->tm_hour);
    obj_ptr[5] = MAKE_INT(local->tm_min);
    obj_ptr[6] = MAKE_INT(local->tm_sec);
    obj_ptr[7] = MAKE_INT(local->tm_wday+1);
    obj_ptr[8] = MAKE_INT(local->tm_yday+1);
    return MAKE_SEQ(result);
}

// Random function 32-bit: (from Euphoria v4.0.5)

// On 32-bit, you need to define 'CLEANUP_MOD' in order to use this function:
#ifdef CLEANUP_MOD
object NewDouble32(double d)
/* allocate space for a new double value */
{
    REGISTER d_ptr n;

    n = (d_ptr)EMalloc((elong)D_SIZE);
    n->ref = 1;
#ifdef BITS64
    n->type = 0; // jjc, allow extended types, only use high bits.
#endif
    n->cleanup = CLEANUP_DOUBLE; // jjc
    n->a_dbl = d; // actual 'double', not 'long double'
    return MAKE_DBL(n);
}
#else
#ifndef BITS64
// Without CLEANUP_MOD, under 32-bit:
#define NewDouble32 NewDouble
#define NewDouble64 NewDouble
#endif
#endif

object make_atom32(unsigned long c32)
/* make a Euphoria atom from an unsigned C value */
{
    if (c32 <= (unsigned long)0x3FFFFFFFL)
        return c32;
    else
        return NewDouble32((double)c32);
}

#ifdef CLEANUP_MOD
object MakeDouble(d_ptr p)
// Convert a d_ptr to a double, truncating long doubles
{
        if (IS_ATOM_INT(p->cleanup))
        {
                if (p->cleanup == CLEANUP_DEFAULT)
                {
                        return NewDouble32((double)p->dbl);
                }
#ifndef BITS64
// 32-bit machine
                else if (p->cleanup == CLEANUP_LONGDOUBLE)
                {
                        return NewDouble32((double)*p->ldbl);
                }
#endif
        }
        p->ref++; // increase ref count by one, becuase we are creating a new atom.
        return MAKE_DBL(p);
}

object Convert32(object ob)
// Convert to 32-bit machine variables
{
        if (IS_ATOM_INT(ob))
        {
                return make_atom32((unsigned long)ob);
        }
        else if (IS_ATOM_DBL(ob))
        {
                return MakeDouble(DBL_PTR(ob));
        }
        else if (IS_SEQUENCE(ob))
        {
                s1_ptr p = SEQ_PTR(ob);
                object_ptr cp, sp = p->base;
                elong len = p->length;
                p = NewS1(len);
                cp = p->base; // cp is converted sequence pointer.
                while (len-- > 0)
                {
                        *(++cp) = Convert32(*(++sp));
                }
                return MAKE_SEQ(p);
        }
        else
        {
                RTFatal("Expected a valid object in Convert32().");
                return NOVALUE;
        }
}

// With CLEANUP_MOD:
object NewDouble64(long double d)
/* allocate space for a new long double value */
{
    REGISTER d_ptr n;

    n = (d_ptr)EMalloc((elong)D_SIZE);
    n->ref = 1;
#ifdef BITS64
    n->type = 0; // jjc, allow extended types, only use high bits.
    n->cleanup = (sizeof(double) == sizeof(eudouble)) ? CLEANUP_DOUBLE : CLEANUP_DEFAULT; // jjc
    n->dbl = d; // 'long double'
#else // 32-bits:
#ifdef __GNUC__
    n->pldbl = (long double*)EMalloc(sizeof(long double));
    n->type = C_FLOAT80;
    n->cleanup = CLEANUP_LONGDOUBLE; // jjc
    *n->pldbl = d;
#else // !__GNUC__
    //n->type = 0; // jjc, allow extended types, only use high bits.
    n->cleanup = CLEANUP_DEFAULT; // jjc, default for 32-bit.
    n->dbl = d; // Not a real 'long double'
#endif // __GNUC__
#endif // BITS64
    return MAKE_DBL(n);
}

object MakeLongDouble(d_ptr p)
// Convert a d_ptr to a long double
{
        if (IS_ATOM_INT(p->cleanup))
        {
                if (p->cleanup == CLEANUP_DOUBLE)
                {
#ifdef BITS64
                        long double ld = (long double)p->a_dbl;
                        p->dbl = ld;
                        p->cleanup = CLEANUP_DEFAULT;
#else
                        return NewDouble64((long double)p->dbl);
#endif
                }       
        }
        p->ref++; // increase ref count by one, becuase we are creating a new atom.
        return MAKE_DBL(p);
}

object Convert64(object ob)
// Convert back to 64-bit machine variables.
{
        if (IS_ATOM_INT(ob))
        {
                return ob;
        }
        else if (IS_ATOM_DBL(ob))
        {
                return MakeDouble(DBL_PTR(ob));
        }
        else if (IS_SEQUENCE(ob))
        {
                s1_ptr p = SEQ_PTR(ob);
                object_ptr cp, sp = p->base;
                elong len = p->length;
                p = NewS1(len);
                cp = p->base; // cp is converted sequence pointer.
                while (len-- > 0)
                {
                        *(++cp) = Convert32(*(++sp));
                }
                return MAKE_SEQ(p);
        }
        else
        {
                RTFatal("Expected a valid object in Convert32().");
                return NOVALUE;
        }
}
#endif

#define V32(a,b) ((((a) << 1) | (a & 0x1)) ^ ((((b) >> 14) & 0x0000FFFF) | ((b) << 18)))

#define prim1 ((long)2147483563L)
#define prim2 ((long)2147483399L)

#define root1 ((long)40014L)
#define root2 ((long)40692L)

#define quo1 ((long)53668L)  /* prim1 / root1 */
#define quo2 ((long)52774L)  /* prim2 / root2 */

#define rem1 ((long)12211L)  /* prim1 % root1 */
#define rem2 ((long)3791L)   /* prim2 % root2 */

/* set random seed1 and seed2 - neither can be 0 */
void setran32()
{
        time_t time_of_day;
        struct tm *local = NULL;
#if !defined( EWINDOWS )
        long garbage;
#endif
        static long src = prim1 ^ prim2;

        time_of_day = time(NULL);
        LOCALTIME(local, &time_of_day)
        //local = localtime(&time_of_day);
        seed2 = (long)(local->tm_yday * 86400 + local->tm_hour * 3600 +
                        local->tm_min * 60 +     local->tm_sec);
#ifdef EWINDOWS
        seed1 = (long)(GetTickCount() + src);  // milliseconds since Windows started
#else
        seed1 = (unsigned long)(&garbage) + random() + src;
#endif
        src += 1; // src is static, so this line is needed.
        good_rand32();  // skip first one, second will be more random-looking
}

static ldiv_t my_ldiv32(long int numer, long int denom)
{
        ldiv_t result;

        result.quot = numer / denom;
        result.rem = numer % denom;

        if (numer >= 0 && result.rem < 0)   {
                ++result.quot;
                result.rem -= denom;
        }

        return result;
}

unsigned long good_rand32()
/* Public Domain random number generator from USENET posting */
{
        ldiv_t temp;
        long remval, quotval;

        if (!rand_was_set && seed1 == 0 && seed2 == 0) {
                // First time thru.
                setran32();
        }

        /* seed = seed * ROOT % PRIME */
        temp = my_ldiv32(seed1, quo1);
        remval = root1 * temp.rem;
        quotval = rem1 * temp.quot;

        /* normalize */
        seed1 = remval - quotval;
        if (remval <= quotval)
                seed1 += prim1;

        temp = my_ldiv32(seed2, quo2);
        remval = root2 * temp.rem;
        quotval = rem2 * temp.quot;

        seed2 = remval - quotval;
        if (remval <= quotval)
                seed2 += prim2;

        if (seed1 == 0) {
                seed1 = prim2;
        }
        if (seed2 == 0)
                seed2 = prim1;

        return V32(seed1, seed2);
}

object Random32(long a)
/* random number from 1 to a */
/* a is a legal integer value */
{
        if (a <= 0)
                RTFatal("argument to rand must be >= 1");
        return MAKE_INT((good_rand32() % (unsigned)a) + 1);
}


object DRandom32(d_ptr a)
/* random number from 1 to a (a <= 1.07 billion) */
{
        unsigned long res;
        double a_dbl;

#ifdef BITS64
        if (a->cleanup == CLEANUP_DOUBLE)
            a_dbl = a->a_dbl;
        else
#endif
            a_dbl = (double)a->dbl;

        if (a_dbl < 1.0)
                RTFatal("argument to rand must be >= 1");
        if ((unsigned)(a_dbl) == 0)
                RTFatal("argument to rand is too large");
        res = (1 + good_rand32() % (unsigned)(a_dbl));
        return MAKE_UINT(res);
}

/* When hashing, we treat doubles differently from integers.  But if a
 * double can be represented as an integer, we want to use that.
 */
#define IS_DOUBLE_AN_INTEGER32( X ) \
if( !IS_ATOM_INT( X ) && IS_ATOM( X ) ){ \
        double TMP_dbl = (double)DBL_PTR( X )->a_dbl; \
        if( TMP_dbl == (double)(object)TMP_dbl ){\
                X = (object)TMP_dbl;\
        }\
}\

#define rol32(a,b) ((((long)a) << (long)b) | (((long)a) >> (32 - (long)b)))
object calc_hash32(object a, object b)
// 32-bit hash function.
/* calculate the hash value of object a.
   b influences the style of hash calculated.
   b ==> -1 SHA256
   b ==> -2 MD5
   b ==> -3 Fletcher-32
   b ==> -4 Adler-32
   b ==> -5 Hsieh-32
   b ==> >=0 and  <1 69096 + b
   b ==> >=1 hash = (hash * b + x)

*/
{
        unsigned long lHashValue;
        long lSLen;


        int tfi;
        object lTemp;

        // union TF // defined in header for data members to be 4-byte aligned
        // {
        //         double ieee_double;
        //         struct dbllong
        //         {
        //                 unsigned int a;
        //                 unsigned int b;
        //         } ieee_uint;
        //         unsigned char ieee_char[8];
        // }
        TF32_t tf, seeder, prev;

        object_ptr ap, lp;
        object av, lv;
#ifdef CLEANUP_MOD
        a = Convert32(a); // need to be DeRef'ed
        b = Convert32(b); // need to be DeRef'ed
#endif
        IS_DOUBLE_AN_INTEGER32(a)
        if (IS_ATOM_INT(b)) {
                // if (b == -6)
                //      return calc_hsieh30(a); // Will always return a Euphoria integer.
                //
                // if (b == -5)
                //      return make_atom32(calc_hsieh32(a));
                //
                // if (b == -4)
                //      return make_atom32(calc_adler32(a));
                //
                // if (b == -3)
                //      return make_atom32(calc_fletcher32(a));
                //
                // if (b == -2)
                //      return calc_MD5(a);
                //
                // if (b == -1)
                //      return calc_SHA256(a);

                if (b < 0)
                        RTFatal("second argument of hash() must not be a negative integer.");

                if (b == 0)
                {
                        if (IS_ATOM_INT(a)) {
                                tf.ieee_double = 69096.0 + (double)a;
                        }
                        else if (IS_ATOM_DBL(a)) {
                                tf.ieee_double = 3690961.0 + (double)(DBL_PTR(a)->a_dbl);
                        }
                        else {
                                tf.ieee_double = 196069.10 + (double)(SEQ_PTR(a)->length);
                        }
                        tf.ieee_uint.a &= (long)0x3FFFFFFFL;
                        if (tf.ieee_uint.a == 0) {
                                tf.ieee_uint.a = (long)0x3FFFFFFFL;
                        }

                        lTemp = calc_hash32(a, (object)tf.ieee_uint.a);

                        if (IS_ATOM_INT(lTemp)) {
                                seeder.ieee_uint.a = lTemp;
                                seeder.ieee_uint.b = rol32(lTemp, 15);
                        }
                        else {
                                seeder.ieee_double = (double)(DBL_PTR(lTemp)->a_dbl);
                        }
                        DeRef(lTemp);
                }
                else {
                        seeder.ieee_uint.a = b;
                        seeder.ieee_uint.b = rol32(b, 15);
                }
        }
        else if (IS_ATOM_DBL(b)) {
                seeder.ieee_double = (double)(DBL_PTR(b)->a_dbl);
        }
        else {
                lTemp = calc_hash32(b, 16063 + (unsigned int)(SEQ_PTR(b)->length));
                if (IS_ATOM_INT(lTemp)) {
                        seeder.ieee_uint.a = lTemp;
                        seeder.ieee_uint.b = rol32(lTemp, 15);
                }
                else {
                        seeder.ieee_double = (double)(DBL_PTR(lTemp)->a_dbl);
                }
                DeRef(lTemp);
        }

        lHashValue = 0x193A74F1;

        for(tfi = 0; tfi < 8; tfi++)
        {
                if (seeder.ieee_char[tfi] == 0)
                        seeder.ieee_char[tfi] = (unsigned char)(tfi * 171 + 1);
                seeder.ieee_char[tfi] += (tfi + 1) << 8;
                lHashValue = rol32(lHashValue, 3) ^ seeder.ieee_char[tfi];
        }

        if (IS_ATOM_INT(a)) {
                tf.ieee_uint.a = a;
                tf.ieee_uint.b = rol32(a, 15);
                for(tfi = 0; tfi < 8; tfi++)
                {
                        if (tf.ieee_char[tfi] == 0)
                                tf.ieee_char[tfi] = (unsigned char)(tfi * 171 + 1);
                        lHashValue = rol32(lHashValue, 3) ^ ((tf.ieee_char[tfi] + (tfi + 1)) << 8);
                }
        }
        else if (IS_ATOM_DBL(a)) {
                tf.ieee_double = ((double)(DBL_PTR(a)->a_dbl));
                for(tfi = 0; tfi < 8; tfi++)
                {
                        if (tf.ieee_char[tfi] == 0)
                                tf.ieee_char[tfi] = (unsigned char)(tfi * 171 + 1);
                        lHashValue = rol32(lHashValue, 3) ^ ((tf.ieee_char[tfi] + (tfi + 1)) << 8);
                }
        }
        else { /* input is a sequence */
                lSLen = SEQ_PTR(a)->length;
                lHashValue += lSLen + 3;
                ap = SEQ_PTR(a)->base;
                lp = ap + lSLen + 1;
                while (lSLen > 0) {
                        av = *(++ap);
                        lv = *(--lp);
                        if (av == NOVALUE) {
                                break;  // we hit the end marker
                        }

                        for(tfi = 0; tfi < 8; tfi++)
                        {
                                lHashValue = rol32(lHashValue, 3) ^ seeder.ieee_char[tfi];
                        }

                        IS_DOUBLE_AN_INTEGER32( lv )
                        if (IS_ATOM_INT(lv)) {
                                prev.ieee_uint.a = lv;
                                prev.ieee_uint.b = rol32(lv, 15);
                        }
                        else if (IS_ATOM_DBL(lv)) {
                                prev.ieee_double = (double)(DBL_PTR(lv)->a_dbl);
                        }
                        else {
                                lv = (unsigned int)(SEQ_PTR(lv)->length);
                                prev.ieee_uint.a = lv;
                                prev.ieee_uint.b = rol32(lv, 15);
                        }

                        IS_DOUBLE_AN_INTEGER32( av )
                        if (IS_ATOM_INT(av)) {
                                tf.ieee_uint.a = av;
                                tf.ieee_uint.b = rol32(av, 15);
                        }
                        else if (IS_ATOM_DBL(av)) {
                                tf.ieee_double = (double)DBL_PTR(av)->a_dbl;
                        }
                        else if (IS_SEQUENCE(av))
                        {
                                lTemp = calc_hash32(av,b);
                                if (IS_ATOM_INT(lTemp))
                                {
                                        tf.ieee_uint.a = lTemp;
                                        tf.ieee_uint.b = rol32(lTemp, 15);
                                }
                                else // if (IS_ATOM_DBL(lTemp))
                                {
                                        tf.ieee_double = (double)(DBL_PTR(lTemp)->a_dbl);
                                }
                                DeRef(lTemp);
                        }

                        tf.ieee_uint.a += prev.ieee_uint.b;
                        tf.ieee_uint.b += prev.ieee_uint.a;
                        for(tfi = 0; tfi < 8; tfi++)
                        {
                                if (tf.ieee_char[tfi] == 0)
                                        tf.ieee_char[tfi] = (unsigned char)(tfi * 171 + 1);
                                lHashValue = rol32(lHashValue, 3) ^ ((tf.ieee_char[tfi] + (tfi + 1)) << 8);
                        }
                        lHashValue = rol32(lHashValue,1);
                        lSLen--;
                }
        }

#ifdef CLEANUP_MOD
        DeRef(a)
        DeRef(b)
#endif // CLEANUP_MOD

        if (lHashValue  & HIGH_BITS) {
                return NewDouble32((double)lHashValue);
        }
        else {
                return MAKE_INT32(lHashValue);
        }

}

// Functions below, "get_rand()" and "set_rand()" are from "be_machine.c" (Euphoria v4.0.5)

// From: be_machine.c

long get_int32(object x)
/* return an integer value if possible, truncated to 32/64 bits. */
{
        if (IS_ATOM_INT(x)){
                return (long)INT_VAL32(x);
        }

        if (IS_ATOM(x)){
                if (DBL_PTR(x)->a_dbl <= 0.0){
                        return (long)(DBL_PTR(x)->a_dbl);
                }
                else{
                        return (long)(DBL_PTR(x)->a_dbl);
                }
        }
        RTFatal("an integer was expected, not a sequence");
        return 0;
}

object get_rand32()
/* Return the random generator's current seed values */
{
        s1_ptr result;

        result = NewS1(2);
        result->base[1] = (object)seed1;
        result->base[2] = (object)seed2;

        return MAKE_SEQ(result);
}

object set_rand32(object x)
/* set random number generator */
{
        long r;
        s1_ptr x_ptr;
        long slen;
        object_ptr obp;
#ifdef CLEANUP_MOD
        x = Convert32(x); // increase ref count
#endif
        if (!ASEQ(x)) {
                // Simple case - just a single value supplied.
                r = get_int32(x);

                seed1 = r+1;
                seed2 = ~(r) + 999;
        } else {
                // We got a sequence given to us.
                x_ptr = SEQ_PTR(x);
                slen = x_ptr->length;
                if (slen == 0) {
                        // Empty sequence means randomize the generator.
                        setran32();
                } else {
                        obp = x_ptr->base;
                        // A sequence of two atoms explictly supplies seed1 and seed2 values.
                        if ((slen == 2) && !ASEQ(obp[1]) && !ASEQ(obp[2])) {
                                seed1 = get_int32(obp[1]);
                                seed2 = get_int32(obp[2]);
                        }
                        else {
                                // Complex case - an arbitary sequence supplied.
                                seed1 = get_int32(calc_hash32(x, slen));
                                seed2 = get_int32(calc_hash32(slen, make_atom32(seed1)));
                        }
                }
        }
#ifdef CLEANUP_MOD
        DeRef(x) // decrease ref count
#endif
        rand_was_set = TRUE;

        return ATOM_1;
}


// End of Random function.
