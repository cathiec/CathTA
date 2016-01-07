#ifndef words_automaton_transition_h
#define words_automaton_transition_h

#include "words_automaton_state.h"

class words_automaton_transition
{

public:

    /* symbol */
    std::string _alpha;

    /* left */
    words_automaton_state _left;

    /* right */
    words_automaton_state _right;

public:

    /// default constructor
    words_automaton_transition()
    {}

    /// reference copy constructor
    words_automaton_transition(const words_automaton_transition & t)
    :_alpha(t._alpha), _left(t._left), _right(t._right)
    {}

    /// destructor
    ~words_automaton_transition()
    {}

    /// assignment (replacement)
    words_automaton_transition & operator=(const words_automaton_transition & t)
    {
        _alpha = t._alpha;
        _left = t._left;
        _right = t._right;
        return *this;
    }

    /// equation test
    bool operator==(const words_automaton_transition & t) const
    {
        return (_alpha == t._alpha) && (_left == t._left) && (_right == t._right);
    }

    /// non-equation test
    bool operator!=(const words_automaton_transition & t) const
    {
        return !(*this == t);
    }

};

/// words_automaton_transition -> std::string
std::string to_string(const words_automaton_transition & t)
{
    std::string result = to_string(t._left);
    result += " --- ";
    result += to_string(t._alpha);
    result += " ---> ";
    result += to_string(t._right);
    return result;
}

/// words_automaton_transition -> std::ostream
std::ostream & operator<<(std::ostream & out, const words_automaton_transition & t)
{
    out << to_string(t);
    return out;
}

#endif
