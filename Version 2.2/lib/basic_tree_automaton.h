#ifndef basic_tree_automaton_h
#define basic_tree_automaton_h

#include "basic_transition.h"
#include "basic_product_state.h"
#include "occurrence.h"

#include <stdio.h>
#include <stdlib.h>
#include <fstream>

#define MAX_DIMENSION 100

int global_counter = 0;
int global_add = 0;
int global_del = 0;

class basic_tree_automaton
{

public:

    /* name */
    std::string _name;

    /* ranked alphabet */
    basic_set<symbol> SIGMA;

    /* finite set of states */
    basic_set<basic_state> Q;

    /* finite set of final states */
    basic_set<basic_state> F;

    /* finite set of transition rules */
    basic_set<basic_transition> DELTA;

public:

    /// default constructor
    basic_tree_automaton()
    {}

    /// reference copy constructor
    basic_tree_automaton(const basic_tree_automaton & ta)
    :_name(ta._name), SIGMA(ta.SIGMA), Q(ta.Q), F(ta.F), DELTA(ta.DELTA)
    {}

    /// constructor by parsing a file
    basic_tree_automaton(std::string f)
    {
        std::ifstream file(f);
        if(file.is_open())
        {
            std::string s;
            // symbols (ranked alphabet)
            do
            {
                file >> s;
            }while(s == "\t");
            if(s != "Ops")
                throw 1; /// no symbol
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
            // automaton name
            if(!has_name)
                    throw 2; /// no automaton name
            do
            {
                file >> s;
            }while(s == "\t");
            _name = s;
            // states
            do
            {
                file >> s;
            }while(s == "\t");
            if(s != "States")
                throw 3; /// no state
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
                        throw 4; /// state has a rank > 0
                    Q.add(basic_state(state_name), true);
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
            // final states
            if(!has_final_states)
                throw 5; /// no final state
            bool has_transitions = false;
            while(file >> s)
            {
                if(s == "\t")
                    continue;
                if(s != "Transitions")
                    F.add(basic_state(s), true);
                else
                {
                    has_transitions = true;
                    break;
                }
            }
            // transitions
            if(!has_transitions)
                throw 6; /// no transition
            file.get();
            while(getline(file, s))
                if(s.length() > 0)
                    DELTA.add(basic_transition(s), true);
            file.close();
        }
        else
        {
            throw 7; ///no such file
        }
    }

    /// destructor
    ~basic_tree_automaton()
    {}

    /// save in a file
    void save_in_file(std::string name) const
    {
        std::ofstream file(name);
        if(file.is_open())
        {
            // ranked alphabet
            file << "Ops ";
            for(int i = 1; i <= SIGMA.size(); i++)
            {
                file << SIGMA[i]._name;
                file << ":";
                file << SIGMA[i]._rank;
                file << " ";
            }
            file << std::endl << std::endl;
            // name
            file << "Automaton A" << std::endl << std::endl;
            // finite set of states
            file << "States ";
            for(int i = 1; i <= Q.size(); i++)
            {
                file << Q[i] << " ";
            }
            file << std::endl << std::endl;
            // finite set of final states
            file << "Final States ";
            for(int i = 1; i <= F.size(); i++)
            {
                file << F[i] << " ";
            }
            file << std::endl << std::endl;
            // finite set of transition rules
            file << "Transitions" << std::endl;
            for(int i = 1; i <= DELTA.size(); i++)
            {
                file << to_string(DELTA[i]) << std::endl;
            }
            file.close();
        }
        else
        {
            throw 8; ///cannot create a new file
        }
    }

