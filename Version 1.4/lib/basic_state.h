#ifndef state_h
#define state_h

#include <iostream>

class basic_state
{

public:

    /* name of the state */
    std::string _name;

public:

    /// default constructor
    basic_state(std::string n = "unnamed")
    :_name(n)
    {}

    /// reference copy constructor
    basic_state(const basic_state & s)
    :_name(s._name)
    {}

    /// destructor
    ~basic_state()
    {}

    /// assignment (replacement)
    basic_state & operator=(const basic_state s)
    {
        _name = s._name;
    }

    /// equation test
    bool operator==(const basic_state & s) const
    {
        return _name == s._name;
    }

    /// non-equation test
    bool operator !=(const basic_state & s) const
    {
        return !(_name == s._name);
    }

    /// product
    basic_state operator*(const basic_state & s) const
    {
        basic_state result("(");
        result._name += _name;
        result._name += ",";
        result._name += s._name;
        result._name += ")";
        return result;
    }

};

/// basic_state -> std::string
std::string to_string(const basic_state & s)
{
    return s._name;
}

/// basic_state -> std::ostream
std::ostream & operator<<(std::ostream & out, const basic_state & s)
{
    out << to_string(s);
    return out;
}

#endif
