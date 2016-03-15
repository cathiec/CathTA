#include "TA.h"
#include <time.h>

int main(int argc, char ** argv)
{
    clock_t start, end;
    start = clock();
    std::string command = "lib/hkc -incl ";
    command += argv[1];
    command += ' ';
    command += argv[2];
    system(command.c_str());
    end = clock();
    std::cout << "\t|\t-> Time used: " << ((double)(end - start)) / ((double)CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
}
