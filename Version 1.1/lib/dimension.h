#ifndef dimension_h
#define dimension_h

#include <iostream>

#define max_dimension 10

#include "tostring.h"

class dim
{

public:

    int _data[max_dimension + 1];   /* data */
    int _size;                      /* size */

public:

    /// default constructor
    dim()
    :_size(0)
    {}

    /// constructor by reference copying
    /* create a new dimension by copying another dimension "d" */
    dim(const dim & d)
    :_size(d._size)
    {
        for(int i = 0; i < _size; i++)
            _data[i] = d._data[i];
    }

    ~dim()
    {}

    /// assignment
    /* replace the current container by a dimension "d" */
    dim & operator=(const dim & d)
    {
        for(int i = 0; i < d._size; i++)
            _data[i] = d._data[i];
        _size = d._size;
        return *this;
    }

    /// accessor
    /* access directly (reference not copy) a element in the current dimension by giving its index */
    /* the first index is 1 not 0 */
    int operator[](int i) const
    {
        return _data[i - 1];
    }

    /// upper bound
    int upper() const
    {
        int _max = -1;
        for(int i = 0; i < _size; i++)
            if(_max < _data[i])
                _max = _data[i];
        return _max;
    }

    /// lower bound
    int lower() const
    {
        int _min = 999;
        for(int i = 0; i < _size; i++)
            if(_min > _data[i])
                _min = _data[i];
        return _min;
    }

    /// element insertion
    /* add an element "e" into the current dimension */
    void add(int e)
    {
        _data[_size] = e;
        _size++;
    }

    /// element deletion
    /* delete an element "e" from the current dimension if it is already in */
    void del(const int & e)
    {
        if(contain(e))
        {
            int j;
            for(int i = 0; i < _size; i++)
                if(_data[i] == e)
                {
                    j = i;
                    break;
                }
            for(int i = j; i < _size - 1; i++)
                _data[i] = _data[i + 1];
            _size--;
        }
    }

    /// element inclusion
    /* check if an element "e" is in the current dimension */
    bool contain(int e) const
    {
        for(int i = 0; i < _size; i++)
            if(e == _data[i])
                return true;
        return false;
    }

    /// equation
    /* check if all a dimension "d" is equal to the current dimension */
    /* 2 dimensions are equal if and only if they contain exactly the same elements */
    bool operator==(const dim & d) const
    {
        for(int i = 1; i <= d._size; i++)
            if(!contain(d[i]))
                return false;
        for(int i = 0; i < _size; i++)
            if(!d.contain(_data[i]))
                return false;
        return true;
    }

    /// non equation
    /* check if all a dimension "d" is not equal to the current dimension */
    /* 2 dimensions are equal if and only if they contain exactly the same elements */
    bool operator!=(const dim & d) const
    {
        return !(*this == d);
    }

};

/// dim -> std::string
std::string to_string(const dim & d)
{
    std::string result = "";
    if(d._size == 0)
        return "{}";
    if(d._size == d.upper() + 1)
    {
        result += "[";
        result += to_string(d.upper());
        result += "]";
        return result;
    }
    if(d._size == 1)
    {
        result += "(";
        result += to_string(d[1]);
        result += ")";
        return result;
    }
    result += "{";
    for(int i = 1; i <= d._size; i++)
    {
        result += to_string(d[i]);
        result += ",";
    }
    result += "\b}";
    return result;
}

/// print a dimension
/* print a dimension by std::cout */
std::ostream & operator<<(std::ostream & out, const dim & d)
{
    out << to_string(d);
    return out;
}

#endif
