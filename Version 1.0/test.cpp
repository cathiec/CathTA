#include "TA.h"
#include <ctime>

using namespace std;

int main(int argc, char **argv)
{
    ofstream file("result.cath", ios::app);
    const clock_t begin_time = clock();

    try
    {


        tree_automaton A1(argv[1]);
        //tree_automaton A2(argv[2]);
        //tree_automaton A1UA2 = A1.U(A2);
        file << argv[1] << " max dimension:\t" << A1.max_dimension();
        /*if(A1.inclusion_check(A1UA2))
            cout << "TRUE" << endl;
        else
            cout << "FALSE" << endl;*/

    }
    catch(int e)
    {
        file << "Error: " << e << endl;
    }

    file << "\tTime Used: " << float( clock () - begin_time ) /  CLOCKS_PER_SEC << endl;
    file.close();

    return 0;
}
