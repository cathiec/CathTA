#include "TA.h"

using namespace std;

int main(int argc, char ** argv)
{
    basic_tree_automaton TA("A1003");
    cout << "# " << TA.max_dimension() << endl;
	return 0;
}
