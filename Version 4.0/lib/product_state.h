#ifndef _product_state_h
#define _product_state_h

#include "standard.h"

class product_state
{

variables:

    str left;

    vec<str> right;

functions:

    product_state()
    {}

    product_state(const str & l, const vec<str> & r)
        :left(l), right(r)
    {}

    ~product_state()
    {}

    str to_string() const
    {
        str result = "(";
        result += left;
        result += ",{";
        for(int i = 0; i < right.size(); i++)
        {
            result += right[i];
            result += ',';
        }
        if(right.size() > 0)
            result.erase(result.end() - 1);
        result += "})";
        return result;
    }

    product_state & operator=(const product_state & ps)
    {
        left = ps.left;
        right = ps.right;
        return *this;
    }

    bool operator==(const product_state & ps) const
    {
        if(left != ps.left)
            return false;
        if(right.size() != ps.right.size())
            return false;
        vec<str> cp_right = right, copy_ps_right = ps.right;
        sort_vector(cp_right);
        sort_vector(copy_ps_right);
        for(int i = 0; i < cp_right.size(); i++)
            if(cp_right[i] != copy_ps_right[i])
                return false;
        return true;
    }

};

os & operator<<(os & out, const product_state & ps)
{
    out << ps.to_string();
    return out;
}


#endif
