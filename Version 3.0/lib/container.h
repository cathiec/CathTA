#ifndef _container_h
#define _state_h

#include <iostream>
#include <set>
#include <vector>
#include <algorithm>

template<typename T>
bool has_value(const std::vector<T> & c, const T & v)
{
    for(int i = 0; i <= c.size(); i++)
        if(c[i] == v)
            return true;
    return false;
}

template<typename T>
bool is_subset(const std::set<T> & c1, const std::set<T> & c2)
{
    typename std::set<T>::const_iterator it;
    for(it = c1.begin(); it != c1.end(); it++)
    {
        if(c2.find(*it) == c2.end())
            return false;
    }
    return true;
}

template<typename T>
bool is_subset(const std::set<T> & c1, const std::vector<T> & c2)
{
    typename std::set<T>::const_iterator it;
    for(it = c1.begin(); it != c1.end(); it++)
    {
        if(has_value(c2, *it))
            return false;
    }
    return true;
}

template<typename T>
bool is_subset(const std::vector<T> & c1, const std::set<T> & c2)
{
    typename std::vector<T>::const_iterator it;
    for(it = c1.begin(); it != c1.end(); it++)
    {
        if(c2.find(*it) == c2.end())
            return false;
    }
    return true;
}

template<typename T>
bool is_subset(const std::vector<T> & c1, const std::vector<T> & c2)
{
    typename std::vector<T>::const_iterator it;
    for(it = c1.begin(); it != c1.end(); it++)
    {
        if(has_value(c2, *it))
            return false;
    }
    return true;
}

#endif
