#ifndef state_h
#define state_h

#include <iostream>
#include "tostring.h"

bool cath_show_state_dimension = false;

class state
{

public:

    std::string _name;  /* name */
	int _dimension;     /* dimension (used for dimension-bounded inclusion checking) */

public:

    /// default constructor
    state(std::string n = "unnamed")
    :_name(n), _dimension(0)
    {}

    /// constructor by reference copying
    /* create a new state by copying another state "s" */
    state(const state & s)
    :_name(s._name), _dimension(s._dimension)
    {}

    /// destructor
    ~state()
    {}

    /// assignment
    /* replace the current state by a state "s" */
    state & operator=(const state & s)
    {
        _name = s._name;
		_dimension = s._dimension;
        return *this;
    }

    /// equation
    /* check if a state "s" is equal to the current state */
    bool operator==(const state & s) const
    {
        return (_name == s._name && _dimension == s._dimension);
    }

    /// non equation
    /* check if a state "s" is not equal to the current state */
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
	std::string result = s._name;
	if(cath_show_state_dimension)
	{
		result += "#";
        result += to_string(s._dimension);
	}
    return result;
}

/// print a state
/* print a state by std::cout */
std::ostream & operator<<(std::ostream & out, const state & s)
{
    out << to_string(s);
    return out;
}

#endif
