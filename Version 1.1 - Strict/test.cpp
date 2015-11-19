#include "TA.h"

using namespace std;

int main(int argc, char **argv)
{
    const clock_t begin_time = clock();

    try
    {
        tree_automaton A1("examples/A0246");
        cout << 1 << endl;
        /*tree_automaton A2("examples/A0177");
        cout << 2 << endl;*/
        if(A1.is_included_in(A1, 5))
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