#ifndef symbol_h
#define symbol_h

#include <iostream>

class symbol
{

public:

    std::string _name;  /* name */
    int _rank;          /* rank (number of parameters) */

public:

    /// default constructor
    symbol(std::string n = "unnamed", int r = 0)
    :_name(n), _rank(r)
    {}

    /// constructor by reference copying
    /* create a new symbol by copying another symbol "s" */
    symbol(const symbol & s)
    :_name(s._name), _rank(s._rank)
    {}

    /// destructor
    ~symbol()
    {}

    /// equation
    /* check if all a symbol "s" is equal to the current symbol */
    /* 2 symbols are equal if and only if their names are same and ranks are equal*/
    bool operator==(const symbol & s) const
    {
        return (_name == s._name) && (_rank == s._rank);
    }

    /// non equation
    /* check if all a symbol "s" is not equal to the current symbol */
    /* 2 symbols are equal if and only if their names are same and ranks are equal*/
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

/// print a symbol
/* print a symbol by std::cout */
std::ostream & operator<<(std::ostream & out, const symbol & s)
{
    out << to_string(s);
    return out;
}

#endif
