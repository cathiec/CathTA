#include <dirent.h>
#include <sys/stat.h>
#include <iostream>
#include "TA.h"

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
        std::string cmd = "./HKC Ax " + path;
        system(cmd.c_str());
        cmd = "./AC Ax " + path;
        system(cmd.c_str());
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
    dim(argv[1]);
}
