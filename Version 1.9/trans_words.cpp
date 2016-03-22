#include "TA.h"
#include <time.h>

int main(int argc, char ** argv)
{
    basic_tree_automaton A(argv[1]);
    timbuk(A.transform_into_words_automaton(atoi(argv[2]))).save_in_file("result.cath");
}
