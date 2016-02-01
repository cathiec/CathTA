#include "TA.h"

int main(int argc, char ** argv)
{
    basic_tree_automaton A(argv[1]);
    std::cout << A.max_dimension(2) << std::endl;
    std::cout << A.dt << std::endl;
}
