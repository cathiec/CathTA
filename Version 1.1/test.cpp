#include "TA.h"

using namespace std;

int main(int argc, char **argv)
{
    HIDE_DIMENSION;

    dim d;
    d.add(0);
    d.add(1);
    d.add(2);

    container<int> c, c2, c3;

    c.set_dimension(d);
    c.add(7);

    c3.add(9);

    container<container<int> > x;
    x.add(c);
    x.add(c2);
    c.add(c3);
    x.add(c);
    cout << x << endl;

    return 0;
}
