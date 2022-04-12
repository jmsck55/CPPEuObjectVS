// Copyright (c) 2022 James Cook
//
// An Object is either an Atom or a Sequence.
// A Sequence is an array of Objects.
// An Atom is either a [long] double or an Integer.
// An Integer is a 31-bit (on 32-bit) or 63-bit (on 64-bit) signed number.

#pragma once

// To use eu::seq(), define USE_STDARG_H
#define USE_STDARG_H

#include "base_class.hpp"

namespace eu
{
    bool is_seq_string(object ob, elong minChar = 32, elong maxChar = 127)
    {
        object_ptr elem;
        elong len;
        ob = (object)SEQ_PTR(ob);
        elem = ((s1_ptr)ob)->base;
        len = ((s1_ptr)ob)->length;
        while (len-- > 0)
        {
            ob = *(++elem);
            if (IS_DBL_OR_SEQUENCE(ob) && IS_ATOM_DBL(ob))
            {
                ob = DoubleToInt(ob);
            }
            if (!IS_ATOM_INT(ob) || (ob > maxChar) || (ob < minChar))
            {
                return FALSE;
            }
        }
        return TRUE;
    }

    template <class ElementType = object>
    class Sequence : public base_class // Sequence inherits base_class data and functions.
    {
    private:
        //object obj;
    protected:
        //object obj;
    public:
#ifndef DONE_DEBUGGING
        inline static d_ptr lookatd;
        inline static s1_ptr lookats;
#endif
        Sequence() { obj = NOVALUE; SET_DEBUG } // default constructor
        ~Sequence() { SET_DEBUG DeRef(obj) obj = NOVALUE; } // default destructor
        Sequence(const Sequence& x) { obj = x.obj; SET_DEBUG Ref(obj) } // copy constructor
        Sequence& operator= (const Sequence& x) { SET_DEBUG DeRef(obj) obj = x.obj; SET_DEBUG Ref(obj) return *this; } // copy assignment
#ifndef __WATCOMC__
        Sequence(Sequence&& x) { SET_DEBUG obj = x.obj; SET_DEBUG x.obj = NOVALUE; } // move constructor
        Sequence& operator= (Sequence&& x) { SET_DEBUG DeRef(obj) obj = x.obj; SET_DEBUG x.obj = NOVALUE; return *this; } // move assignment
#endif
        //Sequence(const object& x) { obj = x; RefObj(); }
        //Sequence(object& x) { obj = x; RefObj(); }
        //Sequence(const object x) { obj = x; RefObj(); }
        Sequence(object x) { SET_DEBUG obj = x; SET_DEBUG } // Increase the reference count before calling this function.
        Sequence& operator= (const object& x)
        {
            SET_DEBUG
                DeRef(obj)
                obj = x;
            SET_DEBUG
                Ref(obj) // Possibly not a memory leak.
                return *this;
        }

        bool is_initialized() { SET_DEBUG return obj != NOVALUE; }
        object swap(object x) { object ret = obj; obj = x; SET_DEBUG return ret; }

        Sequence(s1_ptr a) { obj = MAKE_SEQ(a); SET_DEBUG }

        Sequence(const char* str) { obj = ENewString(str); SET_DEBUG }
        void S_NewString(const char* str) { DeRef(obj); obj = NewString(str); SET_DEBUG } // ENewString(), named that way to avoid naming conflicts.
        char* GetCharStr() {
            SET_DEBUG
            if (IS_DBL_OR_SEQUENCE(obj) && IS_SEQUENCE(obj) && is_seq_string(obj, 1, 255)) {
                elong len = SEQ_PTR(obj)->length;
                char* str = (char*)malloc(len + 1);
                MakeCString(str, obj);
                return str;
            }
            RTFatal("Expected a string Sequence, but found an Object, in 'GetCharStr()'");
            return NULL;
        }
        void ScreenOutput(FILE* f) { char* out_string = GetCharStr(); screen_output(f, out_string); }
        elong length() { if (IS_DBL_OR_SEQUENCE(obj) && IS_SEQUENCE(obj)) { return SEQ_PTR(obj)->length; } else { RTFatal("Expected a Sequence, but found an Atom, in 'length()'"); return -1; } }

