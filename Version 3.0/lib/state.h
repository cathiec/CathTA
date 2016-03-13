#ifndef _state_h
#define _state_h

#include <iostream>

class state
{

private:

    // name of state
    std::string _name;

public:

    // default constructor
    state(std::string n = "unnamed_state")
    :_name(n)
    {}

    // reference recopy constructor
    state(const state & s)
    :_name(s._name)
    {}

    // destructor
    ~state()
    {}

    // comparison
    bool operator==(const state & s) const
    {
        return _name == s._name;
    }

    bool operator!=(const state & s) const
    {
        return _name != s._name;
    }

    bool operator<(const state & s) const
    {
        return _name < s._name;
    }

    // assignment
    state & operator=(const state & s)
    {
        _name = s._name;
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

    // to string
    std::string to_string() const
    {
        return _name;
    }

};

std::ostream & operator<<(std::ostream & out, const state & s)
{
    out << s.to_string();
    return out;
}

#endif
