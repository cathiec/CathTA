#ifndef state_h
#define state_h

#include "container.h"
#include "state.h"

class state_records
{

public:

    std::string _name;                          /* name */
    int _dimension;                             /* dimension */
    container<std::string> _states_needed;      /* states needed to have the current state */
    container<container<int>> _num_needed;      /* numbers of each state needed */

public:

    /// default constructor
    state(std::string n = "unnamed")
    :_name(n), _dimension(0)
    {}

    /// constructor by reference copying
    /* create a new state with records by copying another state with records "s" */
    state(const state_records & s)
    :_name(s._name), _dimension(s._dimension), _states_needed(s._states_needed), _num_needed(s._num_needed)
    {}

    /// destructor
    ~state()
    {}

    /// assignment
    /* replace the current state with records by a state with records "s" */
    state & operator=(const state_records & s)
    {
        _name = s._name;
		_dimension = s._dimension;
        _states_needed = s._states_needed;
        _num_needed = s._num_needed;
        return *this;
    }

    /// equation
    /* check if a state with records "s" is equal to the current state with records */
    bool operator==(const state_records & s) const
    {
        return (_name == s._name && _dimension == s._dimension && _states_needed == s._states_needed && _num_needed == s._num_needed);
    }

    /// non equation
    /* check if a state with records "s" is not equal to the current state with records */
    bool operator!=(const state & s) const
    {
        return !(*this == s);
    }

    /// merge 2 states with same name and same dimension
    int merge(const state_records & s)
    {

    }

};

#endif
