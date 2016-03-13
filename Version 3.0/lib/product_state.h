#ifndef _product_state_h
#define _product_state_h

#include "state.h"
#include "container.h"

class product_state
{

public:

    state _1;               /* part 1 */
    std::set<state> _2;     /* part 2 */

public:

    // default constructor
    product_state()
    {}

    // constructor by a state and a macro state (a set of states)
    product_state(const state & s, const std::set<state> & ms)
    :_1(s), _2(ms)
    {}

    // reference copy constructor
    product_state(const product_state & ps)
    :_1(ps._1), _2(ps._2)
    {}

    // destructor
    ~product_state()
    {}

    // assignment
    product_state & operator=(const product_state & ps)
    {
        _1 = ps._1;
        _2 = ps._2;
        return *this;
    }

    // comparison
    bool operator==(const product_state & ps) const
    {
        return (_1 == ps._1) && (_2 == ps._2);
    }

    bool operator!=(const product_state & ps) const
    {
        return !(*this == ps);
    }

    bool operator<(const product_state & ps) const
    {
        return _1 < ps._1;
    }

    // to string
    std::string to_string() const
    {
        std::string result = "(";
        result += _1.to_string();
        result += ",{";
        for(std::set<state>::iterator it = _2.begin(); it != _2.end(); it++)
        {
            result += (*it).to_string();
            result += ',';
        }
        if(_2.size() > 0)
            result += '\b';
        result += "})";
        return result;
    }

};

std::ostream & operator<<(std::ostream & out, const product_state & ps)
{
    out << ps.to_string();
    return out;
}

#endif
