#include "TA.h"

using namespace std;

int main(int argc, char **argv)
{
    const clock_t begin_time = clock();

    try
    {
        tree_automaton A("A501");
		tree_automaton B("A501");
		if(A.is_included_in(B))
			cout << "YES" << endl;
		else
			cout << "NO" << endl;
        //cout << file << "\tmax dimension:\t" << A.max_dimension() << endl;
    }
    catch(int e)
    {
        cout << "Error: " << e << endl;
    }

    cout << "\tTime Used: " << float( clock () - begin_time ) /  CLOCKS_PER_SEC << endl;

    return 0;
}