        void print(int ascii = 0, const char int_format[] = "%" ELONG_WIDTH "d", int char_min = 32, int char_max = 127,
#ifdef BITS64
            const char dbl_format[] = "%." EUDOUBLE_WIDTH "Lg"
#else
            const char dbl_format[] = "%." EUDOUBLE_WIDTH "g"
#endif
        )
        {
            if (!(IS_DBL_OR_SEQUENCE(obj) && IS_SEQUENCE(obj))) {
                RTFatal("Expected a sequence in Sequence::print().");
            }
            else {
                size_t len = (size_t)SEQ_PTR(obj)->length;
                if ((ascii == 2) && is_seq_string(obj, char_min, char_max))
                { // must be a sequence
                    char* str = (char*)malloc(len + 1);
                    MakeCString(str, obj);
                    printf("\"%s\"", str);
                    free(str);
                }
                else {
                    object_ptr pob = SEQ_PTR(obj)->base;
                    Object ob;
                    printf("{");
                    while (len-- > 0) {
                        ob.swap(*(++pob));
                        ob.print(ascii, int_format, char_min, char_max, dbl_format);
                        if (len > 0)
                            printf(", ");
                    }
                    ob.swap(NOVALUE);
                }
            }
        }

        void E_slice(Sequence src, object start, object end) { // make "obj = src[start..end]", assign src[start..end] to this "obj"
            if (IS_DBL_OR_SEQUENCE(src.obj) && IS_SEQUENCE(src.obj)) {
                rhs_slice_target = &obj; // DeRef()'s "obj" in next statement below.
                RHS_Slice((s1_ptr)src.obj, start, end); // takes all object's.
                rhs_slice_target = NULL;
            }
            else {
                RTFatal("Expected argument to be a Sequence in 'E_slice()'");
            }
        }
        object E_slice_func(object start, object end) { // make "ret = obj[start..end]"
            if (IS_DBL_OR_SEQUENCE(obj) && IS_SEQUENCE(obj)) {
                object ret = NOVALUE; // needs to be NOVALUE
                rhs_slice_target = &ret;
                RHS_Slice((s1_ptr)obj, start, end);
                rhs_slice_target = NULL;
                return ret;
            }
            else {
                RTFatal("Expected this class to be a Sequence in 'E_slice_func()'");
                return NOVALUE;
            }
        }
        void E_assign_to_slice(object start, object end, object val) { // make "slice[start..end] = val", val should be the same length as "end - start + 1"
            if (IS_DBL_OR_SEQUENCE(obj) && IS_SEQUENCE(obj)) {
                assign_slice_seq = (s1_ptr*)&obj;
                AssignSlice(start, end, (s1_ptr)val);
                assign_slice_seq = NULL;
            }
            else {
                RTFatal("Expected target and argument Sequences in 'E_assign_to_slice()'");
            }
        }

