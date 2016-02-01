#include <dirent.h>
#include <sys/stat.h>
#include <iostream>
#include "TA.h"

std::ofstream file;

bool is_file(std::string path)
{
    DIR * d = opendir(path.c_str());
    if(d == NULL)
        return true;
    else
        return false;
}

void dim(std::string path)
{
    if(is_file(path))
    {
        basic_tree_automaton TA(path);
        int x = TA.max_dimension();

        if(x != -1)
            file << x;
        file << "\t" << path;
        file << std::endl;
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
                dim(unit_name);
            }
        }
        closedir(direct);
    }
}

int main(int argc, char ** argv)
{
    file.open("result.cath", std::ios::app);
    dim(argv[1]);
    file.close();
}
