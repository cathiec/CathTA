#include "lib/cathta"

int main(int argc, char ** argv)
{
    cath_timer::time_point start, end;
    tree_automaton TA1(argv[1]), TA2(argv[2]);
    start = cath_timer::now();
    bool result = TA1.classical_antichains(TA2);
    end = cath_timer::now();
    if(result)
        println("YES");
    else
        println("NO");
    double time_sec = cath_timer::time_diff_sec(start, end);
    printf("\t|\t-> Time used: %.3lf sec\n", time_sec);
    double time_msec = cath_timer::time_diff_msec(start, end);
    printf("\t|\t-> Time used: %.3lf msec\n", time_msec);
    unsigned long time_usec = cath_timer::time_diff_usec(start, end);
    printf("\t|\t-> Time used: %ld usec\n", time_usec);
    return 0;
}
