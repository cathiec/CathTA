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

#endif
