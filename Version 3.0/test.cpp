#include "TA.h"

using namespace std;

int main(int argc, char ** argv)
{
    tree_automaton t;
    t.load_from_file(argv[1]);
    //cout << t << endl;
    t.save_into_file("BBB");
    //cout << t.get_DELTA()[t.number_of_transitions() - 5] << endl;

}
