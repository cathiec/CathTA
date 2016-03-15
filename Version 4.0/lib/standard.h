#ifndef _standard_h
#define _standard_h

#include <iostream>
#include <ext/hash_map>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define str std::string
#define vec std::vector
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

#endif
