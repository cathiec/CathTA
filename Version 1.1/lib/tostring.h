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

#endif