    /// concatenation
    void concat(basic_tree_automaton B, std::string x)
    {
        // add new transitions
        for(int i = 1; i <= DELTA.size(); i++)
        {
            if(DELTA[i]._alpha._rank == 0)
            {
                for(int j = 1; j <= B.F.size(); j++)
                {
                    basic_transition t = DELTA[i];
                    t._alpha._rank = 1;
                    basic_state s = B.F[j];
                    s._name = B.F[j]._name + "_" + x;
                    t._inputs.add(s);
                    DELTA.add(t);
                    SIGMA.add(t._alpha);
                }
            }
        }
        // rename and add sigma in B
        for(int i = 1; i <= B.SIGMA.size(); i++)
        {
            symbol s = B.SIGMA[i];
            SIGMA.add(s);
        }
        // rename and add states in B
        for(int i = 1; i <= B.Q.size(); i++)
        {
            basic_state s = B.Q[i];
            s._name = B.Q[i]._name + "_" + x;
            Q.add(s);
        }
        // rename transitions in B
        for(int i = 1; i <= B.DELTA.size(); i++)
        {
            basic_transition t = B.DELTA[i];
            for(int j = 1; j <= t._inputs.size(); j++)
                t._inputs[j]._name = B.DELTA[i]._inputs[j]._name + "_" + x;
            t._output._name = B.DELTA[i]._output._name + + "_" + x;
            DELTA.add(t);
        }
    }

    /// new version of inclusion checking
    /* check if a state "s" is accepted */
    bool accept(const basic_state & s) const
    {
        if(F.contain(s))
            return true;
        else
            return false;
    }

    /* check is a macro state "ms" is rejected */
    bool reject(const basic_set<basic_state> & ms) const
    {
        for(int i = 1; i <= ms.size(); i++)
            if(accept(ms[i]))
                return false;
        return true;
    }

    /* check if a product state "ps" is accepted */
    bool accept(const basic_product_state & ps, const basic_tree_automaton & B) const
    {
        if(accept(ps._1) && B.reject(ps._2))
            return true;
        else
            return false;
    }

    /* produce the set of initial states for a given symbol "s" */
    basic_set<basic_state> initial_states(const symbol & s) const
    {
        basic_set<basic_state> result;
        for(int i = 1; i <= DELTA.size(); i++)
            if(DELTA[i]._alpha == s)
                result.add(DELTA[i]._output);
        return result;
    }

    /* compute the set of post image with a given symbol "s" of a given combination of micro states "cb" */
    basic_set<basic_state> post(const symbol & s,
                                const basic_tuple<basic_set<basic_state> > cb)
    const
    {
        basic_set<basic_state> result;
        if(s._rank != cb.size())
            return result;
        for(int i = 1; i <= DELTA.size(); i++)
        {
            basic_transition t = DELTA[i];
            if(t._alpha == s)
            {
                bool ok = true;
                for(int j = 1; j <= s._rank; j++)
                {
                    if(!cb[j].contain(t._inputs[j]))
                    {
                        ok = false;
                        break;
                    }
                }
                if(ok)
                    result.add(t._output);
            }
        }
        return result;
    }

    /* compute possible Pi according to qi */
    basic_set<basic_set<basic_state> > possible_Pi(const basic_state & qi,
                                                   const basic_set<basic_product_state> & Processed)
    const
    {
        basic_set<basic_set<basic_state> > result;
        for(int i = 1; i <= Processed.size(); i++)
        {
            if(Processed[i]._1 == qi)
                result.add(Processed[i]._2);
        }
        return result;
    }

    /* compute all the possible combinations of P1 to Pn */
    basic_set<basic_tuple<basic_set<basic_state> > > possible_combinations_Pi(const basic_tuple<basic_state> & cb_qi,
                                                                              const basic_set<basic_product_state> & Processed)
    const
    {
        basic_set<basic_tuple<basic_set<basic_state> > > result;
        if(cb_qi.size() > 1)
        {
            basic_tuple<basic_state> next_recursion = cb_qi;
            next_recursion._size_repre--;
            basic_state qn = cb_qi[cb_qi.size()];
            basic_set<basic_set<basic_state> > possible_Pn = possible_Pi(qn, Processed);
            basic_set<basic_tuple<basic_set<basic_state> > > possible_combinations_P1_to_Pn_minus_1 = possible_combinations_Pi(next_recursion, Processed);
            for(int i = 1; i <= possible_combinations_P1_to_Pn_minus_1.size(); i++)
            {
                for(int j = 1; j <= possible_Pn.size(); j++)
                {
                    basic_tuple<basic_set<basic_state> > cb_Pi = possible_combinations_P1_to_Pn_minus_1[i];
                    basic_set<basic_state> Pn = possible_Pn[j];
                    cb_Pi.add(Pn);
                    result.add(cb_Pi);
                }
            }
        }
        else
        {
            basic_state q1 = cb_qi[1];
            basic_set<basic_set<basic_state> > possible_P1 = possible_Pi(q1, Processed);
            for(int i = 1; i <= possible_P1.size(); i++)
            {
                basic_tuple<basic_set<basic_state> > cb_P1;
                basic_set<basic_state> P1 = possible_P1[i];
                cb_P1.add(P1);
                result.add(cb_P1);
            }
        }
        return result;
    }

