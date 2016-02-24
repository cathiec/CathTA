#ifndef symbol_h
#define symbol_h

#include "tostring.h"

class symbol
{

public:

    /* name */
    std::string _name;

    /* number of arguments */
    int _rank;

public:

    /// default constructor
    symbol(std::string n = "unnamed", int r = 0)
    :_name(n), _rank(r)
    {}

    /// reference copy constructor
    symbol(const symbol & s)
    :_name(s._name), _rank(s._rank)
    {}

    /// destructor
    ~symbol()
    {}

    /// equation test
    bool operator==(const symbol & s) const
    {
        return (_name == s._name) && (_rank == s._rank);
    }

    /// non-equation test
    bool operator!=(const symbol & s) const
    {
        return !(*this == s);
    }

};

/// symbol -> std::string
std::string to_string(const symbol & s)
{
    std::string result = s._name;
    if(s._rank != 0)
    {
        result += "(";
        for(int i = 2; i <= s._rank; i++)
            result += " ,";
        result += " )";
    }
    return result;
}

/// symbol -> std::ostream
std::ostream & operator<<(std::ostream & out, const symbol & s)
{
    out << to_string(s);
    return out;
}

#endif
