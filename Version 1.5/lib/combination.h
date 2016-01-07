#ifndef combination_h
#define combination_h

#include "basic_set.h"

template<typename T>
class combination
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
    combination()
    :_data(NULL), _size_repre(0), _size_alloc(0)
    {}

    /// reference copy constructor
    combination(const combination & c)
    :_size_repre(c._size_repre), _size_alloc(c._size_alloc)
    {
        _data = new T[_size_alloc];
        for(int i = 0; i < _size_repre; i++)
            _data[i] = c._data[i];
    }

    /// destructor
    ~combination()
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

    /// equation test
    bool operator==(const combination<T> & c) const
    {
        if(_size_repre != c._size_repre)
            return false;
        for(int i = 0; i < _size_repre; i++)
            if(_data[i] != c._data[i])
                return false;
        return true;
    }

    /// non-equation test
    bool operator !=(const combination<T> & c) const
    {
        return !(*this == c);
    }

    /// assignment (replacement)
    combination<T> & operator=(const combination<T> & c)
    {
        delete[] _data;
        _data = new T[c._size_alloc];
        for(int i = 0; i < c._size_repre; i++)
            _data[i] = c._data[i];
        _size_repre = c._size_repre;
        _size_alloc = c._size_alloc;
        return *this;
    }

};

/// combination -> std::string
template<typename T>
std::string to_string(const combination<T> & c)
{
    std::string result("=");
    for(int i = 0; i < c._size_repre; i++)
    {
        result += to_string(c._data[i]);
        result += ",";
    }
    if(c._size_repre != 0)
        result += "\b";
    result += "=";
    return result;
}

/// combination -> std::ostream
template<typename T>
std::ostream & operator<<(std::ostream & out, const combination<T> & c)
{
    out << to_string(c);
    return out;
}

#endif
