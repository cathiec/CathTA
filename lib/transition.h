#ifndef TRANSITION_h
#define TRANSITION_h

#include "set.h"
#include "box.h"
#include "symbol.h"

class transition
{

public:

    symbol _alpha;

    box<state> _input;

    state _output;

public:

    //constructor

    transition()
    {}

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
        if(pos_left_par != -1)
        {
            _alpha._name = s.substr(0, pos_left_par);
            std::string temp = s.substr(pos_left_par + 1);
            while(true)
            {
                int pos_comma = temp.find(',');
                if(pos_comma == -1)
                {
                    int pos_right_par = temp.find(')');
                    _input.add(state(temp.substr(0, pos_right_par)));
                    _alpha._rank++;
                    break;
                }
                else
                {
                    _input.add(state(temp.substr(0, pos_comma)));
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

    //destructor

    ~transition()
    {}

    //take value with =

    transition & operator=(const transition & t)
    {
        _alpha = t._alpha;
        _input = t._input;
        _output = t._output;
        return *this;
    }

    //comparison

    bool operator==(const transition & t) const
    {
        return (_alpha == t._alpha) && (_input == t._input) && (_output == t._output);
    }

    bool operator!=(const transition & t) const
    {
        return !(*this == t);
    }

    //string and print

    std::string to_string() const
    {
        std::string result = _alpha._name;
        if(_alpha._rank != 0)
        {
            result += "(";
            result += _input[1].to_string();
            for(int i = 2; i <= _alpha._rank; i++)
            {
                result += ",";
                result += _input[i].to_string();
            }
            result += ")";
        }
        result += " -> ";
        result += _output.to_string();
        return result;
    }

    friend std::ostream & operator<<(std::ostream & out, const transition & t)
    {
        out << t.to_string();
        return out;
    }

    //cross product

    friend set<transition> cross_product_transitions(const set<transition> & st1, const set<transition> & st2)
    {
        set<transition> result;
        for(int i = 1; i <= st1.size(); i++)
        {
            for(int j = 1; j <= st2.size(); j++)
            {
                if(st1[i]._alpha == st2[j]._alpha)
                {
                    transition temp;
                    temp._alpha = st1[i]._alpha;
                    for(int k = 1; k <= st1[i]._alpha._rank; k++)
                        temp._input.add(st1[i]._input[k] * st2[j]._input[k]);
                    temp._output = st1[i]._output * st2[j]._output;
                    result.add(temp);
                }
            }
        }
        return result;
    }

};

#endif