    /* compute the set of post image of a given set of product states "Processed" w.r.t. a given product state "rR" */
    /* this function does not work for dimension 0 */
    basic_set<basic_product_state> post(const basic_product_state & rR,
                                        basic_set<basic_product_state> * Processed,
                                        int d,
                                        const basic_tree_automaton & B)
    const
    {
        basic_state r = rR._1;
        basic_set<basic_state> R = rR._2;
        basic_set<basic_product_state> result;
        basic_set<basic_state> q_Processed;
        basic_set<basic_product_state> safe_processed;
        safe_processed.add(rR);
        for(int i = 0; i < d; i++)
            safe_processed.add(Processed[i]);
        for(int i = 1; i <= safe_processed.size(); i++)
            q_Processed.add(safe_processed[i]._1);
        for(int i = 1; i <= DELTA.size(); i++)
        {
            basic_transition t = DELTA[i];
            basic_set<int> occurrences_r = occurrences(t._inputs, r);
            if(occurrences_r.size() == 1 && q_Processed.contain(t._inputs))
            {
                basic_set<basic_tuple<basic_set<basic_state> > > possible_P1_to_Pn = possible_combinations_Pi(t._inputs, safe_processed);
                for(int j = 1; j <= possible_P1_to_Pn.size(); j++)
                {
                    basic_set<int> occurrences_R = occurrences(possible_P1_to_Pn[j], R);
                    if(occurrences_have_same_index(occurrences_r, occurrences_R))
                        result.add(basic_product_state(t._output, B.post(t._alpha, possible_P1_to_Pn[j])));
                }
            }
        }
        return result;
    }

    /* optimization */
    int optimize(const basic_state & p,
                  const basic_set<basic_state> & P,
                  int d,
                  basic_set<basic_product_state> * Processed,
                  basic_set<basic_product_state> & Next)
    const
    {
        bool exist = false;

        for(int j = 0; j < d; j++)
        {
            int l = Processed[j].size();
            for(int k = 1; k <= l; k++)
            {
                if(p == Processed[j][k]._1 && P.contain(Processed[j][k]._2))
                {
                    exist = true;
                    Processed[d].add(Processed[j][k]);
                    Processed[j].del(Processed[j][k]);
                    break;
                }
            }
        }

        for(int j = 1; j <= Processed[d].size(); j++)
        {
            basic_state q = Processed[d][j]._1;
            basic_set<basic_state> Q = Processed[d][j]._2;
            if(p == q && P.contain(Q))
            {
                exist = true;
                break;
            }
        }
        if(exist)
            return 0;
        for(int j = 1; j <= Next.size(); j++)
        {
            basic_state q = Next[j]._1;
            basic_set<basic_state> Q = Next[j]._2;
            if(p == q && P.contain(Q))
            {
                exist = true;
                break;
            }
        }
        if(exist)
            return 0;
        for(int j = 0; j < d; j++)
        {
            int l = Processed[j].size();
            for(int k = 1; k <= l; k++)
            {
                if(p == Processed[j][k]._1 && Processed[j][k]._2.contain(P))
                {
                    //std::cout << "DEL from " << j << ": " << Processed[j][k] <<std::endl;
                    Processed[j].del(Processed[j][k]);
                    k--;
                    l--;
                    global_del++;
                    global_counter--;
                }
            }
        }
        int k = Next.size();
        for(int j = 1; j <= k; j++)
        {
            basic_state q = Next[j]._1;
            basic_set<basic_state> Q = Next[j]._2;
            if(q == p && Q.contain(P))
            {
                //std::cout << "DEL: " << Next[j] <<std::endl;
                Next.del(Next[j]); 
                j--;
                k--;
            }
        }
        return 1;
    }

