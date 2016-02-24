#include "TA.h"

int main(int argc, char ** argv)
{
    basic_tree_automaton A(argv[1]), B(argv[2]);
    if(A.is_included_in(B))
        std::cout << "YES" << std::endl;
    else
        std::cout << "NO" << std::endl;
}
