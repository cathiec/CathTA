#ifndef set_h
#define set_h

#include <iostream>
#include "bin_index.h"
#include "box.h"

template<typename T>
class box;

template<typename T>
class set
{

public:

    T * _data;
    int _size;
    int _dimension;

public:

    set()
    :_data(NULL), _size(0), _dimension(-1)
    {}

    set(const set<T> & s)
    :_size(s._size), _dimension(s._dimension)
    {
        _data = new T[_size];
        for(int i = 0; i < _size; i ++)
            _data[i] = s._data[i];
    }

    ~set()
    {
        delete[] _data;
    }

    bool contain(const T & e) const
    {
        for(int i = 0; i < _size; i++)
        {
            if(e == _data[i])
                return true;
        }
        return false;
    }

    bool contain(const set<T> & s) const
    {
        for(int i = 1; i <= s.size(); i++)
            if(!contain(s[i]))
                return false;
        return true;
    }

    bool contain(const box<T> & b) const
    {
        for(int i = 1; i <= b.size(); i++)
            if(!contain(b[i]))
                return false;
        return true;
    }

    void remove(const T & e)
    {
        if(contain(e))
        {
            T * temp = new T[_size - 1];
            int j = 0;
            for(int i = 0; i < _size; i ++)
                if(!(_data[i] == e))
                    temp[j++] = _data[i];
            delete[] _data;
            _data = temp;
            _size--;
        }
    }

    void add(const T & e, bool force = false)
    {
        if(force == true || !contain(e))
        {
            T * temp = new T[_size + 1];
            for(int i = 0; i < _size; i ++)
                temp[i] = _data[i];
            delete[] _data;
            temp[_size] = e;
            _data = temp;
            _size++;
        }
    }

    void add(const set<T> & s)
    {
        for(int i = 1; i <= s.size(); i++)
            add(s[i]);
    }

    void add(const box<T> & b)
    {
        for(int i = 1; i <= b.size(); i++)
            add(b[i]);
    }

    bool operator==(const set<T> & s) const
    {
        return (s.contain(*this) && contain(s));
    }

    bool operator!=(const set<T> & s) const
    {
        return !(*this == s);
    }

    set<T> & operator=(const set<T> & s)
    {
        _dimension = s._dimension;
        delete[] _data;
        _size = s._size;
        _data = new T[_size];
        for(int i = 0; i < _size; i ++)
            _data[i] = s._data[i];
        return *this;
    }

    set<T> & operator+=(const T & e)
    {
        add(e);
        return *this;
    }

    set<T> & operator+=(const set<T> & s)
    {
        add(s);
        return *this;
    }

    set<T> operator+(const T & e) const
    {
        set<T> result(*this);
        result += e;
        return result;
    }

    set<T> operator+(const set<T> & s) const
    {
        set<T> result(*this);
        result += s;
        return result;
    }

    set<T> operator*(const set<T> & s) const
    {
        set<T> result;
        for(int i = 1; i <= _size; i++)
            for(int j = 1; j <= s.size(); j++)
                result.add(_data[i - 1] * s[j]);
        return result;
    }

    int size() const
    {
        return _size;
    }

    T & operator[](int i) const
    {
        return _data[i - 1];
    }

    bool has_intersection_with(const set<T> & s) const
    {
        for(int i = 0; i < _size; i++)
            for(int j = 1; j <= s.size(); j++)
                if(_data[i] == s[j])
                    return true;
        return false;
    }

    set<T> intersection(const set<T> & s) const
    {
        set<T> result;
        for(int i = 0; i < _size; i++)
            for(int j = 1; j <= s.size(); j++)
                if(_data[i] == s[j])
                    result += s[j];
        return result;
    }

    set<T> subset_at_bin(const bin_index & b) const
    {
        set<T> result;
        for(int i = 0; i < _size; i++)
            if(b[i] == 1)
                result += _data[i];
        return result;
    }

    set<set<T> > all_subsets() const
    {
        set<set<T> > result;
        for(int i = 0; i <= (1 << _size) - 1; i++)
            result += subset_at_bin(bin_index(i));
        return result;
    }

    friend std::ostream & operator<<(std::ostream & out, const set<T> & s)
    {
        if(s.size() == 0)
        {
            out << "{}";
            return out;
        }
        out << "{ ";
        for(int i = 1; i <= s._size; i++)
        {
            out << s[i];
            out << ", ";
        }
        out << "\b\b }_" << s._dimension << "_";
        return out;
    }

};

#endif