    /* check inclusion by giving a dimension */
    bool is_included_in(const basic_tree_automaton & B, int d, basic_set<basic_product_state> * Processed) const
    {
        // initialization
        basic_set<basic_product_state> Next;
        /**
         **  dimension 0
         **/
        if(d == 0)
        {
            for(int i = 1; i <= DELTA.size(); i++)
                // check all the transition rules with 0 argument
                if(DELTA[i]._alpha._rank == 0)
                {
                    basic_product_state temp(DELTA[i]._output, B.initial_states(DELTA[i]._alpha));
                    // if we find an accepting product state, return false
                    if(accept(temp, B))
                        return false;
                    // else we add it into next as an initial state
                    Next.add(temp);
                }
        }
        /**
         **  dimension > 0
         **/
        else if(Processed[d - 1].size() > 0)
        {
            basic_set<basic_state> q_Processed;
            for(int i = 1; i <= Processed[d - 1].size(); i++)
                q_Processed.add(Processed[d - 1][i]._1);
            basic_set<basic_set<basic_state> > Q_Processed;
            for(int i = 1; i <= Processed[d - 1].size(); i++)
                Q_Processed.add(Processed[d - 1][i]._2);
            for(int i = 1; i <= DELTA.size(); i++)
                // check all the transition rules with 2 or more arguments
                if(DELTA[i]._alpha._rank >= 2)
                {
                    int in_Processed_d_minus_1 = 0;
                    bool at_least_2_in_Processed_d_minus_1 = false;
                    for(int j = 1; j <= DELTA[i]._inputs.size(); j++)
                    {
                        if(q_Processed.contain(DELTA[i]._inputs[j]))
                        {
                            in_Processed_d_minus_1++;
                            if(in_Processed_d_minus_1 == 2)
                                at_least_2_in_Processed_d_minus_1 = true;
                        }
                        if(at_least_2_in_Processed_d_minus_1 == true)
                            break;
                    }
                    if(at_least_2_in_Processed_d_minus_1)
                    {
                        //std::cout << d << DELTA[i] << std::endl;
                        basic_set<basic_product_state> safe_processed;
                        for(int j = 0; j < d; j++)
                            safe_processed.add(Processed[j]);
                        basic_set<basic_tuple<basic_set<basic_state> > > possible_P1_to_Pn = possible_combinations_Pi(DELTA[i]._inputs, safe_processed);
                        for(int j = 1; j <= possible_P1_to_Pn.size(); j++)
                        {
                            in_Processed_d_minus_1 = 0;
                            at_least_2_in_Processed_d_minus_1 = false;
                            for(int k = 1; k <= possible_P1_to_Pn[j].size(); k++)
                            {
                                if(Q_Processed.contain(possible_P1_to_Pn[j][k]))
                                {
                                    in_Processed_d_minus_1++;
                                    if(in_Processed_d_minus_1 == 2)
                                        at_least_2_in_Processed_d_minus_1 = true;
                                }
                                if(at_least_2_in_Processed_d_minus_1 == true)
                                    break;
                            }
                            if(at_least_2_in_Processed_d_minus_1 == true)
                            {
                                basic_product_state temp(DELTA[i]._output, B.post(DELTA[i]._alpha, possible_P1_to_Pn[j]));
                                if(accept(temp, B))
                                    return false;
                                //if(optimize(temp._1, temp._2, d, Processed, Next) != 0)
                                //optimize(temp._1, temp._2, d, Processed, Next);
                                Next.add(temp);
                            }
                        }
                    }
                }
        }
        // start computing next
        while(Next.size() > 0)
        {
            //std::cout << d << " NEXT " << Next << std::endl;
            // pick a product state (r,R) from next
            basic_product_state rR = Next[1];
            Next.del(rR);
            // add (r,R) into processed of current dimension
            Processed[d].add(rR);
            global_counter++;
            global_add++;
            // compute the post image of rR
            /**
             **  dimension 0
             **/
            if(d == 0)
            {
                for(int i = 1; i <= DELTA.size(); i++)
                {
                    // if the transition rule has more than one argument, we ignore this transition rule
                    basic_transition t = DELTA[i];
                    if(t._alpha._rank != 1)
                        continue;
                    // else if the only argument is the left part of rR
                    if(t._inputs.contain(rR._1))
                    {
                        // then the left part of the post image pP is the output of the transition rule
                        basic_state p = t._output;
                        // compute the right part of the post image pP
                        basic_set<basic_state> P;
                        for(int j = 1; j <= rR._2.size(); j++)
                        {
                            // pick a state Ri from the macro state R
                            basic_state Ri = rR._2[j];
                            for(int k = 1; k <= B.DELTA.size(); k++)
                                // if there's a same transition rule in B whose input is Ri
                                if(B.DELTA[k]._alpha == t._alpha && B.DELTA[k]._inputs.contain(Ri))
                                    // we add it into P
                                    P.add(B.DELTA[k]._output);
                        }
                        // if (p, P) is an accepting product state, return false
                        if(accept(basic_product_state(p, P), B))
                            return false;
                        // optimization
                        if(optimize(p, P, d, Processed, Next) != 0)
                            Next.add(basic_product_state(p, P));
                    }
                }
            }
            /**
             **  dimension > 0
             **/
            else
            {
                basic_set<basic_product_state> rR_post = post(rR, Processed, d, B);
                for(int i = 1; i <= rR_post.size(); i++)
                {
                    basic_product_state pP = rR_post[i];
                    basic_state p = pP._1;
                    basic_set<basic_state> P = pP._2;
                    // if (p, P) is an accepting product state, return false
                    if(accept(pP, B))
                        return false;
                    // optimization
                    if(optimize(p, P, d, Processed, Next) != 0)
                        Next.add(pP);
                }
            }
        }
        return true;
    }

