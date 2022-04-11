// Copyright (c) 2022 James Cook
//
// An Object is either an Atom or a Sequence.
// A Sequence is an array of Objects.
// An Atom is either a [long] double or an Integer.
// An Integer is a 31-bit (on 32-bit) or 63-bit (on 64-bit) signed number.

#pragma once

//#include "../modules/base_class.hpp"
#include "Integer.hpp"

#define IS_DOUBLE_TO_INT(temp_dbl) ((((eu::eudouble)((eu::elong)temp_dbl)) == temp_dbl) && (temp_dbl <= MAXINT_DBL) && (temp_dbl >= MININT_DBL))

namespace eu
{
    template <class AtomType = eudouble>
    class Atom : public Integer // Atom inherits Integer data and functions.
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
        Atom() { obj = NOVALUE; SET_DEBUG } // default constructor
        ~Atom() { SET_DEBUG DeRef(obj) obj = NOVALUE; } // default destructor
        Atom(const Atom& x) { obj = x.obj; Ref(obj) SET_DEBUG } // copy constructor
        Atom& operator= (const Atom& x) { DeRef(obj) obj = x.obj; Ref(obj) SET_DEBUG return *this; } // copy assignment
    // On newer compilers:
        Atom(Atom&& x) { obj = x.obj; x.obj = NOVALUE; SET_DEBUG } // move constructor
        Atom& operator= (Atom&& x) { DeRef(obj) obj = x.obj; x.obj = NOVALUE; SET_DEBUG return *this; } // move assignment
    // End On newer compilers.
        Atom(d_ptr a) { obj = MAKE_DBL(a); SET_DEBUG }
        Atom(object a) { obj = a; SET_DEBUG }
        Atom(eudouble d) { obj = IS_DOUBLE_TO_INT(d) ? (object)d : NewDouble(d); SET_DEBUG }
        void NewAtom(eudouble d) { DeRef(obj) obj = IS_DOUBLE_TO_INT(d) ? (object)d : NewDouble(d); SET_DEBUG }
        void NewAtom(integer i) { DeRef(obj) obj = TYPE_CHECK_INTEGER(i) ? i : NewDouble((eudouble)i); SET_DEBUG }
        void NewAtom(eulong u) { DeRef(obj) obj = TYPE_CHECK_INTEGER(u) ? u : NewDouble((eudouble)u); SET_DEBUG }

        eudouble GetAtomDbl() { SET_DEBUG if (IS_ATOM_INT(obj)) { return (eudouble)obj; } else if (IS_ATOM_DBL(obj)) { return DBL_PTR(obj)->dbl; } else { RTFatal("Expected an Atom, but found a Sequence, in 'GetAtomDbl()'"); return (eudouble)0; } }
        elong GetAtomInt() { SET_DEBUG if (IS_ATOM_INT(obj)) { return obj; } else if (IS_ATOM_DBL(obj)) { return (elong)(DBL_PTR(obj)->dbl); } RTFatal("Expected an Atom, but found a Sequence, in 'GetAtomInt()'"); return 0; }
        eulong GetAtomUnsignedInt() { SET_DEBUG if (IS_ATOM_INT(obj)) { return (eulong)obj; } else if (IS_ATOM_DBL(obj)) { return (eulong)(DBL_PTR(obj)->dbl); } RTFatal("Expected an Atom, but found a Sequence, in 'GetAtomUnsignedInt()'"); return 0; }
        elong TryDoubleToInt() { if (IS_DBL_OR_SEQUENCE(obj) && IS_ATOM_DBL(obj)) { object ob = DoubleToInt(obj); if (ob != obj) { DeRefDS(obj) obj = ob; return E_INTEGER; } return E_ATOM; } return IS_ATOM_INT(obj); }

        void print(int ascii = 0, const char int_format[] = "%" ELONG_WIDTH "d", int char_min = 32, int char_max = 127,
#ifdef BITS64
            const char dbl_format[] = "%." EUDOUBLE_WIDTH "Lg"
#else
            const char dbl_format[] = "%." EUDOUBLE_WIDTH "g"
#endif
        )
        {
            object ob = obj;
            if (!IS_ATOM(ob)) {
                RTFatal("Expected an atom in Atom::print().");
            }
            if (!IS_ATOM_INT(ob)) {
                ob = DoubleToInt(ob);
                if (!IS_ATOM_INT(ob)) {
                    // must be a eudouble
                    printf(dbl_format, DBL_PTR(ob)->dbl);
                    return;
                }
            }
            {
                Integer i(ob);
                i.print(ascii, int_format, char_min, char_max);
            }
        }
    };


}
