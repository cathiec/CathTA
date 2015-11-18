#ifndef print_h
#define print_h

#include "tostring.h"

/// print a dimension
/* print a dimension by std::cout */
std::ostream & operator<<(std::ostream & out, const dim & d)
{
    out << to_string(d);
    return out;
}

/// print a container
/* print a container by std::cout */
template<typename T>
std::ostream & operator<<(std::ostream & out, const container<T> & c)
{
    out << to_string(c);
    return out;
}

/// print a state
/* print a state by std::cout */
std::ostream & operator<<(std::ostream & out, const state & s)
{
    out << to_string(s);
    return out;
}

/// print a symbol
/* print a symbol by std::cout */
std::ostream & operator<<(std::ostream & out, const symbol & s)
{
    out << to_string(s);
    return out;
}

/// print a product state
/* print a product state by std::cout */
std::ostream & operator<<(std::ostream & out, const product_state & ps)
{
    out << to_string(ps);
    return out;
}

/// print a transition
/* print a transition by std::cout */
std::ostream & operator<<(std::ostream & out, const transition & t)
{
    out << to_string(t);
    return out;
}

#endif
