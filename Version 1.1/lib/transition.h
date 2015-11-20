#ifndef transition_h
#define transition_h

#include "symbol.h"
#include "state.h"
#include "container.h"

class transition
{

public:

    symbol _alpha;              /* symbol */
    __container<state> _input;  /* parameters of the symbol */
    state _output;              /* result */

public:

    /// default constructor
    transition()
    {}

    /// constructor by parser
    /* parse a string in order to construct a transition */
    transition(std::string s)
    {
        for(int i = 1; i <= s.length(); i++)
        {
            if(s[i - 1] == ' ' || s[i - 1] == '\t')
            {
                s.replace(i - 1, 1, 0, ' ');
                i--;
            }
        }
        int pos_left_par = s.find('(');
        if(pos_left_par != -1 && (s.find(')') - pos_left_par > 1))
        {
            _alpha._name = s.substr(0, pos_left_par);
            std::string temp = s.substr(pos_left_par + 1);
            while(true)
            {
                int pos_comma = temp.find(',');
                if(pos_comma == -1)
                {
                    int pos_right_par = temp.find(')');
                    _input.add(state(temp.substr(0, pos_right_par)), true);
                    _alpha._rank++;
                    break;
                }
                else
                {
                    _input.add(state(temp.substr(0, pos_comma)), true);
                    _alpha._rank++;
                    temp = temp.substr(pos_comma + 1);
                }
            }
            int pos_first_minus = s.find('-');
            _output._name = s.substr(pos_first_minus + 2);
        }
        else
        {
            int pos_first_minus = s.find('-');
            _alpha._name = s.substr(0, pos_first_minus);
            _output._name = s.substr(pos_first_minus + 2);
        }
    }

    /// destructor
    ~transition()
    {}

    /// assignment
    /* replace the current transition by a transition state "t" */
    transition & operator=(const transition & t)
    {
        _alpha = t._alpha;
        _input = t._input;
        _output = t._output;
        return *this;
    }

    /// equation
    /* check if all a transition "t" is equal to the current transition */
    /* 2 transitions are equal if and only if their symbols, inputs and outputs are same */
    bool operator==(const transition & t) const
    {
        return (_alpha == t._alpha) && (_input == t._input) && (_output == t._output);
    }

    /// non equation
    /* check if all a transition "t" is not equal to the current transition */
    /* 2 transitions are equal if and only if their symbols, inputs and outputs are same */
    bool operator!=(const transition & t) const
    {
        return !(*this == t);
    }

    /// cross product of transition sets
    /* compute the cross product of 2 sets of transitions */
    friend container<transition> cross_product_transitions(const container<transition> & st1, const container<transition> & st2)
    {
        container<transition> result;
        for(int i = 1; i <= st1._size; i++)
        {
            for(int j = 1; j <= st2._size; j++)
            {
                if(st1[i]._alpha == st2[j]._alpha)
                {
                    transition temp;
                    temp._alpha = st1[i]._alpha;
                    for(int k = 1; k <= st1[i]._alpha._rank; k++)
                        temp._input.add(st1[i]._input[k] * st2[j]._input[k], true);
                    temp._output = st1[i]._output * st2[j]._output;
                    result.add(temp);
                }
            }
        }
        return result;
    }

};

/// transition -> std::string
std::string to_string(const transition & t)
{
    std::string result = t._alpha._name;
    if(t._alpha._rank != 0)
    {
        result += "(";
        result += to_string(t._input[1]);
        for(int i = 2; i <= t._alpha._rank; i++)
        {
            result += ",";
            result += to_string(t._input[i]);
        }
        result += ")";
    }
    result += " -> ";
    result += to_string(t._output);
    return result;
}

/// print a transition
/* print a transition by std::cout */
std::ostream & operator<<(std::ostream & out, const transition & t)
{
    out << to_string(t);
    return out;
}

#endif
