#include "TA.h"

int main(int argc, char ** argv)
{
    basic_tree_automaton A(argv[1]), B(argv[2]);
    A.concat(B, argv[3]);
    A.save_in_file("xxx");
}
