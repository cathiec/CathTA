#ifndef _standard_h
#define _standard_h

#include <iostream>
#include <ext/hash_map>
#include <vector>
#include <set>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <hash_map>
#include <algorithm>

#define str std::string
#define vec std::vector
#define set std::set
#define hmp __gnu_cxx::hash_map
#define ifs std::ifstream
#define ofs std::ofstream
#define os  std::ostream

#define variables   public
#define functions   public

struct hash_str{
        size_t operator()(const str & s) const
        {
                return __gnu_cxx::__stl_hash_string(s.c_str());
        }
};

template<typename T>
void print(const T & e)
{
    std::cout << e;
}

template<typename T>
void println(const T & e)
{
    std::cout << e << std::endl;
}

template<typename T>
os & operator<<(os & out, const vec<T> & c)
{
    out << '(';
    for(int i = 0; i < c.size(); i++)
        out << c[i] << ',';
    if(c.size() > 0)
        out << '\b';
    out << ')';
    return out;
}

template<typename T>
void sort_vector(vec<T> & c)
{
    std::sort(c.begin(), c.end());
}

template<typename T>
bool exist_in(const T & e, const vec<T> & c)
{
    for(int i = 0; i < c.size(); i++)
        if(e == c[i])
            return true;
    return false;
}

template<typename T>
bool is_subset(const vec<T> & c1, const vec<T> & c2)
{
    if(c1.size() > c2.size())
        return false;
    for(int i = 0; i < c1.size(); i++)
        if(!exist_in(c1[i], c2))
            return false;
    return true;
}

#endif
