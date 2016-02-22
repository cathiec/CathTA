#include "TA.h"

int main(int argc, char ** argv)
{
    basic_tree_automaton A("17.cath"), B("6.cath");
    int x;
    std::cout << "xx" << std::endl;
    std::cin >> x;
    if(A.is_included_in(B, 3))
        std::cout << "TRUE" << std::endl;
    else
        std::cout << "FALSE" << std::endl;
}
