#ifndef dimension_h
#define dimension_h

#include <iostream>

#define max_dimension 10

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

    /// get size
    int size() const
    {
        return _size;
    }

    /// upper bound
    int upper() const
    {
        int max = -1;
        for(int i = 0; i < _size; i++)
            if(max < _data[i])
                max = _data[i];
        return max;
    }

    /// lower bound
    int lower() const
    {
        int min = 999;
        for(int i = 0; i < _size; i++)
            if(min > _data[i])
                min = _data[i];
        return min;
    }

    /// element insertion
    /* add an element "e" into the current dimension */
    void add(int e)
    {
        _data[_size] = e;
        _size++;
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

#endif
