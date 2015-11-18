#ifndef tree_automaton_h
#define tree_automaton_h

#include "transition.h"
#include "product_state.h"
#include <stdio.h>
#include <stdlib.h>
#include <fstream>

class tree_automaton
{

public:

    std::string _name;              /* name */
    container<symbol> SIGMA;        /* ranked alphabet */
    container<state> Q;             /* finite set of states */
    container<state> F;             /* set of final states */
    container<transition> DELTA;    /* set of transition rules */

public:

    /// default constructor
    tree_automaton()
    {}

    /// constructor by parser
    /* parse a file in order to construct a tree automaton */
    tree_automaton(std::string f)
    {
        std::ifstream file(f);
        if(file.is_open())
        {
            std::string s;
            //symbols
            do
            {
                file >> s;
            }while(s == "\t");
            if(s != "Ops")
                throw 1; ///no symbol
            bool has_name = false;
            int nb_symbols = 0;
            while(file >> s)
            {
                if(s == "\t")
                    continue;
                if(s != "Automaton")
                {
                    int pos_mid = s.find(':');
                    std::string symbol_name = s.substr(0, pos_mid);
                    int symbol_rank = atoi(s.substr(pos_mid + 1).c_str());
                    SIGMA.add(symbol(symbol_name, symbol_rank), true);
                    nb_symbols++;
                }
                else
                {
                    has_name = true;
                    break;
                }
            }
            //automaton name
            if(!has_name)
                    throw 2; ///no automaton name
            do
            {
                file >> s;
            }while(s == "\t");
            _name = s;
            //states
            do
            {
                file >> s;
            }while(s == "\t");
            if(s != "States")
                throw 3; ///no state
            int nb_states = 0;
            bool has_final_states = false;
            while(file >> s)
            {
                if(s == "\t")
                    continue;
                if(s != "Final")
                {
                    int pos_mid = s.find(':');
                    std::string state_name = s.substr(0, pos_mid);
                    int state_rank = atoi(s.substr(pos_mid + 1).c_str());
                    if(state_rank != 0)
                        throw 4; ///state has a rank > 0
                    Q.add(state(state_name), true);
                    nb_states++;
                }
                else
                {
                    do
                    {
                        file >> s;
                    }while(s == "\t");
                    if(s == "States")
                        has_final_states = true;
                    break;
                }
            }
            //final states
            if(!has_final_states)
                throw 5; ///no final state
            int nb_final_states = 0;
            bool has_transitions = false;
            while(file >> s)
            {
                if(s == "\t")
                    continue;
                if(s != "Transitions")
                {
                    state temp(s);
                    F.add(temp, true);
                    nb_final_states++;
                }
                else
                {
                    has_transitions = true;
                    break;
                }
            }
            //transitions
            if(!has_transitions)
                throw 6; ///no transition
            int nb_transitions = 0;
            file.get();
            while(getline(file, s))
            {
                if(s.length() > 0)
                {
                    transition temp(s);
                    DELTA.add(temp, true);
                    nb_transitions++;
                }
            }
        }
        else
        {
            throw 7; ///no such file
        }
    }

};

#endif