        object E_at(elong i) // use (1 to length) or (-1 to -length) // make "obj = seq[index]"
        {
            object ret;
            if (IS_DBL_OR_SEQUENCE(obj) && IS_SEQUENCE(obj) && TYPE_CHECK_INTEGER(i)) {
                s1_ptr sp = SEQ_PTR(obj);
                if (i < 0) {
                    ++i += sp->length;
                }
                if ((i >= 1) && (i <= sp->length)) {
                    ret = sp->base[i];
                    Ref(ret); // Not a possible memory leak.
                }
                else {
                    RTFatal("Expected a valid index number in 'E_at()'");
                }
            }
            else {
                RTFatal("Expected a Sequence and a valid index number in 'E_at()'");
            }
            return ret;
        }
        void E_assign_to_at(elong i, object val) // make "seq[index] = obj"
        {
            if (IS_DBL_OR_SEQUENCE(obj) && IS_SEQUENCE(obj) && TYPE_CHECK_INTEGER(i)) {
                s1_ptr sp = SEQ_PTR(obj);
                if (i < 0) {
                    ++i += sp->length;
                }
                if ((i >= 1) && (i <= sp->length)) {
                    if (!UNIQUE(sp)) {
                        sp = (s1_ptr)SequenceCopy(sp); // deref's 'sp'
                        obj = MAKE_SEQ(sp); // store new value to obj
                    }
                    // Ref(val); // possible memory leak, use "s.GetValue()" on classes to avoid memory leak.
                    DeRef(sp->base[i])
                        sp->base[i] = val;
                }
                else {
                    RTFatal("Expected a valid index number in 'E_assign_to_at()'");
                }
            }
            else {
                RTFatal("Expected a Sequence and a valid index number in 'E_assign_to_at()'");
            }
        }

        void repeat(object item, object repcount)
        {
            DeRef(obj) // Repeat does not deref target.
            obj = Repeat(item, repcount);
        }
        void prepend(object a)
        {
            // deref's target already
            // Ref(a) // possible memory leak.
            if (IS_DBL_OR_SEQUENCE(obj) && IS_SEQUENCE(obj)) {
                Prepend(&obj, obj, a);
            }
            else {
                RTFatal("Expected a target Sequence in 'prepend()'");
            }
        }
        void append(object a)
        {
            // deref's target already
            // Ref(a) // possible memory leak.
            if (IS_DBL_OR_SEQUENCE(obj) && IS_SEQUENCE(obj)) {
                Append(&obj, obj, a);
            }
            else {
                RTFatal("Expected a target Sequence in 'append()'");
            }
        }
        object concatN() // join()
        {
            object ret = NOVALUE;
            if (IS_DBL_OR_SEQUENCE(obj) && IS_SEQUENCE(obj)) {
                s1_ptr sp = SEQ_PTR(obj);
                Concat_N(&ret, sp->base + 1, sp->length);
            }
            else {
                RTFatal("Expected this class to be a Sequence in 'concatN()'");
            }
            return ret;
        }

        //friend object S_repeat(object item, object repcount);

        friend void S_prepend(object_ptr target_ptr, Sequence& src, object a);
        friend void S_append(object_ptr target_ptr, Sequence& src, object a);
        friend void S_concat(object_ptr target_ptr, object a, object b);
        friend void S_concatN(object_ptr target_ptr, Sequence& sources);

        friend object S_prepend_func(Sequence& src, object a); // target = prepend(src, a);
        friend object S_append_func(Sequence& src, object a); // target = append(src, a);
        friend object S_concat_func(object a, object b); // target = a & b; a and b can be atoms or sequences
        friend object S_concatN_func(Sequence& sources); // concatenate all elements of source, store in target.

        friend elong E_find(object a, Sequence b); // pos = find(a, b);
        friend elong E_find_from(object a, Sequence b, object c); // pos = find_from(a, b, start_from);
        friend elong E_match(Sequence a, Sequence b); // pos = match(a, b);
        friend elong E_match_from(Sequence a, Sequence b, object c); // pos = match_from(a, b, start_from);
    };

    elong E_compare(object a, object b) { return compare(a, b); }

    elong E_find(object a, Sequence<> b) { return find(a, (s1_ptr)b.obj); }
    elong E_match(Sequence<> a, Sequence<> b) { return e_match((s1_ptr)a.obj, (s1_ptr)b.obj); }

    elong E_find_from(object a, Sequence<> b, object c) { return find_from(a, (s1_ptr)b.obj, c); }
    elong E_match_from(Sequence<> a, Sequence<> b, object c) { return e_match_from((s1_ptr)a.obj, (s1_ptr)b.obj, c); }

