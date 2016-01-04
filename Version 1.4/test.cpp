#include "TA.h"

using namespace std;

int main(int argc, char ** argv)
{
    basic_tree_automaton TA1("A2"), TA2("A1");
    if(TA1.is_included_in(TA2))
        cout << "1" << endl;
    else
        cout << "0" << endl;
	return 0;
}
