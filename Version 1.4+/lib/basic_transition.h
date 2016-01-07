#ifndef basic_transition_h
#define basic_transition_h

#include "symbol.h"
#include "basic_state.h"
#include "basic_set.h"

class basic_transition
{

public:

    /* symbol */
    symbol _alpha;

    /* arguments (inputs) */
    basic_tuple<basic_state> _inputs;

    /* result (output) */
    basic_state _output;

public:

    /// default constructor
    basic_transition()
    {}

    /// reference copy constructor
    basic_transition(const basic_transition & t)
    :_alpha(t._alpha), _inputs(t._inputs), _output(t._output)
    {}

    /// constructor by parsing a string
    basic_transition(std::string s)
    {
        // delete all the ' ' and '\t'
        for(int i = 0; i < s.length(); i++)
        {
            if(s[i] == ' ' || s[i] == '\t')
            {
                s.replace(i, 1, 0, ' ');
                i--;
            }
        }
        // find the first '('
        int pos_left_par = s.find('(');
        if(pos_left_par != -1 && (s.find(')') - pos_left_par > 1))
        {
            // find the symbol
            _alpha._name = s.substr(0, pos_left_par);
            std::string temp = s.substr(pos_left_par + 1);
            // find the inputs
            while(true)
            {
                int pos_comma = temp.find(',');
                if(pos_comma == -1)
                {
                    int pos_right_par = temp.find(')');
                    _inputs.add(basic_state(temp.substr(0, pos_right_par)));
                    _alpha._rank++;
                    break;
                }
                else
                {
                    _inputs.add(basic_state(temp.substr(0, pos_comma)));
                    _alpha._rank++;
                    temp = temp.substr(pos_comma + 1);
                }
            }
            // find the output
            int pos_first_minus = s.find('-');
            _output._name = s.substr(pos_first_minus + 2);
        }
        // if there is no input
        else
        {
            int pos_first_minus = s.find('-');
            _alpha._name = s.substr(0, pos_first_minus);
            _output._name = s.substr(pos_first_minus + 2);
        }
    }

    /// destructor
    ~basic_transition()
    {}

    /// assignment (replacement)
    basic_transition & operator=(const basic_transition & t)
    {
        _alpha = t._alpha;
        _inputs = t._inputs;
        _output = t._output;
        return *this;
    }

    /// equation test
    bool operator==(const basic_transition & t) const
    {
        return (_alpha == t._alpha) && (_inputs == t._inputs) && (_output == t._output);
    }

    /// non-equation test
    bool operator!=(const basic_transition & t) const
    {
        return !(*this == t);
    }

    /// cross product of transition sets
    friend basic_set<basic_transition> cross_product_transitions(const basic_set<basic_transition> & st1, const basic_set<basic_transition> & st2)
    {
        basic_set<basic_transition> result;
        for(int i = 1; i <= st1.size(); i++)
        {
            for(int j = 1; j <= st2.size(); j++)
            {
                if(st1[i]._alpha == st2[j]._alpha)
                {
                    basic_transition temp;
                    temp._alpha = st1[i]._alpha;
                    for(int k = 1; k <= st1[i]._alpha._rank; k++)
                        temp._inputs.add(st1[i]._inputs[k] * st2[j]._inputs[k]);
                    temp._output = st1[i]._output * st2[j]._output;
                    result.add(temp);
                }
            }
        }
        return result;
    }

};

/// basic_transition -> std::string
std::string to_string(const basic_transition & t)
{
    std::string result = t._alpha._name;
    if(t._alpha._rank != 0)
    {
        result += "(";
        result += to_string(t._inputs[1]);
        for(int i = 2; i <= t._alpha._rank; i++)
        {
            result += ",";
            result += to_string(t._inputs[i]);
        }
        result += ")";
    }
    result += " -> ";
    result += to_string(t._output);
    return result;
}

/// basic_transition -> std::ostream
std::ostream & operator<<(std::ostream & out, const basic_transition & t)
{
    out << to_string(t);
    return out;
}

#endif
