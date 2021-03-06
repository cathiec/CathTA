#include "TA.h"

#include <dirent.h>
#include <sys/stat.h>
#include <fstream>

using namespace std;

ofstream data_file;
ofstream max_file;

bool is_file(std::string path)
{
    DIR * d = opendir(path.c_str());
    if(d == NULL)
        return true;
    else
        return false;
}

void test(std::string path, int & max)
{
    if(is_file(path))
    {
        const clock_t begin_time = clock();
        tree_automaton A(path);
        int i;
        for(i = 0; i <= UPPER_BOUND_DIMENSION; i++)
            if(A.is_included_in(A, i))
                break;
        int j = A.max_dimension();
        data_file << path << "  \t";
        data_file << "Time Used: " << float( clock () - begin_time ) /  CLOCKS_PER_SEC << "\t";
        if((i != j) && (i != UPPER_BOUND_DIMENSION + 1))
            data_file << "\t\tERROR: ##### " << i << " " << j << " " << path << endl;
        else
        {
            data_file << "\t\t";
            if(j != -1)
                data_file << j;
        }
        data_file << endl;
        if(j > max)
        {
            max = j;
            max_file.open("max.cdat");
            max_file << max << endl;
            max_file.close();
        }
    }
    else
    {
        DIR * direct = opendir(path.c_str());
        struct dirent * entry;
        std::string unit_name;
        while(entry = readdir(direct))
        {
            if(std::string(entry->d_name) != "." && std::string(entry->d_name) != "..")
            {
                unit_name = path + "/" + std::string(entry->d_name);
                test(unit_name, max);
            }
        }
        closedir(direct);
    }
}

int main(int argc, char **argv)
{
    SHOW_STATE_DIMENSION;
    data_file.open("result.cdat");
    int max = -1;
    test("examples", max);
    data_file.close();
    return 0;
}
