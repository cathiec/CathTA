#ifndef timbuk_h
#define timbuk_h

#include "words_automaton.h"
#include "basic_tree_automaton.h"

int find_index(const words_automaton_state & s, const words_automaton & a)
{
    for(int i = 1; i <= a.Q.size(); i++)
    {
        if(s == a.Q[i])
            return i;
    }
    return -1;
}

basic_tree_automaton timbuk(const words_automaton & wa)
{
    basic_tree_automaton result;
    // name
    result._name = "timbuk format " + wa._name;
    // finite set of states
    for(int i = 1; i <= wa.Q.size(); i++)
        result.Q.add(basic_state("q" + to_string(i)));
    // finite set of final states
    for(int i = 1; i <= wa.F.size(); i++)
        result.F.add(basic_state("q" + to_string(find_index(wa.F[i], wa))));
    // finite set of transition rules and ranked alphabet
    symbol epsilon;
    epsilon._name = "EPSILON";
    epsilon._rank = 0;
    result.SIGMA.add(epsilon);
    for(int i = 1; i <= wa.I.size(); i++)
    {
        basic_transition temp;
        temp._alpha = epsilon;
        temp._output = basic_state("q" + to_string(find_index(wa.I[i], wa)));
        result.DELTA.add(temp);
    }
    for(int i = 1; i <= wa.DELTA.size(); i++)
    {
        basic_transition temp1;
        symbol temp2;
        temp2._name = wa.DELTA[i]._alpha;
        temp2._rank = 1;
        result.SIGMA.add(temp2);
        temp1._alpha = temp2;
        temp1._output = basic_state("q" + to_string(find_index(wa.DELTA[i]._right, wa)));
        basic_tuple<basic_state> temp3;
        temp3.add(basic_state("q" + to_string(find_index(wa.DELTA[i]._left, wa))));
        temp1._inputs = temp3;
        result.DELTA.add(temp1);
    }
    return result;
}

#endif
