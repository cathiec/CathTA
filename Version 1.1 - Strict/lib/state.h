#ifndef state_h
#define state_h

#include <iostream>

class state
{

public:

    std::string _name;  /* name */

public:

    /// default constructor
    state(std::string n = "unnamed")
    :_name(n)
    {}

    /// constructor by reference copying
    /* create a new state by copying another state "s" */
    state(const state & s)
    :_name(s._name)
    {}

    /// destructor
    ~state()
    {}

    /// assignment
    /* replace the current state by a state "s" */
    state & operator=(const state & s)
    {
        _name = s._name;
        return *this;
    }

    /// equation
    /* check if a state "s" is equal to the current state */
    /* 2 states are equal if and only if their names are same */
    bool operator==(const state & s) const
    {
        return _name == s._name;
    }

    /// non equation
    /* check if a state "s" is not equal to the current state */
    /* 2 states are equal if and only if their names are same */
    bool operator!=(const state & s) const
    {
        return !(*this == s);
    }

    /// cross product
    /* compute the cross product of 2 states */
    /* the cross product of state "a" and state "b" is "(a,b)" */
    state operator*(const state & s) const
    {
        state result("(");
        result._name += _name;
        result._name += ",";
        result._name += s._name;
        result._name += ")";
        return result;
    }

};

/// state -> std::string
std::string to_string(const state & s)
{
    return s._name;
}

/// print a state
/* print a state by std::cout */
std::ostream & operator<<(std::ostream & out, const state & s)
{
    out << to_string(s);
    return out;
}

#endif
