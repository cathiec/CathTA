#include "TA.h"

using namespace std;

int main(int argc, char **argv)
{
    const clock_t begin_time = clock();

    try
    {
        tree_automaton A("examples/A2003");
        cout << A.SIGMA.size() << endl;
        cout << A.Q.size() << endl;
        cout << A.F << endl;
        cout << A.DELTA.size() << endl;
    }
    catch(int e)
    {
        cout << "Error: " << e << endl;
    }

    cout << "\tTime Used: " << float( clock () - begin_time ) /  CLOCKS_PER_SEC << endl;

    return 0;
}
