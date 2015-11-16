#ifndef state_h
#define state_h

#include <iostream>

class state
{

public:

    std::string _name;

public:

    //constructor

    state()
    :_name("unnamed state")
    {}

    state(std::string n)
    :_name(n)
    {}

    state(const state & s)
    :_name(s._name)
    {}

    //destructor

    ~state()
    {}

    //take value with =

    state & operator=(const state & s)
    {
        _name = s._name;
        return *this;
    }

    //comparison

    bool operator==(const state & s) const
    {
        return _name == s._name;
    }

    bool operator!=(const state & s) const
    {
        return !(*this == s);
    }

    //cross product
    state operator*(const state & s) const
    {
        state result("(");
        result._name += _name;
        result._name += ",";
        result._name += s._name;
        result._name += ")";
        return result;
    }

    //string and print

    std::string to_string() const
    {
        return _name;
    }

    friend std::ostream & operator<<(std::ostream & out, const state & s)
    {
        out << s.to_string();
        return out;
    }

};

#endif
