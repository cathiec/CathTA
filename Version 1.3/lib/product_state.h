#ifndef product_state_h
#define product_state_h

#include "container.h"
#include "state.h"

class product_state
{

public:

    state _1;               /* part 1 */
    container<state> _2;    /* part 2 */

public:

    /// default constructor
    product_state()
    {}

    /// constructor by a state and a macro state (a set of states)
    product_state(const state & s, const container<state> & ms)
    :_1(s), _2(ms)
    {}

    /// constructor by reference copying
    /* create a new product state by copying another state "s" */
    product_state(const product_state & ps)
    :_1(ps._1), _2(ps._2)
    {}

    /// destructor
    ~product_state()
    {}

    /// assignment
    /* replace the current product state by a product state "ps" */
    product_state & operator=(const product_state & ps)
    {
        _1 = ps._1;
        _2 = ps._2;
        return *this;
    }

    /// equation
    /* check if all a product state "ps" is equal to the current product state */
    /* 2 product states are equal if and only if their 2 parts are equal */
    bool operator==(const product_state & ps) const
    {
        return (_1 == ps._1) && (_2 == ps._2);
    }

    /// non equation
    /* check if all a product state "ps" is not equal to the current product state */
    /* 2 product states are equal if and only if their 2 parts are equal */
    bool operator!=(const product_state & ps) const
    {
        return !(*this == ps);
    }

};

/// product_state -> std::string
std::string to_string(const product_state & ps)
{
    std::string result = "(";
    result += to_string(ps._1);
    result += ",{";
    if(ps._2.size() == 0)
    {
        result += "})";
        return result;
    }
    for(int i = 1; i <= ps._2.size(); i++)
    {
        result += to_string(ps._2[i]);
        result += ",";
    }
    result += "\b})";
    return result;
}

/// print a product state
/* print a product state by std::cout */
std::ostream & operator<<(std::ostream & out, const product_state & ps)
{
    out << to_string(ps);
    return out;
}

#endif
