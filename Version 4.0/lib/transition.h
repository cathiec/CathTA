#ifndef _transition_h
#define _transition_h

#include "standard.h"

class transition
{

variables:

    str symbol;

    vec<str> input;

    str output;

functions:

    transition()
    {}

    ~transition()
    {}

    transition & operator=(const transition & t)
    {
        symbol = t.symbol;
        input = t.input;
        output = t.output;
    }

    str to_string() const
    {
        str result = "";
        result += symbol;
        if(input.size() > 0)
        {
            result += '(';
            for(int j = 0; j < input.size(); j++)
            {
                result += input[j];
                result += ',';
            }
            result.erase(result.end() - 1);
            result += ")";
        }
        result += " -> ";
        result += output;
        return result;
    }

};

os & operator<<(os & out, const transition & t)
{
    out << t.to_string();
    return out;
}

#endif
