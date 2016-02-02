#include "TA.h"

using namespace std;

int main(int argc, char ** argv)
{
    clock_t start, end;

    /// parse the 1st tree automaton
    cout << "# Start parsing 1st tree automaton..." << endl;
    start = clock();
    basic_tree_automaton TA1(argv[1]);
    end = clock();
    cout << "\t|\t1st tree automaton has been successfully parsed." << endl;
    cout << "\t|\tNumber of States: " << TA1.Q.size() << endl;
    cout << "\t|\tNumber of Transitions: " << TA1.DELTA.size() << endl;
    cout << "\t|\t-> Time used: " << ((double)(end - start)) / ((double)CLOCKS_PER_SEC / 1000) << " ms" << endl;

    /// parse the 2nd tree automaton
    cout << "# Start parsing 2nd tree automaton..." << endl;
    start = clock();
    basic_tree_automaton TA2(argv[2]);
    end = clock();
    cout << "\t|\t2nd tree automaton has been successfully parsed." << endl;
    cout << "\t|\tNumber of States: " << TA2.Q.size() << endl;
    cout << "\t|\tNumber of Transitions: " << TA2.DELTA.size() << endl;
    cout << "\t|\t-> Time used: " << ((double)(end - start)) / ((double)CLOCKS_PER_SEC / 1000) << " ms" << endl;

    /// antichain algorithm
    cout << "# Start antichain algorithm..." << endl;
    start = clock();
    bool result = TA1.is_included_in(TA2);
    end = clock();
    double t_p = ((double)(end - start)) / ((double)CLOCKS_PER_SEC / 1000);
    cout << "\t|\t-> Time used: " << t_p << " ms" << endl;
    cout << "--------------------------------------------------" << endl;
    if(!result)
        cout << "NOT included" << endl;
    else
        cout << "included" << endl;
    cout << "--------------------------------------------------" << endl;

    cout << "Total time used: " << t_p << " ms" << endl;

	return 0;
}
