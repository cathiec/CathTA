#ifndef occurrence_h
#define occurrence_h

#include "basic_tuple.h"

/// occurrences of elements in a basic_tuple
template<typename T>
basic_set<int> occurrences(const basic_tuple<T> & t, const T & s)
{
    basic_set<int> result;
    for(int i = 0; i < t.size(); i++)
        if(t._data[i] == s)
            result.add(i);
    return result;
}

/// check if 2 elements' occurrences have any same index
bool occurrences_have_same_index(const basic_set<int> & o1, const basic_set<int> & o2)
{
    for(int i = 1; i <= o1.size(); i++)
        if(o2.contain(o1[i]))
            return true;
    return false;
}

#endif
