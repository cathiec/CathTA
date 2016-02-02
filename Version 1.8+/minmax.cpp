#include <dirent.h>
#include <sys/stat.h>
#include <iostream>
#include "TA.h"

using namespace std;

int main(int argc, char ** argv)
{
    basic_tree_automaton TA(argv[1]);
    cout << TA.max_dimension(3) << endl;
    cout << TA.dt << endl;
}
