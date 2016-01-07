#include "TA.h"

using namespace std;

int main(int argc, char ** argv)
{
    basic_tree_automaton TA("TA1");
    //cout << TA.max_dimension() << endl;
    cout << TA.transform_into_words_automaton(5) << endl;
	return 0;
}
