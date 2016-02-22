#include "TA.h"

int main(int argc, char ** argv)
{
    basic_tree_automaton A("17.cath"), B("6.cath");
    //std::cout << "Dimension of 7.cath is " << A.max_dimension() << std::endl;
    //std::cout << "Dimension of 8.cath is " << B.max_dimension() << std::endl;
    if(A.is_included_in(B))
        std::cout << "TRUE" << std::endl;
    else
        std::cout << "FALSE" << std::endl;
}
