#include "TA.h"

int main(int argc, char ** argv)
{
    basic_tree_automaton A(argv[1]);
    std::cout << A.max_dimension(2) << std::endl;
    clock_t start, end;
    start = clock();
    timbuk(A.transform_into_words_automaton(2)).save_in_file("test_result.cath");
    end = clock();
    std::cout << "\t|\t-> Time used: " << ((double)(end - start)) / ((double)CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
}
