#ifndef dim_table_h
#define dim_table_h

#include "state_with_dimension.h"

class dim_table
{

public:

    int _max_dim;
    basic_set<std::string> _state;
    basic_tuple<int> _min;
    basic_tuple<int> _max;

public:

    /// default constructor
    dim_table()
    :_max_dim(0)
    {}

    /// reference copy constructor
    dim_table(const dim_table &dt)
    :_state(dt._state), _min(dt._min), _max(dt._max), _max_dim(dt._max_dim)
    {}

    /// destructor
    ~dim_table()
    {}

    /// add data
    void add_data(const state_with_dimension & s)
    {
        _state.add(s._name, true);
        _min.add(s._min_dimension);
        _max.add(s._dimension);
        if(s._dimension > _max_dim)
            _max_dim = s._dimension;
    }

    /// find the index of an element
    int find(std::string s) const
    {
        for(int i = 1; i <= _state.size(); i++)
        {
            if(_state[i] == s)
                return i;
        }
        return -1;
    }

};

/// dim_table -> std::string
std::string to_string(const dim_table & dt)
{
    std::string result;
    for(int i = 1; i <= dt._state.size(); i++)
    {
        result += dt._state[i];
        result += "\t";
        result += to_string(dt._min[i]);
        result += "\t";
        result += to_string(dt._max[i]);
        result += "\n";
    }
    return result;
}

/// dim_table -> std::ostream
std::ostream & operator<<(std::ostream & out, const dim_table & dt)
{
    out << to_string(dt);
    return out;
}

#endif
