#ifndef state_with_dimension_h
#define state_with_dimension_h

#include "component.h"

class state_with_dimension
{

public:

    /* name of the state */
    std::string _name;

    /* dimension of the state */
    int _dimension;

    /* history of the construction */
    basic_set<component> _history;

public:

    /// default constructor
    state_with_dimension(std::string n = "unnamed", int d = 0)
    :_name(n), _dimension(d)
    {}

    /// reference copy constructor
    state_with_dimension(const state_with_dimension & s)
    :_name(s._name), _dimension(s._dimension), _history(s._history)
    {}

    /// destructor
    ~state_with_dimension()
    {}

    /// assignment (replacement)
    state_with_dimension & operator=(const state_with_dimension s)
    {
        _name = s._name;
        _dimension = s._dimension;
        _history = s._history;
    }

    /// equation test
    bool operator==(const state_with_dimension & s) const
    {
        return (_name == s._name) && (_dimension == s._dimension) && (_history == s._history);
    }

    /// non-equation test
    bool operator !=(const state_with_dimension & s) const
    {
        return !(*this == s);
    }

    /// smaller or equal
    bool operator<=(const state_with_dimension & s) const
    {
        if(_name != s._name)
            return false;
        if(_dimension <= s._dimension)
            return true;
        for(int i = 1; i <= _history.size(); i++)
        {
            bool smaller_or_equal = false;
            for(int j = 1; j <= s._history.size(); j++)
            {
                if(_history[i]._name == s._history[j]._name && _history[i]._occurrence <= s._history[j]._occurrence)
                {
                    smaller_or_equal = true;
                    break;
                }
            }
            if(!smaller_or_equal)
                return false;
        }
        return true;
    }

    /// add component
    void add_component(const component & c)
    {
        bool exist = false;
        for(int i = 1; i <= _history.size(); i++)
            if(c._name == _history[i]._name)
            {
                exist = true;
                _history[i]._occurrence += c._occurrence;
                break;
            }
        if(!exist)
            _history.add(c, true);
    }

    /// add history
    void add_history(const state_with_dimension & s)
    {
        for(int i = 1; i <= s._history.size(); i++)
            add_component(s._history[i]);
        add_component(component(s._name, 1));
    }

    /// triangle check
    bool triangle_check() const
    {
        for(int i = 1; i <= _history.size(); i++)
            if(_name == _history[i]._name && _history[i]._occurrence >= 2)
                return true;
        return false;
    }

};

/// basic_state -> std::string
std::string to_string(const state_with_dimension & s)
{
    return s._name + "(" + to_string(s._dimension) + ")" + " " + to_string(s._history);
}

/// basic_state -> std::ostream
std::ostream & operator<<(std::ostream & out, const state_with_dimension & s)
{
    out << to_string(s);
    return out;
}

#endif
