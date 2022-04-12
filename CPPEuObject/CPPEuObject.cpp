// Copyright (c) 2022 James Cook
//
// CPPEuObject.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdio.h>

#include "eu.hpp"

using namespace std;
using namespace eu;

#ifdef USE_QUADMATH_H
#ifndef __GNUC__
struct __complex128 {
    __float128 x, y; /* real and imaginary parts */
};
#endif
#endif

#define VIEW_COUNT(name, count) printf("%s -> count == %d\n", name, (int)count)

int main()
{
    cout << "INTPTR_MAX==" << INTPTR_MAX << endl;
    cout << "INT64_MAX==" << INT64_MAX << endl;
#ifdef USE_QUADMATH_H
    cout << "Sizeof(__complex128) is " << sizeof(__complex128) << endl;
    cout << "Sizeof(equadmath) is " << sizeof(equadmath) << endl;
#endif
    cout << "Sizeof(eudouble) is " << sizeof(eudouble) << endl;
    cout << "Sizeof(elong) is " << sizeof(elong) << endl;
    cout << "Sizeof(object) is " << sizeof(object) << endl;
    cout << "Sizeof(void*) is " << sizeof(void*) << endl;

    cout << "Sizeof(struct d) is " << sizeof(struct d) << endl;
    cout << "Sizeof(struct s1) is " << sizeof(struct s1) << endl;
    cout << "Sizeof(class Object) is " << sizeof(class Object<>) << endl;
    cout << "Sizeof(class Sequence) is " << sizeof(class Sequence<>) << endl;
    cout << "Sizeof(class Atom) is " << sizeof(class Atom<>) << endl;
    cout << "Sizeof(class Integer) is " << sizeof(class Integer) << endl;

    cout << "DBL_DIG is " << DBL_DIG << endl;
    cout << "LDBL_DIG is " << LDBL_DIG << endl;


    if (true) {
        Object<>* ap;
        Object<>* bp;
        Object<>* cp;

        if (true) {
            Object st(NewString("abc"));
            Object sum((elong)'a' - (elong)'A');
            Object result;

            VIEW_COUNT("st", st.eobject()->ViewCount());
            VIEW_COUNT("sum", sum.eobject()->ViewCount());
            VIEW_COUNT("result", result.eobject()->ViewCount());

            *result.eobject() = *st.eobject() - *sum.eobject();

            VIEW_COUNT("st", st.eobject()->ViewCount());
            VIEW_COUNT("sum", sum.eobject()->ViewCount());
            VIEW_COUNT("result", result.eobject()->ViewCount());

            //here

            st.print(2);
            printf("\n");
            printf("Results should be in uppercase:\n");
            result.print(2);
            printf("\n");

            ap = &st;
            bp = &sum;
            cp = &result;

            //st.~Object();
            //sum.~Object();
            //result.~Object();
        }
        VIEW_COUNT("st", ap->eobject()->ViewCount());
        VIEW_COUNT("sum", bp->eobject()->ViewCount());
        VIEW_COUNT("result", cp->eobject()->ViewCount());
    }
    //here


#ifdef USE_QUADMATH_H
    equadmath a;

    a = 1.23Q;

    cout << (double)a << endl;
#endif

    //std::cout << "Hello World!\n";

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
