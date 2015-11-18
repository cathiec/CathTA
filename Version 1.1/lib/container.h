#ifndef container_h
#define container_h

#include "dimension.h"

template<typename T>
class container
{

private:

    T * _data;          /* data */
    int _size;          /* size */
    dim _dimension;     /* dimension (used for dimension-bounded inclusion checking) */

public:

    /// default constructor
    container()
    :_data(NULL), _size(0)
    {}

    /// constructor by reference copying
    /* create a new container by copying another container "c" */
    container(const container<T> & c)
    :_size(c._size), _dimension(c._dimension)
    {
        _data = new T[_size];
        for(int i = 0; i < _size; i++)
            _data[i] = c._data[i];
    }

    /// destructor
    ~container()
    {
        delete[] _data;
    }

    /// accessor
    /* access directly (reference not copy) a element in the current container by giving its index */
    /* the first index is 1 not 0 */
    T & operator[](int i) const
    {
        return _data[i - 1];
    }

    /// element inclusion
    /* check if a element "e" is in the current container */
    bool contain(const T & e) const
    {
        for(int i = 0; i < _size; i++)
            if(e == _data[i])
                return true;
        return false;
    }

    /// container inclusion
    /* check if all the elements of a container "c" are also in the current container */
    bool contain(const container<T> & c) const
    {
        for(int i = 1; i <= c._size; i++)
            if(!contain(c[i]))
                return false;
        return true;
    }

    /// element insertion
    /* add an element "e" into the current container */
    /* if "f" is true, then the insertion is forcible (the repetition of elements is allowed) */
    /* if "f" is false, then no repetition of elements is allowed */
    void add(const T & e, bool f = false)
    {
        if(f || (!contain(e)))
        {
            T * temp = new T[_size + 1];
            for(int i = 0; i < _size; i++)
                temp[i] = _data[i];
            temp[_size] = e;
            delete[] _data;
            _data = temp;
            _size++;
        }
    }

    /// container union
    /* add all the elements of a container "c" into the current container */
    /* if "f" is true, then the insertion is forcible (the repetition of elements is allowed) */
    /* if "f" is false, then no repetition of elements is allowed */
    void add(const container<T> & c, bool f = false)
    {
        for(int i = 1; i <= c._size; i++)
            add(c[i], f);
    }

    /// element deletion
    /* remove an element "e" from the current container if it is already in */
    /* if "e" appears several times in the current container, then just remove the first occurrence */
    void remove(const T & e)
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

    /// equation
    /* check if all a container "c" is equal to the current container */
    /* 2 containers are equal if and only if they contain exactly the same elements no matter their occurrence */
    /* dimension does not make any sense in equation */
    bool operator==(const container<T> & c) const
    {
        return (contain(c) && c.contain(*this));
    }

    /// non equation
    /* check if all a container "c" is not equal to the current container */
    /* 2 containers are equal if and only if they contain exactly the same elements no matter their occurrence */
    /* dimension does not make any sense in equation */
    bool operator!=(const container<T> & c) const
    {
        return !(*this == c);
    }

    /// get size
    int size() const
    {
        return _size;
    }

    /// get dimension
    dim dimension() const
    {
        return _dimension;
    }

    /// modify dimension
    void set_dimension(const dim & d)
    {
        _dimension = d;
    }

    /// assignment
    /* replace all the elements of the current container by the elements of a container "c" */
    container<T> & operator=(const container<T> & c)
    {
        delete[] _data;
        _data = new T[c._size];
        for(int i = 0; i < c._size; i++)
            _data[i] = c._data[i];
        _size = c._size;
        _dimension = c._dimension;
        return *this;
    }

};

#endif
