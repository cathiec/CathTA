#include "TA.h"

using namespace std;

int main(int argc, char ** argv)
{
    clock_t start, end;
    double total_t;

    /// parse the 1st tree automaton
    cout << "# Start parsing 1st tree automaton..." << endl;
    start = clock();
    basic_tree_automaton TA1(argv[1]);
    end = clock();
    cout << "\t|\t1st tree automaton has been successfully parsed." << endl;
    cout << "\t|\tNumber of States: " << TA1.Q.size() << endl;
    cout << "\t|\tNumber of Transitions: " << TA1.DELTA.size() << endl;
    cout << "\t|\t-> Time used: " << ((double)(end - start)) / ((double)CLOCKS_PER_SEC / 1000) << " ms" << endl;

    /// parse the 2nd tree automaton
    cout << "# Start parsing 2nd tree automaton..." << endl;
    start = clock();
    basic_tree_automaton TA2(argv[2]);
    end = clock();
    cout << "\t|\t2nd tree automaton has been successfully parsed." << endl;
    cout << "\t|\tNumber of States: " << TA2.Q.size() << endl;
    cout << "\t|\tNumber of Transitions: " << TA2.DELTA.size() << endl;
    cout << "\t|\t-> Time used: " << ((double)(end - start)) / ((double)CLOCKS_PER_SEC / 1000) << " ms" << endl;

    /// compute the maximum dimension of 1st tree automaton
    cout << "# Start computing the maximum dimension of 1st tree automaton..." << endl;
    start = clock();
    int max_dim_1 = TA1.max_dimension();
    end = clock();
    double t_p1 = ((double)(end - start)) / ((double)CLOCKS_PER_SEC / 1000);
    cout << "\t|\tThe miximum dimension of 1st tree automaton is: ";
    if(max_dim_1 == -1)
        cout << "INFINITE" << endl;
    else
        cout << max_dim_1 << endl;
    cout << "\t|\t-> Time used: " << t_p1 << " ms" << endl;

    /// compute the maximum dimension of 2nd tree automaton
    cout << "# Start computing the maximum dimension of 2nd tree automaton..." << endl;
    start = clock();
    int max_dim_2 = TA2.max_dimension();
    end = clock();
    double t_p2 = ((double)(end - start)) / ((double)CLOCKS_PER_SEC / 1000);
    cout << "\t|\tThe miximum dimension of 2nd tree automaton is: ";
    if(max_dim_2 == -1)
        cout << "INFINITE" << endl;
    else
        cout << max_dim_2 << endl;
    cout << "\t|\t-> Time used: " << t_p2 << " ms" << endl;

    /// compare the 2 maximum dimensions
    if((max_dim_2 != -1 && max_dim_1 > max_dim_2) || (max_dim_1 == -1 && max_dim_2 != -1))
    {
        cout << "--------------------------------------------------" << endl;
        cout << "NOT included" << endl;
        cout << "--------------------------------------------------" << endl;
        total_t = t_p1 + t_p2;
    }
    else if(max_dim_1 == -1)
    {
        /// antichain algorithm
        cout << "# Start antichain algorithm..." << endl;
        start = clock();
        bool result = TA1.is_included_in(TA2);
        end = clock();
        double t_p3 = ((double)(end - start)) / ((double)CLOCKS_PER_SEC / 1000);
        cout << "\t|\t-> Time used: " << t_p3 << " ms" << endl;
        cout << "--------------------------------------------------" << endl;
        if(!result)
            cout << "NOT included" << endl;
        else
            cout << "included" << endl;
        cout << "--------------------------------------------------" << endl;
        total_t = t_p1 + t_p2 + t_p3;
    }
    else
    {
        /// call HKC
        cout << "# Start transforming 1st tree automaton into words automaton..." << endl;
        start = clock();
        timbuk(TA1.transform_into_words_automaton(max_dim_1)).save_in_file("words_automaton_1");
        end = clock();
        double t_p3 = ((double)(end - start)) / ((double)CLOCKS_PER_SEC / 1000);
        cout << "\t|\tThe 1st automaton has been successfully transformed into a words automaton."<< endl;
        cout << "\t|\tThe transformed automaton has been saved in the file \"words_automaton_1\"." << endl;
        cout << "\t|\t-> Time used: " << t_p3 << " ms" << endl;
        cout << "# Start transforming 2nd tree automaton into words automaton..." << endl;
        start = clock();
        timbuk(TA2.transform_into_words_automaton(max_dim_1)).save_in_file("words_automaton_2");
        end = clock();
        double t_p4 = ((double)(end - start)) / ((double)CLOCKS_PER_SEC / 1000);
        cout << "\t|\tThe 2nd automaton has been successfully transformed into a words automaton."<< endl;
        cout << "\t|\tThe transformed automaton has been saved in the file \"words_automaton_2\"." << endl;
        cout << "\t|\t-> Time used: " << t_p4 << " ms" << endl;
        cout << "# Start calling HKC..." << endl;
        cout << "--------------------------------------------------" << endl;
        start = clock();
        system("lib/hkc -incl words_automaton_1 words_automaton_2");
        end = clock();
        double t_p5 = ((double)(end - start)) / ((double)CLOCKS_PER_SEC / 1000);
        cout << "\t|\t-> Time used: " << t_p5 << " ms" << endl;
        cout << "--------------------------------------------------" << endl;
        //total_t = t_p1 + t_p2 + t_p3 + t_p4 + t_p5;
        total_t = t_p1 + t_p2 + t_p5;
    }

    cout << "Total time used: " << total_t << endl;

	return 0;
}
