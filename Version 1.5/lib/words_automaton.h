#ifndef words_automaton_h
#define words_automaton_h

#include "words_automaton_transition.h"

class words_automaton
{

public:

    /* name */
    std::string _name;

    /* ranked alphabet */
    basic_set<std::string> SIGMA;

    /* finite set of states */
    basic_set<words_automaton_state> Q;

    /* finite set of initial states */
    basic_set<words_automaton_state> I;

    /* finite set of final states */
    basic_set<words_automaton_state> F;

    /* finite set of transition rules */
    basic_set<words_automaton_transition> DELTA;

public:

    /// default constructor
    words_automaton()
    {}

    /// reference copy constructor
    words_automaton(const words_automaton & a)
    :_name(a._name), SIGMA(a.SIGMA), Q(a.Q), I(a.I), F(a.F), DELTA(a.DELTA)
    {}

    /// destructor
    ~words_automaton()
    {}

    /// assignment (replacement)
    words_automaton & operator=(const words_automaton & a)
    {
        _name = a._name;
        SIGMA = a.SIGMA;
        Q = a.Q;
        I = a.I;
        F = a.F;
        DELTA = a.DELTA;
        return *this;
    }

};

/// words_automaton -> std::string
std::string to_string(const words_automaton & a)
{
    std::string result = "";
    int i;
    result += "Automaton [";
    result += a._name;
    result += "]\n";
    result += "--- ALPHABET ---\n";
    for(i = 1; i <= a.SIGMA.size(); i++)
    {
        result += "\t";
        result += to_string(a.SIGMA[i]);
        result += "\n";
    }
    result += "--- STATES ---\n";
    for(i = 1; i <= a.Q.size(); i++)
    {
        result += "\t";
        result += to_string(a.Q[i]);
        result += "\n";
    }
    result += "--- INITIAL STATES ---\n";
    for(i = 1; i <= a.I.size(); i++)
    {
        result += "\t";
        result += to_string(a.I[i]);
        result += "\n";
    }
    result += "--- FINAL STATES ---\n";
    for(i = 1; i <= a.F.size(); i++)
    {
        result += "\t";
        result += to_string(a.F[i]);
        result += "\n";
    }
    result += "--- TRANSITIONS ---\n";
    for(i = 1; i <= a.DELTA.size(); i++)
    {
        result += "\t";
        result += to_string(a.DELTA[i]);
        result += "\n";
    }
    return result;
}

/// words_automaton -> std::ostream
std::ostream & operator<<(std::ostream & out, const words_automaton & a)
{
    out << to_string(a);
    return out;
}

#endif
