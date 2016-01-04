#ifndef basic_set_h
#define basic_set_h

#define BASIC_SET_ALLOCATION_INCREMENT 300

#include "tostring.h"
#include "basic_tuple.h"

template<typename T>
class basic_set
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
    basic_set()
    :_data(NULL), _size_repre(0), _size_alloc(0)
    {}

    /// reference copy constructor
    basic_set(const basic_set & s)
    :_size_repre(s._size_repre), _size_alloc(s._size_alloc)
    {
        _data = new T[_size_alloc];
        for(int i = 0; i < _size_repre; i++)
            _data[i] = s._data[i];
    }

    /// destructor
    ~basic_set()
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

    /// set inclusion
    bool contain(const basic_set<T> & s) const
    {
        for(int i = 0; i < s._size_repre; i++)
            if(!contain(s._data[i]))
                return false;
        return true;
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
    void add(const T & e, bool force = false)
    {
        if(force || !contain(e))
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
    }

    /// set union
    void add(const basic_set<T> & s, bool force = false)
    {
        for(int i = 0; i < s._size_repre; i++)
            add(s._data[i], force);
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
    bool operator==(const basic_set<T> & s) const
    {
        return (contain(s)) && (s.contain(*this));
    }

    /// non-equation test
    bool operator !=(const basic_set<T> & s) const
    {
        return !(*this == s);
    }

    /// assignment (replacement)
    basic_set<T> & operator=(const basic_set<T> & s)
    {
        delete[] _data;
        _data = new T[s._size_alloc];
        for(int i = 0; i < s._size_repre; i++)
            _data[i] = s._data[i];
        _size_repre = s._size_repre;
        _size_alloc = s._size_alloc;
        return *this;
    }

    /// cross product
    basic_set<T> operator*(const basic_set<T> s) const
    {
        basic_set<T> result;
        for(int i = 0; i < _size_repre; i++)
            for(int j = 0; j < s._size_repre; j++)
                result.add(_data[i] * s._data[j]);
        return result;
    }

};

/// basic_set -> std::string
template<typename T>
std::string to_string(const basic_set<T> & s)
{
    std::string result("{");
    for(int i = 0; i < s._size_repre; i++)
    {
        result += to_string(s._data[i]);
        result += ",";
    }
    if(s._size_repre != 0)
        result += "\b";
    result += "}";
    return result;
}

/// basic_set -> std::ostream
template<typename T>
std::ostream & operator<<(std::ostream & out, const basic_set<T> & s)
{
    out << to_string(s);
    return out;
}

#endif
