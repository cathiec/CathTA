#include "lib/cathta"

int main(int argc, char ** argv)
{
    tree_automaton TA(argv[1]);
    for(int i = 0; i < TA.I.size(); i++)
        println(TA.I[i]);
    return 0;
}
