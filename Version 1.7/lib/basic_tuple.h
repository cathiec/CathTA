#ifndef basic_tuple_h
#define basic_tuple_h

#define BASIC_TUPLE_ALLOCATION_INCREMENT 5

#include "basic_set.h"

template<typename T>
class basic_tuple
{

public:

    /* data(array) */
    T * _data;

    /* size of representation */
    int _size_repre;

    /* size of allocation */
    int _size_alloc;

public:

    /// default constructor
    basic_tuple()
    :_data(NULL), _size_repre(0), _size_alloc(0)
    {}

    /// reference copy constructor
    basic_tuple(const basic_tuple & t)
    :_size_repre(t._size_repre), _size_alloc(t._size_alloc)
    {
        _data = new T[_size_alloc];
        for(int i = 0; i < _size_repre; i++)
            _data[i] = t._data[i];
    }

    /// destructor
    ~basic_tuple()
    {
        delete[] _data;
    }

    /// size
    int size() const
    {
        return _size_repre;
    }

    /// accessor (the first index is 1 !!! NOT 0 !!!)
    T & operator[](int i ) const
    {
        return _data[i - 1];
    }

    /// element inclusion
    bool contain(const T & e) const
    {
        for(int i = 0; i < _size_repre; i++)
            if(e == _data[i])
                return true;
        return false;
    }

    /// tuple inclusion
    bool contain(const basic_tuple<T> & t) const
    {
        for(int i = 0; i < t._size_repre; i++)
            if(!contain(t._data[i]))
                return false;
        return true;
    }

    /// element insertion
    void add(const T & e)
    {
        if(_size_repre + 1 > _size_alloc)
        {
            T * temp = new T[_size_repre + BASIC_SET_ALLOCATION_INCREMENT];
            for(int i = 0; i < _size_repre; i++)
                temp[i] = _data[i];
            temp[_size_repre] = e;
            delete[] _data;
            _data = temp;
            _size_alloc = _size_repre + BASIC_SET_ALLOCATION_INCREMENT;
            _size_repre++;
        }
        else
        {
            _data[_size_repre] = e;
            _size_repre++;
        }
    }

    /// element deletion
    void del(const T & e)
    {
        int rep = -1;
        for(int i = 0; i < _size_repre; i++)
            if(_data[i] == e)
            {
                rep = i;
                break;
            }
        if(rep != -1)
        {
            for(int i = rep; i < _size_repre - 1; i++)
                _data[i] = _data[i + 1];
            _size_repre--;
        }
    }

    /// equation test
    bool operator==(const basic_tuple<T> & t) const
    {
        return (contain(t)) && (t.contain(*this));
    }

    /// non-equation test
    bool operator !=(const basic_tuple<T> & t) const
    {
        return !(*this == t);
    }

    /// assignment (replacement)
    basic_tuple<T> & operator=(const basic_tuple<T> & t)
    {
        delete[] _data;
        _data = new T[t._size_alloc];
        for(int i = 0; i < t._size_repre; i++)
            _data[i] = t._data[i];
        _size_repre = t._size_repre;
        _size_alloc = t._size_alloc;
        return *this;
    }

};

/// basic_tuple -> std::string
template<typename T>
std::string to_string(const basic_tuple<T> & t)
{
    std::string result("<");
    for(int i = 0; i < t._size_repre; i++)
    {
        result += to_string(t._data[i]);
        result += ",";
    }
    if(t._size_repre != 0)
        result += "\b";
    result += ">";
    return result;
}

/// basic_tuple -> std::ostream
template<typename T>
std::ostream & operator<<(std::ostream & out, const basic_tuple<T> & t)
{
    out << to_string(t);
    return out;
}

#endif
