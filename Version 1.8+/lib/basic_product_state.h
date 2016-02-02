#ifndef basic_product_state_h
#define basic_product_state_h

#include "basic_state.h"
#include "basic_set.h"

class basic_product_state
{

public:

    basic_state _1;               /* part 1 */
    basic_set<basic_state> _2;    /* part 2 */

public:

    /// default constructor
    basic_product_state()
    {}

    /// constructor by a state and a macro state (a set of states)
    basic_product_state(const basic_state & s, const basic_set<basic_state> & ms)
    :_1(s), _2(ms)
    {}

    /// reference copy constructor
    basic_product_state(const basic_product_state & ps)
    :_1(ps._1), _2(ps._2)
    {}

    /// destructor
    ~basic_product_state()
    {}

    /// assignment (replacement)
    basic_product_state & operator=(const basic_product_state & ps)
    {
        _1 = ps._1;
        _2 = ps._2;
        return *this;
    }

    /// equation test
    bool operator==(const basic_product_state & ps) const
    {
        return (_1 == ps._1) && (_2 == ps._2);
    }

    /// non-equation test
    bool operator!=(const basic_product_state & ps) const
    {
        return !(*this == ps);
    }

};

/// basic_product_state -> std::string
std::string to_string(const basic_product_state & ps)
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

/// basic_product_state -> std::ostream
/* print a product state by std::cout */
std::ostream & operator<<(std::ostream & out, const basic_product_state & ps)
{
    out << to_string(ps);
    return out;
}

#endif
