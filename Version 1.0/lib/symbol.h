#ifndef symbol_h
#define symbol_h

#include "state.h"

class symbol
{

public:

    std::string _name;

    int _rank;

public:

    //constructor

    symbol()
    :_name("unnamed symbol"), _rank(0)
    {}

    symbol(std::string n, int r = 0)
    :_name(n), _rank(r)
    {}

    //destructor

    ~symbol()
    {}

    //take value with =

    symbol & operator=(const symbol & a)
    {
        _name = a._name;
        _rank = a._rank;
        return *this;
    }

    //comparison

    bool operator==(const symbol & a) const
    {
        return (_name == a._name) && (_rank == a._rank);
    }

    bool operator!=(const symbol & a) const
    {
        return !(*this == a);
    }

    //string and print

    std::string to_string() const
    {
        std::string result = _name;
        if(_rank != 0)
        {
            result += "(";
            for(int i = 2; i <= _rank; i++)
                result += " ,";
            result += " )";
        }
        return result;
    }

    friend std::ostream & operator<<(std::ostream & out, const symbol & a)
    {
        out << a.to_string();
        return out;
    }

};

#endif