    object S_repeat(object item, object repcount)
    {
        return Repeat(item, repcount);
    }
    void S_prepend(object_ptr target_ptr, Sequence<>& src, object a)
    {
        // Ref(a) // possible memory leak.
        Prepend(target_ptr, src.obj, a);
    }
    object S_prepend_func(Sequence<>& src, object a) // target = prepend(src, a);
    {
        object ret = NOVALUE;
        S_prepend(&ret, src, a);
        return ret;
    }
    void S_append(object_ptr target_ptr, Sequence<>& src, object a)
    {
        // Ref(a) // possible memory leak.
        Append(target_ptr, src.obj, a);
    }
    object S_append_func(Sequence<>& src, object a) // target = append(src, a);
    {
        object ret = NOVALUE;
        S_append(&ret, src, a);
        return ret;
    }
    void S_concat(object_ptr target_ptr, object a, object b)
    {
        bool is_seq_a = IS_DBL_OR_SEQUENCE(a) && IS_SEQUENCE(a);
        bool is_seq_b = IS_DBL_OR_SEQUENCE(b) && IS_SEQUENCE(b);
        if (is_seq_a && (!is_seq_b))
        {
            // Ref(b) // possible memory leak.
            Append(target_ptr, a, b);
        }
        else if ((!is_seq_a) && is_seq_b)
        {
            // Ref(a) // possible memory leak.
            Prepend(target_ptr, b, a);
        }
        else
        {
            Concat(target_ptr, a, (s1_ptr)b); // deref's target
            //RefDS(target.obj); // possible memory leak.
        }
    }
    object S_concat_func(object a, object b) // target = a & b; a and b can be atoms or sequences
    {
        object ret = NOVALUE;
        S_concat(&ret, a, b);
        return ret;
    }
    void S_concatN(object_ptr target_ptr, Sequence<>& sources)
    {
        if (IS_DBL_OR_SEQUENCE(sources.obj) && IS_SEQUENCE(sources.obj)) {
            s1_ptr source_ptr = SEQ_PTR(sources.obj);
            Concat_N(target_ptr, source_ptr->base + 1, source_ptr->length);
        }
        else {
            RTFatal("Expected this class to be a Sequence in 'concatN()'");
        }
    }
    object S_concatN_func(Sequence<>& sources) // concatenate all elements of source, store in target.
    {
        object ret = NOVALUE;
        S_concatN(&ret, sources);
        return ret;
    }

#ifdef USE_STDARG_H
    object seq(elong n, ...) {
        // Make a sequence with 'n' elements, each element must be an object.
        va_list vl; // vl is "vee" (v) followed by an "el" (l)
        va_start(vl, n);
        // If C++11 then:
        if (n == NOVALUE)
        {
            object val;
            va_list vl_count;
            n = -1;
            /* count number of arguments: */
            va_copy(vl_count, vl);
            do {
                n++;
                val = va_arg(vl_count, object);
            } while (val != NOVALUE);
            va_end(vl_count);
        }
        // EndIf C++11.
        if (IS_ATOM_INT(n))
        {
            object ob;
            object_ptr obj_ptr;
            s1_ptr ptr;
            ptr = NewS1(n); // ref==1
            obj_ptr = ptr->base;
#ifdef DONE_DEBUGGING
            while (n-- > 0)
#else
            for (elong i = 1; i <= n; i++) // Euphoric 'i' starts at one (1).
#endif
            {
                ob = va_arg(vl, object);
                if (!(IS_ATOM_INT(ob) || IS_DBL_OR_SEQUENCE(ob)))
                {
                    RTFatal("Expected a number, then that number of objects, as parameters to 'seq()'");
                    return NOVALUE;
                }
                // (*(base_class*)&ob).RefObj(); // memory leak.
                *(++obj_ptr) = ob;
            }
            va_end(vl);
            ob = MAKE_SEQ(ptr);
            // (*(base_class*)&ob).RefObj(); // memory leak.
            return ob; // return value.
        }
        RTFatal("Expected a number or NOVALUE as the first parameter of 'seq()'");
        return NOVALUE;
    }
#endif // USE_STDARG_H



}
