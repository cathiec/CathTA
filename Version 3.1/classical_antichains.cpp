#include "TA.h"
#include <time.h>

using namespace std;

int main(int argc, char ** argv)
{
    clock_t start, end;
    tree_automaton TA1(argv[1]), TA2(argv[2]);
    start = clock();
    bool result = TA1.is_included_in_classical_antichains(TA2);
    end = clock();
    if(result)
        cout << "YES" << endl;
    else
        cout << "NO" << endl;
    std::cout << "\t|\t-> Time used: " << ((double)(end - start)) / ((double)CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
}
