#include "TA.h"
#include <time.h>

int main(int argc, char ** argv)
{
    clock_t start, end;
    basic_tree_automaton A(argv[1]), B(argv[2]);
    start = clock();
    if(A.is_included_in(B, 100))
        std::cout << "YES" << std::endl;
    else
        std::cout << "NO" << std::endl;
    end = clock();
    std::cout << "\t|\t-> Time used: " << ((double)(end - start)) / ((double)CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
}
