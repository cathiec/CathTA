#include "TA.h"
#include <time.h>

using namespace std;

int main(int argc, char ** argv)
{
    clock_t start, end;
    start = clock();
    tree_automaton TA1(argv[1]), TA2(argv[2]);
    if(TA1.is_included_in(TA2))
        cout << "YES" << endl;
    else
        cout << "NO" << endl;
    end = clock();
    std::cout << "\t|\t-> Time used: " << ((double)(end - start)) / ((double)CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
}
