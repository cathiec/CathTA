#ifndef _symbol_h
#define _symbol_h

#include <iostream>
#include <stdio.h>

class symbol
{

private:

    // name of symbol
    std::string _name;

    // arity
    int _arity;

public:

    // default constructor
    symbol(std::string n = "unnamed_symbol", int a = 0)
    :_name(n), _arity(a)
    {}

    // reference recopy constructor
    symbol(const symbol & s)
    :_name(s._name), _arity(s._arity)
    {}

    // destructor
    ~symbol()
    {}

    // comparison
    bool operator==(const symbol & s) const
    {
        return (_name == s._name && _arity == s._arity);
    }

    bool operator<(const symbol & s) const
    {
        if(_name != s._name)
            return _name < s._name;
        else
            return _arity < s._arity;
    }

    // assignment
    symbol & operator=(const symbol & s)
    {
        _name = s._name;
        _arity = s._arity;
        return *this;
    }

    // get name
    std::string get_name() const
    {
        return _name;
    }

    // set name
    void set_name(std::string n)
    {
        _name = n;
    }

    // get arity
    int get_arity() const
    {
        return _arity;
    }

    // set arity
    void set_arity(int a)
    {
        _arity = a;
    }

    // to string
    std::string to_string() const
    {
        std::string result = _name;
        result += ':';
        char temp[10];
        sprintf(temp, "%d", _arity);
        result += std::string(temp);
        return result;
    }

};

std::ostream & operator<<(std::ostream & out, const symbol & s)
{
    out << s.to_string();
    return out;
}

#endif