    /* check inclusion by giving a maximum dimension */
    bool is_included_in(const basic_tree_automaton & B, int b) const
    {
        basic_set<basic_product_state> Processed[MAX_DIMENSION + 1];
        for(int i = 0; i <= b; i++)
        {
            //std::cout << i << ":" << std::endl;
            if(is_included_in(B, i, Processed) == false)
            {
                std::cout << global_counter << std::endl;
                std::cout << global_add << std::endl;
                std::cout << global_del << std::endl;
                return false;
            }
            //std::cout << Processed[i] << std::endl;
        }
        std::cout << global_counter << std::endl;
        std::cout << global_add << std::endl;
        std::cout << global_del << std::endl;
        return true;
    }

};

/// basic_tree_automaton -> std::string
std::string to_string(const basic_tree_automaton & ta)
{
    std::string result = "";
    int i;
    result += "Automaton [";
    result += ta._name;
    result += "]\n";
    result += "--- RANKED ALPHABET ---\n";
    for(i = 1; i <= ta.SIGMA.size(); i++)
    {
        result += "\t";
        result += to_string(ta.SIGMA[i]);
        result += "\n";
    }
    result += "--- STATES ---\n";
    for(i = 1; i <= ta.Q.size(); i++)
    {
        result += "\t";
        result += to_string(ta.Q[i]);
        result += "\n";
    }
    result += "--- FINAL STATES ---\n";
    for(i = 1; i <= ta.F.size(); i++)
    {
        result += "\t";
        result += to_string(ta.F[i]);
        result += "\n";
    }
    result += "--- TRANSITIONS ---\n";
    for(i = 1; i <= ta.DELTA.size(); i++)
    {
        result += "\t";
        result += to_string(ta.DELTA[i]);
        result += "\n";
    }
    return result;
}

/// basic_tree_automaton -> std::ostream
std::ostream & operator<<(std::ostream & out, const basic_tree_automaton & ta)
{
    out << to_string(ta);
    return out;
}

#endif
