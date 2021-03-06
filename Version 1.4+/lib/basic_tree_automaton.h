#ifndef basic_tree_automaton_h
#define basic_tree_automaton_h

#include "basic_transition.h"
#include "basic_product_state.h"
#include "occurrence.h"
#include "state_with_dimension.h"
#include <stdio.h>
#include <stdlib.h>
#include <fstream>


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

    /// union
    basic_tree_automaton rename() const
    {
        basic_tree_automaton result = *this;
        result._name += "'";
        for(int i = 1; i <= Q.size(); i++)
            result.Q[i]._name += "'";
        for(int i = 1; i <= F.size(); i++)
            result.F[i]._name += "'";
        for(int i = 1; i <= DELTA.size(); i++)
        {
            for(int j = 1; j <= DELTA[i]._inputs.size(); j++)
                result.DELTA[i]._inputs[j]._name += "'";
            result.DELTA[i]._output._name += "'";
        }
        return result;
    }

    basic_tree_automaton U(const basic_tree_automaton & B) const
    {
        basic_tree_automaton result;
        result._name = "Union of ";
        result._name += _name;
        result._name += " and ";
        result._name += B._name;
        result.SIGMA = SIGMA;
        basic_tree_automaton _B = B.rename();
        result.Q = Q;
        result.Q.add(_B.Q);
        result.F = F;
        result.F.add(_B.F);
        result.DELTA = DELTA;
        result.DELTA.add(_B.DELTA);
        return result;
    }

    /// intersection
    basic_tree_automaton I(const basic_tree_automaton & B) const
    {
        basic_tree_automaton result;
        result._name = "Union of ";
        result._name += _name;
        result._name += " and ";
        result._name += B._name;
        result.SIGMA = SIGMA;
        result.Q = Q * B.Q;
        result.F = F * B.F;
        result.DELTA = cross_product_transitions(DELTA, B.DELTA);
        return result;
    }

    /// inclusion check
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
    basic_set<basic_state> post(const symbol & s, const basic_tuple<basic_set<basic_state> > cb) const
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
    basic_set<basic_set<basic_state> > possible_Pi(const basic_state & qi, const basic_set<basic_product_state> & Processed) const
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
    basic_set<basic_tuple<basic_set<basic_state> > > possible_combinations_Pi(const basic_tuple<basic_state> & cb_qi, const basic_set<basic_product_state> & Processed) const
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
    basic_set<basic_product_state> post(const basic_product_state & rR, const basic_set<basic_product_state> & Processed, const basic_tree_automaton & B) const
    {
        basic_state r = rR._1;
        basic_set<basic_state> R = rR._2;
        basic_set<basic_product_state> result;
        basic_set<basic_state> q_Processed;
        for(int i = 1; i <= Processed.size(); i++)
            q_Processed.add(Processed[i]._1);
        for(int i = 1; i <= DELTA.size(); i++)
        {
            basic_transition t = DELTA[i];
            basic_set<int> occurrences_r = occurrences(t._inputs, r);
            if(occurrences_r.size() > 0 && q_Processed.contain(t._inputs))
            {
                basic_set<basic_tuple<basic_set<basic_state> > > possible_P1_to_Pn = possible_combinations_Pi(t._inputs, Processed);
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

    /* check if the language of the current tree automaton is included in the language of a tree automaton "B" */
    bool is_included_in(const basic_tree_automaton & B) const
    {
        basic_set<basic_product_state> Processed;
        basic_set<basic_product_state> Next;
        for(int i = 1; i <= DELTA.size(); i++)
            if(DELTA[i]._alpha._rank == 0)
            {
                basic_product_state temp(DELTA[i]._output, B.initial_states(DELTA[i]._alpha));
                if(accept(temp, B))
                    return false;
                Next.add(temp);
            }
        while(Next.size() > 0)
        {
            basic_product_state rR = Next[1];
            Next.del(rR);
            Processed.add(rR);
            basic_set<basic_product_state> rR_post = post(rR, Processed, B);
            for(int i = 1; i <= rR_post.size(); i++)
            {
                basic_product_state pP = rR_post[i];
                basic_state p = pP._1;
                basic_set<basic_state> P = pP._2;
                if(accept(pP, B))
                    return false;
                bool exist = false;
                for(int j = 1; j <= Processed.size(); j++)
                {
                    basic_state q = Processed[j]._1;
                    basic_set<basic_state> Q = Processed[j]._2;
                    if(p == q && P.contain(Q))
                    {
                        exist = true;
                        break;
                    }
                }
                if(exist)
                    continue;
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
                    continue;
                for(int j = 1; j <= Processed.size(); j++)
                {
                    basic_state q = Processed[j]._1;
                    basic_set<basic_state> Q = Processed[j]._2;
                    if(q == p && Q.contain(P))
                    {
                        Processed.del(Processed[j]);
                        j--;
                    }
                }
                for(int j = 1; j <= Next.size(); j++)
                {
                    basic_state q = Next[j]._1;
                    basic_set<basic_state> Q = Next[j]._2;
                    if(q == p && Q.contain(P))
                    {
                        Next.del(Next[j]);
                        j--;
                    }
                }
                Next.add(pP);
            }
        }
        return true;
    }

    /// compute the max dimension
    basic_set<basic_tuple<state_with_dimension> > all_combinations(basic_tuple<basic_set<state_with_dimension> > cb) const
    {
        basic_set<basic_tuple<state_with_dimension> > result;
        if(cb.size() == 1)
        {
            for(int i = 1; i <= cb[1].size(); i++)
            {
                basic_tuple<state_with_dimension> temp;
                temp.add(cb[1][i]);
                result.add(temp);
            }
        }
        else
        {
            basic_tuple<basic_set<state_with_dimension> > small_recursion = cb;
            small_recursion._size_repre--;
            basic_set<basic_tuple<state_with_dimension> > result_small_recursion = all_combinations(small_recursion);
            for(int i = 1; i <= result_small_recursion.size(); i++)
                for(int j = 1; j <= cb[cb.size()].size(); j++)
                {
                    basic_tuple<state_with_dimension> temp;
                    temp = result_small_recursion[i];
                    temp.add(cb[cb.size()][j]);
                    result.add(temp);
                }
        }
        return result;
    }

    basic_set<state_with_dimension> Post(basic_set<state_with_dimension> Processed, state_with_dimension qd) const
    {
        basic_set<state_with_dimension> result;
        for(int i = 1; i <= DELTA.size(); i++)
        {
            basic_transition t = DELTA[i];
            bool all_in_Processed = true;
            bool contain_qd = false;
            basic_tuple<basic_set<state_with_dimension> > cb;
            for(int j = 1; j <= t._inputs.size(); j++)
            {
                bool one_in_Processed = false;
                basic_set<state_with_dimension> possible_input_slot;
                for(int k = 1; k <= Processed.size(); k++)
                {
                    if(Processed[k]._name == t._inputs[j]._name)
                    {
                        one_in_Processed = true;
                        possible_input_slot.add(Processed[k]);
                    }
                }
                cb.add(possible_input_slot);
                if(!one_in_Processed)
                {
                    all_in_Processed = false;
                    break;
                }
                if(!contain_qd && t._inputs[j]._name == qd._name)
                    contain_qd = true;
            }
            if(all_in_Processed && contain_qd)
            {
                basic_set<basic_tuple<state_with_dimension> > all_possible_inputs = all_combinations(cb);
                for(int j = 1; j <= all_possible_inputs.size(); j++)
                {
                    state_with_dimension one_result(t._output._name);
                    int max_d = -1;
                    bool max_d_twice = false;
                    bool triangle = false;
                    for(int k = 1; k <= all_possible_inputs[j].size(); k++)
                    {
                        one_result.add_history(all_possible_inputs[j][k]);
                        if(one_result.triangle_check())
                        {
                            triangle = true;
                            break;
                        }
                        if(all_possible_inputs[j][k]._dimension > max_d)
                        {
                            max_d = all_possible_inputs[j][k]._dimension;
                            max_d_twice = false;
                        }
                        else if(all_possible_inputs[j][k]._dimension == max_d)
                            max_d_twice = true;
                    }
                    if(triangle)
                        one_result._dimension = -1;
                    else
                    {
                        if(max_d_twice)
                            max_d++;
                        one_result._dimension = max_d;
                    }
                    result.add(one_result);
                }
            }
        }
        return result;
    }

    int max_dimension() const
    {
        basic_set<state_with_dimension> Processed;
        basic_set<state_with_dimension> Next;
        for(int i = 1; i <= DELTA.size(); i++)
            if(DELTA[i]._alpha._rank == 0)
                Next.add(state_with_dimension(DELTA[i]._output._name, 0));
        int max_dim = 0;
        while(Next.size() > 0)
        {
            state_with_dimension qd = Next[1];
            Next.del(qd);
            Processed.add(qd);
            basic_set<state_with_dimension> Post_Processed_qd = Post(Processed, qd);
            for(int i = 1; i <= Post_Processed_qd.size(); i++)
            {
                state_with_dimension pe = Post_Processed_qd[i];
                if(pe._dimension == -1)
                    return -1;
                if(pe._dimension > max_dim)
                    max_dim = pe._dimension;
                bool exist = false;
                for(int j = 1; j <= Processed.size(); j++)
                {
                    state_with_dimension pf = Processed[j];
                    if(pe <= pf)
                    {
                        exist = true;
                        break;
                    }
                }
                if(exist)
                    continue;
                for(int j = 1; j <= Next.size(); j++)
                {
                    state_with_dimension pf = Next[j];
                    if(pe <= pf)
                    {
                        exist = true;
                        break;
                    }
                }
                if(!exist)
                {
                    for(int j = 1; j <= Processed.size(); j++)
                    {
                        state_with_dimension pf = Processed[j];
                        if(pf <= pe)
                            Processed.del(pf);
                    }
                    for(int j = 1; j <= Next.size(); j++)
                    {
                        state_with_dimension pf = Next[j];
                        if(pf <= pe)
                            Next.del(pf);
                    }
                    Next.add(pe);
                }
            }
        }
        return max_dim;
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
