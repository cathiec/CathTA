#include "TA.h"

using namespace std;

int main(int argc, char **argv)
{
    ofstream file("result.cath", ios::app);
    const clock_t begin_time = clock();

    try
    {
        tree_automaton A1(argv[1]);
        file << argv[1] << " max dimension:\t" << A1.upper_bound_dimension();
    }
    catch(int e)
    {
        file << "Error: " << e << endl;
    }

    file << "\tTime Used: " << float( clock () - begin_time ) /  CLOCKS_PER_SEC << endl;
    file.close();


    return 0;
}
