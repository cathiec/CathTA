#ifndef tree_automaton_h
#define tree_automaton_h

#include "transition.h"
#include "product_state.h"
#include <stdio.h>
#include <stdlib.h>
#include <fstream>

#include "print.h"

#define UPPER_BOUND_DIMENSION 5

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

    /// destructor
    ~tree_automaton()
    {}

    /// assignment
    tree_automaton & operator=(const tree_automaton & ta)
    {
        _name = ta._name;
        Q = ta.Q;
        SIGMA = ta.SIGMA;
        DELTA = ta.DELTA;
        F = ta.F;
        return *this;
    }

    /// union
    /* add a "'" after the name of each state */
    tree_automaton rename() const
    {
        tree_automaton result = *this;
        result._name += "'";
        for(int i = 1; i <= Q._size; i++)
            result.Q[i]._name += "'";
        for(int i = 1; i <= F._size; i++)
            result.F[i]._name += "'";
        for(int i = 1; i <= DELTA._size; i++)
        {
            for(int j = 1; j <= DELTA[i]._input._size; j++)
                result.DELTA[i]._input[j]._name += "'";
            result.DELTA[i]._output._name += "'";
        }
        return result;
    }

    /* union of 2 tree automata */
    tree_automaton U(const tree_automaton & B) const
    {
        tree_automaton result;
        result._name = "Union of ";
        result._name += _name;
        result._name += " and ";
        result._name += B._name;
        result.SIGMA = SIGMA;
        tree_automaton _B = B.rename();
        result.Q = Q;
        result.Q.add(_B.Q);
        result.F = F;
        result.F.add(_B.F);
        result.DELTA = DELTA;
        result.DELTA.add(_B.DELTA);
        return result;
    }

    /// intersection
    /* intersection of 2 tree automata */
    tree_automaton I(const tree_automaton & B) const
    {
        tree_automaton result;
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

    /// inclusion checking
    /* check if a state "s" is accepted */
    bool accept(const state & s) const
    {
        if(F.contain(s))
            return true;
        else
            return false;
    }

    /* check is a macro state "ms" is rejected */
    bool reject(const container<state> & ms) const
    {
        for(int i = 1; i <= ms._size; i++)
            if(accept(ms[i]))
                return false;
        return true;
    }

    /* check if a product state "ps" is accepted */
    bool accept(const product_state & ps, const tree_automaton & B) const
    {
        if(accept(ps._1) && B.reject(ps._2))
            return true;
        else
            return false;
    }

    /* produce the set of initial states for a given symbol "s" */
    container<state> initial_states(const symbol & s) const
    {
        container<state> result;
        for(int i = 1; i <= DELTA._size; i++)
            if(DELTA[i]._alpha == s)
                result.add(DELTA[i]._output);
        return result;
    }

    /* compute the set of post image with a given symbol "s" of a given combination of micro states "cb" */
    container<state> post(const symbol & s, const __container<container<state> > cb) const
    {
        container<state> result;
        if(s._rank != cb._size)
            return result;
        for(int i = 1; i <= DELTA._size; i++)
        {
            transition t = DELTA[i];
            if(t._alpha == s)
            {
                bool ok = true;
                for(int j = 1; j <= s._rank; j++)
                    if(!cb[j].contain(t._input[j]))
                    {
                        ok = false;
                        break;
                    }
                if(ok)
                    result.add(t._output);
            }
        }
        return result;
    }

    /* compute possible Pi according to qi */
    container<container<state> > possible_Pi(const state & qi, const container<product_state> & Processed) const
    {
        container<container<state> > result;
        for(int i = 1; i <= Processed._size; i++)
        {
            if(Processed[i]._1 == qi)
                result.add(Processed[i]._2);
        }
        return result;
    }

    /* compute all the possible combinations of P1 to Pn */
    container<__container<container<state> > > possible_combinations_Pi(const __container<state> & cb_qi, const container<product_state> & Processed) const
    {
        container<__container<container<state> > > result;
        if(cb_qi._size > 1)
        {
            __container<state> next_recursion = cb_qi;
            next_recursion._size--;
            state qn = cb_qi[cb_qi._size];
            container<container<state> > possible_Pn = possible_Pi(qn, Processed);
            container<__container<container<state> > > possible_combinations_P1_to_Pn_minus_1 = possible_combinations_Pi(next_recursion, Processed);
            for(int i = 1; i <= possible_combinations_P1_to_Pn_minus_1._size; i++)
            {
                for(int j = 1; j <= possible_Pn._size; j++)
                {
                    __container<container<state> > cb_Pi = possible_combinations_P1_to_Pn_minus_1[i];
                    container<state> Pn = possible_Pn[j];
                    cb_Pi.add(Pn, true);
                    result.add(cb_Pi);
                }
            }
        }
        else
        {
            state q1 = cb_qi[1];
            container<container<state> > possible_P1 = possible_Pi(q1, Processed);
            for(int i = 1; i <= possible_P1._size; i++)
            {
                __container<container<state> > cb_P1;
                container<state> P1 = possible_P1[i];
                cb_P1.add(P1, true);
                result.add(cb_P1);
            }
        }
        return result;
    }

    /* compute the set of post image of a given set of product states "Processed" w.r.t. a given product state "rR" */
    container<product_state> post(const product_state & rR, const container<product_state> & Processed, const tree_automaton & B) const
    {
        state r = rR._1;
        container<state> R = rR._2;
        container<product_state> result;
        container<state> q_Processed;
        for(int i = 1; i <= Processed._size; i++)
            q_Processed.add(Processed[i]._1);
        for(int i = 1; i <= DELTA._size; i++)
        {
            transition t = DELTA[i];
            container<int> occurrences_r = t._input.occurrences(r);
            if(occurrences_r._size > 0 && q_Processed.contain(t._input))
            {
                container<container<container<state> > > possible_P1_to_Pn = possible_combinations_Pi(t._input, Processed);
                for(int j = 1; j <= possible_P1_to_Pn._size; j++)
                {
                    container<int> occurrences_R = possible_P1_to_Pn[j].occurrences(R);
                    if(occurrences_have_same_index(occurrences_r, occurrences_R))
                        result.add(product_state(t._output, B.post(t._alpha, possible_P1_to_Pn[j])));
                }
            }
        }
        return result;
    }

    /* check if the language of the current tree automaton is included in the language of a tree automaton "B" */
    bool is_included_in(const tree_automaton & B) const
    {
        container<product_state> Processed;
        container<product_state> Next;
        for(int i = 1; i <= DELTA._size; i++)
            if(DELTA[i]._alpha._rank == 0)
            {
                product_state temp(DELTA[i]._output, B.initial_states(DELTA[i]._alpha));
                if(accept(temp, B))
                    return false;
                Next.add(temp);
            }
        while(Next._size > 0)
        {
            product_state rR = Next[1];
            Next.del(rR);
            Processed.add(rR);
            container<product_state> rR_post = post(rR, Processed, B);
            for(int i = 1; i <= rR_post._size; i++)
            {
                product_state pP = rR_post[i];
                state p = pP._1;
                container<state> P = pP._2;
                if(accept(pP, B))
                    return false;
                bool exist = false;
                for(int j = 1; j <= Processed._size; j++)
                {
                    state q = Processed[j]._1;
                    container<state> Q = Processed[j]._2;
                    if(p == q && P.contain(Q))
                    {
                        exist = true;
                        break;
                    }
                }
                if(exist)
                    continue;
                for(int j = 1; j <= Next._size; j++)
                {
                    state q = Next[j]._1;
                    container<state> Q = Next[j]._2;
                    if(p == q && P.contain(Q))
                    {
                        exist = true;
                        break;
                    }
                }
                if(exist)
                    continue;
                for(int j = 1; j <= Processed._size; j++)
                {
                    state q = Processed[j]._1;
                    container<state> Q = Processed[j]._2;
                    if(q == p && Q.contain(P))
                    {
                        Processed.del(Processed[j]);
                        j--;
                    }
                }
                for(int j = 1; j <= Next._size; j++)
                {
                    state q = Next[j]._1;
                    container<state> Q = Next[j]._2;
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

    /// inclusion checking with addition of dimension bound
    /* compute the set of post image with a given symbol "s" of a given combination of micro states "cb" */
    /* procedure is with dimension bound "k" */
    container<state> post(const symbol & s, const __container<container<state> > cb, int bound, bool _try) const
    {
        container<state> result;
        if(s._rank != cb._size)
            return result;
        int min_dim = -1;
        int max_dim = -1;
        bool max_2_times = false;
        bool min_2_times = false;

        for(int i = 1; i <= cb._size; i++)
        {
            if(cb[i]._dimension.lower() > min_dim)
            {
                min_dim = cb[i]._dimension.lower();
                min_2_times = false;
            }
            else if(cb[i]._dimension.lower() == min_dim)
                min_2_times = true;
            if(cb[i]._dimension.upper() > max_dim)
            {
                max_dim = cb[i]._dimension.upper();
                max_2_times = false;
            }
            else if(cb[i]._dimension.upper() == max_dim)
                max_2_times = true;
        }
        if(max_2_times)
            max_dim++;
        if(min_2_times)
            min_dim++;
        if(max_dim > bound)
            max_dim = bound + 1;
        if(min_dim > bound)
            min_dim = bound + 1;
        dim result_dimension;
        for(int i = min_dim; i <= max_dim; i++)
            result_dimension.add(i);

        if(result_dimension._size == 0)
            return result;
        for(int i = 1; i <= DELTA._size; i++)
        {
            transition t = DELTA[i];
            if(t._alpha == s)
            {
                bool ok = true;
                for(int j = 1; j <= s._rank; j++)
                    if(!cb[j].contain(t._input[j]))
                    {
                        ok = false;
                        break;
                    }
                if(ok)
                    result.add(t._output);
            }
        }
        if(result._size > 0)
            result.set_dimension(result_dimension);
        /*std::cout << result << std::endl;
        std::cout << std::endl;*/
        return result;
    }

    /* compute the set of post image of a given set of product states "Processed" w.r.t. a given product state "rR" */
    /* procedure is with dimension bound "k" */
    container<product_state> post(const product_state & rR, const container<product_state> & Processed, const tree_automaton & B, int bound, bool _try) const
    {
        state r = rR._1;
        container<state> R = rR._2;
        container<product_state> result;
        container<state> q_Processed;
        for(int i = 1; i <= Processed._size; i++)
            q_Processed.add(Processed[i]._1);
        for(int i = 1; i <= DELTA._size; i++)
        {
            transition t = DELTA[i];
            container<int> occurrences_r = t._input.occurrences(r);
            if(occurrences_r._size > 0 && q_Processed.contain(t._input))
            {
                container<container<container<state> > > possible_P1_to_Pn = possible_combinations_Pi(t._input, Processed);
                for(int j = 1; j <= possible_P1_to_Pn._size; j++)
                {
                    container<int> occurrences_R = possible_P1_to_Pn[j].occurrences(R);
                    if(occurrences_have_same_index(occurrences_r, occurrences_R))
                    {
                        container<state> P = B.post(t._alpha, possible_P1_to_Pn[j], bound, true);
                        if(P._dimension.upper() > bound)
                        {
                            P._dimension.del(bound + 1);
                            container<state> empty_set;
                            result.add(product_state(t._output, empty_set));
                        }
                        result.add(product_state(t._output, P));
                    }
                }
            }
        }
        return result;
    }

    /* check if the language of the current tree automaton is included in the language of a tree automaton "B" */
    /* procedure is with dimension bound "k" */
    bool is_included_in(const tree_automaton & B, int bound) const
    {
        container<product_state> Processed;
        container<product_state> Next;
        for(int i = 1; i <= DELTA._size; i++)
            if(DELTA[i]._alpha._rank == 0)
            {
                container<state> init_B = B.initial_states(DELTA[i]._alpha);
                dim dim_init_B;
                dim_init_B.add(0);
                init_B.set_dimension(dim_init_B);
                product_state temp(DELTA[i]._output, init_B);
                if(accept(temp, B))
                    return false;
                Next.add(temp);
            }
        while(Next._size > 0)
        {
            product_state rR = Next[1];
            Next.del(rR);
            Processed.add(rR);
            container<product_state> rR_post = post(rR, Processed, B, bound, true);
            for(int i = 1; i <= rR_post._size; i++)
            {
                product_state pP = rR_post[i];
                state p = pP._1;
                container<state> P = pP._2;
                //std::cout << P._dimension << std::endl;
                //if(P._dimension.upper() > k)
                //std::cout << pP << std::endl;
                if(accept(pP, B))
                    return false;
                bool exist = false;

                for(int j = 1; j <= Processed._size; j++)
                {
                    state q = Processed[j]._1;
                    container<state> Q = Processed[j]._2;
                    if(p == q && (Q._size == 0 || (P.contain(Q) && (P.common_dimensions(Q)._size > 0))))
                    {
                        exist = true;
                        break;
                    }
                }
                if(exist)
                    continue;
                for(int j = 1; j <= Next._size; j++)
                {
                    state q = Next[j]._1;
                    container<state> Q = Next[j]._2;
                    if(p == q && (Q._size == 0 || (P.contain(Q) && (P.common_dimensions(Q)._size > 0))))
                    {
                        exist = true;
                        break;
                    }
                }
                if(exist)
                    continue;
                for(int j = 1; j <= Processed._size; j++)
                {
                    state q = Processed[j]._1;
                    container<state> Q = Processed[j]._2;
                    if(q == p && Q.contain(P))
                    {
                        if(P._size > 0)
                        {
                            container<int> common_dimension_PQ = P.common_dimensions(Q);
                            for(int x = 1; x <= common_dimension_PQ._size; x++)
                                Processed[j]._2._dimension.del(common_dimension_PQ[x]);
                            if(Processed[j]._2._dimension._size == 0)
                            {
                                Processed.del(Processed[j]);
                                j--;
                            }
                        }
                        else
                        {
                            Processed.del(Processed[j]);
                            j--;
                        }
                    }
                }
                for(int j = 1; j <= Next._size; j++)
                {
                    state q = Next[j]._1;
                    container<state> Q = Next[j]._2;
                    if(q == p && Q.contain(P))
                    {
                        if(P._size > 0)
                        {
                            container<int> common_dimension_PQ = P.common_dimensions(Q);
                            for(int x = 1; x <= common_dimension_PQ._size; x++)
                                Next[j]._2._dimension.del(common_dimension_PQ[x]);
                            if(Next[j]._2._dimension._size == 0)
                            {
                                Next.del(Next[j]);
                                j--;
                            }
                        }
                        else
                        {
                            Next.del(Next[j]);
                            j--;
                        }

                    }
                }
                /*std::cout << pP << std::endl;
                std::cout << pP._2._dimension << std::endl;*/
                Next.add(pP);
            }
        }
        return true;
    }

    /// compute the max dimension
    container<state> post(const symbol & s, const __container<container<state> > cb, int & bound) const
    {
        container<state> result;
        if(s._rank != cb._size)
            return result;
        int min_dim = -1;
        int max_dim = -1;
        bool max_2_times = false;
        bool min_2_times = false;
        //std::cout << cb << std::endl;
        for(int i = 1; i <= cb._size; i++)
        {
            if(cb[i]._dimension.lower() > min_dim)
            {
                min_dim = cb[i]._dimension.lower();
                min_2_times = false;
            }
            else if(cb[i]._dimension.lower() == min_dim)
                min_2_times = true;
            if(cb[i]._dimension.upper() > max_dim)
            {
                max_dim = cb[i]._dimension.upper();
                max_2_times = false;
            }
            else if(cb[i]._dimension.upper() == max_dim)
                max_2_times = true;
        }
        if(max_2_times)
            max_dim++;
        if(min_2_times)
            min_dim++;
        dim result_dimension;
        for(int i = min_dim; i <= max_dim; i++)
            result_dimension.add(i);
        if(result_dimension._size == 0)
            return result;
        for(int i = 1; i <= DELTA._size; i++)
        {
            transition t = DELTA[i];
            if(t._alpha == s)
            {
                bool ok = true;
                for(int j = 1; j <= s._rank; j++)
                {
                    if(!cb[j].contain(t._input[j]))
                    {
                        ok = false;
                        break;
                    }
                }
                if(ok)
                    result.add(t._output);
            }
        }
        if(result._size > 0)
            result.set_dimension(result_dimension);
        /*std::cout << result << std::endl;
        std::cout << std::endl;*/
        if(result._dimension.upper() > bound)
            bound = result._dimension.upper();
        return result;
    }

    container<product_state> post(const product_state & rR, const container<product_state> & Processed, const tree_automaton & B, int & bound) const
    {
        state r = rR._1;
        container<state> R = rR._2;
        container<product_state> result;
        __container<state> q_Processed;
        for(int i = 1; i <= Processed._size; i++)
            q_Processed.add(Processed[i]._1, true);
        for(int i = 1; i <= DELTA._size; i++)
        {
            transition t = DELTA[i];
            container<int> occurrences_r = t._input.occurrences(r);
            if(occurrences_r._size > 0 && q_Processed.contain(t._input))
            {
                container<container<container<state> > > possible_P1_to_Pn = possible_combinations_Pi(t._input, Processed);
                for(int j = 1; j <= possible_P1_to_Pn._size; j++)
                {
                    container<int> occurrences_R = possible_P1_to_Pn[j].occurrences(R);
                    if(occurrences_have_same_index(occurrences_r, occurrences_R))
                        result.add(product_state(t._output, B.post(t._alpha, possible_P1_to_Pn[j], bound)));
                }
            }
        }
        return result;
    }

    int upper_bound_dimension() const
    {
        int bound = 0;
        container<product_state> Processed;
        container<product_state> Next;
        for(int i = 1; i <= DELTA._size; i++)
            if(DELTA[i]._alpha._rank == 0)
            {
                //std::cout << DELTA[i] << std::endl;
                container<state> init_B = initial_states(DELTA[i]._alpha);
                dim dim_init_B;
                dim_init_B.add(0);
                init_B.set_dimension(dim_init_B);
                product_state temp(DELTA[i]._output, init_B);
                /*if(accept(temp,*this))
                    return bound;*/
                Next.add(temp);
            }
        while(Next._size > 0)
        {
            product_state rR = Next[1];
            //std::cout << rR << std::endl;
            Next.del(rR);
            Processed.add(rR);
            container<product_state> rR_post = post(rR, Processed, *this, bound);
            //std::cout << Processed << std::endl;
            if(bound > UPPER_BOUND_DIMENSION)
                return -1;
            for(int i = 1; i <= rR_post._size; i++)
            {
                product_state pP = rR_post[i];
                state p = pP._1;
                container<state> P = pP._2;
                /*if(accept(pP, *this))
                    return bound;*/
                bool exist = false;
                for(int j = 1; j <= Processed._size; j++)
                {
                    state q = Processed[j]._1;
                    container<state> Q = Processed[j]._2;
                    if(p == q && (Q._size == 0 || (P.contain(Q) && (P.common_dimensions(Q)._size > 0))))
                    {
                        exist = true;
                        break;
                    }
                }
                if(exist)
                    continue;
                for(int j = 1; j <= Next._size; j++)
                {
                    state q = Next[j]._1;
                    container<state> Q = Next[j]._2;
                    if(p == q && (Q._size == 0 || (P.contain(Q) && (P.common_dimensions(Q)._size > 0))))
                    {
                        exist = true;
                        break;
                    }
                }
                if(exist)
                    continue;
                for(int j = 1; j <= Processed._size; j++)
                {
                    state q = Processed[j]._1;
                    container<state> Q = Processed[j]._2;
                    if(q == p && Q.contain(P))
                    {
                        if(P._size > 0)
                        {
                            container<int> common_dimension_PQ = P.common_dimensions(Q);
                            for(int x = 1; x <= common_dimension_PQ._size; x++)
                                Processed[j]._2._dimension.del(common_dimension_PQ[x]);
                            if(Processed[j]._2._dimension._size == 0)
                            {
                                Processed.del(Processed[j]);
                                j--;
                            }
                        }
                        else
                        {
                            Processed.del(Processed[j]);
                            j--;
                        }
                    }
                }
                for(int j = 1; j <= Next._size; j++)
                {
                    state q = Next[j]._1;
                    container<state> Q = Next[j]._2;
                    if(q == p && Q.contain(P))
                    {
                        if(P._size > 0)
                        {
                            container<int> common_dimension_PQ = P.common_dimensions(Q);
                            for(int x = 1; x <= common_dimension_PQ._size; x++)
                                Next[j]._2._dimension.del(common_dimension_PQ[x]);
                            if(Next[j]._2._dimension._size == 0)
                            {
                                Next.del(Next[j]);
                                j--;
                            }
                        }
                        else
                        {
                            Next.del(Next[j]);
                            j--;
                        }
                    }
                }
                //std::cout << pP._2 << std::endl;
                Next.add(pP);
            }
        }
        return bound;
    }

};

/// tree_automaton -> std::string
std::string to_string(const tree_automaton & ta)
{
    std::string result = "";
    int i;
    result += "Automaton [";
    result += ta._name;
    result += "]\n";
    result += "--- RANKED ALPHABET---\n";
    for(i = 1; i <= ta.SIGMA._size; i++)
    {
        result += "\t";
        result += to_string(ta.SIGMA[i]);
        result += "\n";
    }
    result += "--- STATES ---\n";
    for(i = 1; i <= ta.Q._size; i++)
    {
        result += "\t";
        result += to_string(ta.Q[i]);
        result += "\n";
    }
    result += "--- FINAL STATES ---\n";
    for(i = 1; i <= ta.F._size; i++)
    {
        result += "\t";
        result += to_string(ta.F[i]);
        result += "\n";
    }
    result += "--- TRANSITIONS ---\n";
    for(i = 1; i <= ta.DELTA._size; i++)
    {
        result += "\t";
        result += to_string(ta.DELTA[i]);
        result += "\n";
    }
    return result;
}

/// print a tree automaton
/* print a tree automaton by std::cout */
std::ostream & operator<<(std::ostream & out, const tree_automaton & ta)
{
    out << to_string(ta);
    return out;
}

#endif
