#ifndef component_h
#define component_h

#include "basic_state.h"
#include "basic_set.h"

class component
{

public:

    /* name of the component state */
    std::string _name;

    /* occurrence of the component state */
    int _occurrence;

public:

    /// default constructor
    component(std::string n = "unnamed", int o = 0)
    :_name(n), _occurrence(o)
    {}

    /// reference copy constructor
    component(const component & c)
    :_name(c._name), _occurrence(c._occurrence)
    {}

    /// destructor
    ~component()
    {}

    /// assignment (replacement)
    component & operator=(const component & c)
    {
        _name = c._name;
        _occurrence = c._occurrence;
    }

    /// equation test
    bool operator==(const component & c) const
    {
        return (_name == c._name) && (_occurrence == c._occurrence);
    }

    /// non-equation test
    bool operator !=(const component & c) const
    {
        return !(*this == c);
    }

};

/// component -> std::string
std::string to_string(const component & c)
{
    std::string result = "";
    result += c._name;
    result += "(";
    result += to_string(c._occurrence);
    result += ")";
    return result;
}

/// component -> std::ostream
/* print a product state by std::cout */
std::ostream & operator<<(std::ostream & out, const component & c)
{
    out << to_string(c);
    return out;
}

#endif
