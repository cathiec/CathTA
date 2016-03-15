#include "TA.h"
#include <time.h>

int main(int argc, char ** argv)
{
    clock_t start, end;
    start = clock();
    tree_automaton A(argv[1]);
    A.save_into_file("result");
    end = clock();
    std::cout << "\t|\t-> Time used: " << ((double)(end - start)) / ((double)CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
}
