#ifndef words_automaton_state_h
#define words_automaton_state_h

#include "combination.h"

class words_automaton_state
{

public:

    /* tuple containing the name of the states in the ancien tree automaton */
    /* ATTENTION: */
    /* It is a stack, but here we reverse the whole stack, we always play with the last element. */
    /* In another word, when we insert one element, we put it at the end, when we pop, we always pop the last element. */
    combination<std::string> _t;

public:

    /// default constructor
    words_automaton_state()
    {}

    /// reference copy constructor
    words_automaton_state(const words_automaton_state & s)
    :_t(s._t)
    {}

    /// destructor
    ~words_automaton_state()
    {}

    /// assignment (replacement)
    words_automaton_state & operator=(const words_automaton_state s)
    {
        _t = s._t;
    }

    /// equation test
    bool operator==(const words_automaton_state & s) const
    {
        return _t == s._t;
    }

    /// non-equation test
    bool operator !=(const words_automaton_state & s) const
    {
        return !(_t == s._t);
    }

    /// remove the first element
    void push_out()
    {
        _t._size_repre--;
    }

    /// add an element
    void push_in(std::string e)
    {
        _t.add(e);
    }

};

/// words_automaton_state -> std::string
std::string to_string(const words_automaton_state & s)
{
    std::string result = "<";
    for(int i = s._t.size(); i >= 1; i--)
    {
        result += s._t[i];
        result += ",";
    }
    if(s._t.size() != 0)
        result += "\b";
    result += ">";
    return result;
}

/// words_automaton_state -> std::ostream
std::ostream & operator<<(std::ostream & out, const words_automaton_state & s)
{
    out << to_string(s);
    return out;
}

#endif
