#ifndef box_h
#define box_h

#include <iostream>
#include "set.h"

template<typename T>
class set;

template<typename T>
class box
{

public:

    T * _data;
    int _size;

public:

    box()
    :_data(NULL), _size(0)
    {}

    box(const box<T> & b)
    :_size(b._size)
    {
        _data = new T[_size];
        for(int i = 0; i < _size; i ++)
            _data[i] = b._data[i];
    }

    ~box()
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

    bool contain(const box<T> & b) const
    {
        for(int i = 1; i <= b.size(); i++)
            if(!contain(b[i]))
                return false;
        return true;
    }

    bool contain(const set<T> & s) const
    {
        for(int i = 1; i <= s.size(); i++)
            if(!contain(s[i]))
                return false;
        return true;
    }

    void remove(const T & e)
    {
        if(contain(e))
        {
            T * temp = new T[_size - 1];
            int i, j;
            for(i = 0; i < _size; i++)
            {
                if(_data[i] == e)
                {
                    j = i;
                    break;
                }
                else
                    temp[i] = _data[i];
            }
            while(j < _size - 1)
            {
                temp[j] = _data[j + 1];
                j++;
            }
            delete[] _data;
            _data = temp;
            _size--;
        }
    }

    void add(const T & e)
    {
        T * temp = new T[_size + 1];
        for(int i = 0; i < _size; i ++)
            temp[i] = _data[i];
        delete[] _data;
        temp[_size] = e;
        _data = temp;
        _size++;

    }

    void add(const box<T> & b)
    {
        for(int i = 1; i <= b.size(); i++)
            add(b[i]);
    }

    void add(const set<T> & s)
    {
        for(int i = 1; i <= s.size(); i++)
            add(s[i]);
    }

    bool operator==(const box<T> & b) const
    {
        if(_size != b._size)
            return false;
        else
        {
            for(int i = 0; i < _size; i++)
                if(_data[i] != b._data[i])
                    return false;
            return true;
        }
    }

    bool operator!=(const box<T> & b) const
    {
        return !(*this == b);
    }

    box<T> & operator=(const box<T> & b)
    {
        delete[] _data;
        _size = b._size;
        _data = new T[_size];
        for(int i = 0; i < _size; i ++)
            _data[i] = b._data[i];
        return *this;
    }

    box<T> & operator+=(const T & e)
    {
        add(e);
        return *this;
    }

    box<T> & operator+=(const box<T> & b)
    {
        add(b);
        return *this;
    }

    box<T> operator+(const T & e) const
    {
        box<T> result(*this);
        result += e;
        return result;
    }

    box<T> operator+(const box<T> & b) const
    {
        box<T> result(*this);
        result += b;
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

    friend std::ostream & operator<<(std::ostream & out, const box<T> & b)
    {
        if(b.size() == 0)
        {
            out << "{}";
            return out;
        }
        out << "{ ";
        for(int i = 1; i <= b._size; i++)
        {
            out << b[i];
            out << ", ";
        }
        out << "\b\b }";
        return out;
    }

};

#endif
