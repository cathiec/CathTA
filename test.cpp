#include "TA.h"

using namespace std;

int main(int argc, char **argv)
{
    try
    {
        tree_automaton A1(argv[1]);
        cout << "#" << endl;
        //tree_automaton A2(argv[2]);
        //tree_automaton A1UA2 = A1.U(A2);
        cout << argv[1] << " max dimension:\t" << A1.max_dimension() << endl;
        /*if(A1.inclusion_check(A1))
            cout << "TRUE" << endl;
        else
            cout << "FALSE" << endl;*/

    }
    catch(int e)
    {
        cout << "Error: " << e << endl;
    }


    return 0;
}
