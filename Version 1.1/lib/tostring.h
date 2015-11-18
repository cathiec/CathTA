#ifndef to_string_h
#define to_string_h

#include <stdio.h>

#include "container.h"

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

#endif
