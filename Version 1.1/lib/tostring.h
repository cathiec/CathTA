#ifndef to_string_h
#define to_string_h

#include <stdio.h>

#include "tree_automaton.h"

bool cath_show_dimension = false;

/// int -> std::string
std::string to_string(int i)
{
    std::string result = "";
    char temp[255];
    sprintf(temp, "%d", i);
    result += std::string(temp);
    return result;
}

/// double -> std::string
std::string to_string(double d)
{
    std::string result = "";
    char temp[255];
    sprintf(temp, "%f", d);
    result += std::string(temp);
    return result;
}

/// std::string -> std::string
/* we write this function in order to make the function to_string general */
std::string to_string(std::string s)
{
    return s;
}

/// dim -> std::string
std::string to_string(const dim & d)
{
    std::string result = "";
    if(d.size() == 0)
        return result;
    if(d.size() == d.upper() + 1)
    {
        result += "[";
        result += to_string(d.upper());
        result += "]";
        return result;
    }
    if(d.size() == 1)
    {
        result += "(";
        result += to_string(d[1]);
        result += ")";
        return result;
    }
    result += "{";
    for(int i = 1; i <= d.size(); i++)
    {
        result += to_string(d[i]);
        result += ",";
    }
    result += "\b}";
    return result;
}

/// container -> std::string
template<typename T>
std::string to_string(const container<T> & c)
{
    std::string result = "{";
    for(int i = 1; i <= c.size(); i++)
    {
        result += to_string(c[i]);
        result += ",";
    }
    if(c.size() != 0)
        result += "\b";
    result += "}";
    if(cath_show_dimension)
    {
        result += "#";
        result += to_string(c.dimension());
    }
    return result;
}

/// state -> std::string
std::string to_string(const state & s)
{
    return s._name;
}

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

/// product_state -> std::string
std::string to_string(const product_state & ps)
{
    std::string result = "(";
    result += to_string(ps._1);
    result += ",{";
    if(ps._2.size() == 0)
    {
        result += "})";
        return result;
    }
    for(int i = 1; i <= ps._2.size(); i++)
    {
        result += to_string(ps._2[i]);
        result += ",";
    }
    result += "\b})";
    return result;
}

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

#endif
