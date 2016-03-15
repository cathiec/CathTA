#include "lib/cathta"

int main(int argc, char ** argv)
{
    cath_timer::time_point start, end;
    str command = "./lib/vata incl ";
    command += argv[1];
    command += ' ';
    command += argv[2];
    start = cath_timer::now();
    system(command.c_str());
    end = cath_timer::now();
    double time_sec = cath_timer::time_diff_sec(start, end);
    printf("\t|\t-> Time used: %.3lf sec\n", time_sec);
    double time_msec = cath_timer::time_diff_msec(start, end);
    printf("\t|\t-> Time used: %.3lf msec\n", time_msec);
    unsigned long time_usec = cath_timer::time_diff_usec(start, end);
    printf("\t|\t-> Time used: %ld usec\n", time_usec);
    return 0;
}
