#include <fstream>
#include <iostream>

using namespace std;

int main(int argc, char ** argv)
{
    ofstream f("out.cath");
    for(int i = 1; i <= atoi(argv[1]) - 1; i++)
        f << "f(q0,q" << i << ") -> q" << i + 1 << endl;
    f.close();
}
