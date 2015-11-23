#include "TA.h"

using namespace std;

int main(int argc, char **argv)
{
    SHOW_DIMENSION;

    const clock_t begin_time = clock();

    try
    {
        tree_automaton A1("xx");
        //cout << A1.DELTA << endl;
        cout << "max dimension:\t" << A1.upper_bound_dimension();
    }
    catch(int e)
    {
        cout << "Error: " << e << endl;
    }

    cout << "\tTime Used: " << float( clock () - begin_time ) /  CLOCKS_PER_SEC << endl;

/*
    try
    {
        clock_t begin_time = clock();
        clock_t time_used;

        tree_automaton A1("examples/A0006");

        time_used = clock() - begin_time;

        cout << endl;

        cout << A1._name << " has been read successfully." << endl;
        cout << "Number of States:\t" << A1.Q._size << endl;
        cout << "Number of Transitions:\t" << A1.DELTA._size << endl;

        cout << "\tTime Used for Reading the File:\t\t" << float(time_used) /  CLOCKS_PER_SEC << " sec" << endl;

        cout << endl;

        begin_time = clock();

        bool included = A1.is_included_in(A1);

        time_used = clock() - begin_time;

        if(included)
            cout << "L(" << A1._name << ") is included in L(" << A1._name << ")." << endl;
        else
            cout << "NO" << endl;

        cout << "\tTime Used for Inclusion Checking:\t" << float(time_used) /  CLOCKS_PER_SEC << " sec " << endl;
        cout << A1.upper_bound_dimension() << endl;
    }
    catch(int e)
    {
        cout << "Error: " << e << endl;
    }
    */

    return 0;
}
