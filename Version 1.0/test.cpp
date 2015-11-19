#include "TA.h"
#include <ctime>

using namespace std;

int main(int argc, char **argv)
{
    /*
    ofstream file("result.cath", ios::app);
    const clock_t begin_time = clock();

    try
    {
        tree_automaton A1(argv[1]);
        file << argv[1] << " max dimension:\t" << A1.max_dimension();
    }
    catch(int e)
    {
        file << "Error: " << e << endl;
    }

    file << "\tTime Used: " << float( clock () - begin_time ) /  CLOCKS_PER_SEC << endl;
    file.close();
    */

    const clock_t begin_time = clock();

    try
    {
        tree_automaton A1("examples/A0053");
        cout << 1 << endl;
        tree_automaton A2("examples/A0054");
        cout << 2 << endl;
        tree_automaton A1UA2 = A1.U(A2);
        cout << 3 << endl;
        tree_automaton A2IA1 = A2.I(A1);
        cout << 4 << endl;
        cout << A2IA1.DELTA.size() << endl;
        if(A2IA1.inclusion_check(A1))
            cout << "YES" << endl;
        else
            cout << "NO" << endl;
    }
    catch(int e)
    {
        cout << "Error: " << e << endl;
    }

    cout << "\tTime Used: " << float( clock () - begin_time ) /  CLOCKS_PER_SEC << endl;

    return 0;
}
