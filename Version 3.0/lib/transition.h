#ifndef _transition_h
#define _transition_h

#include "state.h"
#include <vector>

class transition
{

private:

    // name of symbol
    std::string _name;

    // inputs
    std::vector<state> _inputs;

    // output
    state _output;

public:

    // default constructor
    transition()
    {}

    // reference recopy constructor
    transition(const transition & t)
    :_name(t._name), _inputs(t._inputs), _output(t._output)
    {}

    // destructor
    ~transition()
    {}

    // comparison
    bool operator==(const transition & t) const
    {
        return (_name == t._name && _inputs == t._inputs && _output == t._output);
    }

    bool operator<(const transition & t) const
    {
        if(_name != t._name)
            return _name < t._name;
        else
            return _inputs.size() < t._inputs.size();
    }

    // assignment
    transition & operator=(const transition & t)
    {
        _name = t._name;
        _inputs = t._inputs;
        _output = t._output;
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
        return _inputs.size();
    }

    // get output
    state get_output() const
    {
        return _output;
    }

    // set output
    void set_output(const state & o)
    {
        _output = o;
    }

    // add input
    void add_input(const state & i)
    {
        _inputs.push_back(i);
    }

    // get input by giving index (starting from 1)
    const state & get_input(int i) const
    {
        return _inputs[i - 1];
    }

    // to string
    std::string to_string() const
    {
        std::string result = _name;
        if(_inputs.size() > 0)
        {
            result += '(';
            for(int i = 1; i <= _inputs.size(); i++)
            {
                result += _inputs[i - 1].to_string();
                result += ',';
            }
            result.erase(result.length() - 1);
            result += ')';
        }
        result += " -> ";
        result += _output.to_string();
        return result;
    }

};

std::ostream & operator<<(std::ostream & out, const transition & t)
{
    out << t.to_string();
    return out;
}

#endif
