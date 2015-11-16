#ifndef bin_index_h
#define bin_index_h

#include <iostream>

int bin_length(int i)
{
    int result = 1;
    while(i > 1)
    {
        i = i / 2;
        result++;
    }
    return result;
}

class bin_index
{

private:

    int _length;

    int * _p_index;

public:

    bin_index()
    :_length(0), _p_index(NULL)
    {}

    bin_index(int n)
    :_length(bin_length(n))
    {
        int temp;
        _p_index = new int[_length];
        for(int i = _length - 1; i >= 0; i--)
        {
            temp = 1 << i;
            _p_index[i] = n / temp;
            n -= (temp * _p_index[i]);
        }
    }

    ~bin_index()
    {
        delete[] _p_index;
    }

    int operator[](int i) const
    {
        return _p_index[i];
    }

    friend std::ostream & operator<<(std::ostream & out, const bin_index & b)
    {
        for(int i = b._length - 1; i >= 0; i--)
            out << b._p_index[i];
        return out;
    }

};

#endif
