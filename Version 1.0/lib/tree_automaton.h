#ifndef TREE_AUTOMATON_h
#define TREE_AUTOMATON_h

#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include "transition.h"

class product_state
{

public:

    state _1;
    set<state> _2;

public:

    product_state()
    {}

    product_state(const state & s, const set<state> &ms)
    :_1(s), _2(ms)
    {}

    ~product_state()
    {}

    bool operator==(const product_state & ps) const
    {
        return (_1 == ps._1) && (_2 == ps._2);
    }

    bool operator!=(const product_state & ps) const
    {
        return !(*this == ps);
    }

    std::string to_string() const
    {
        std::string result = "(";
        result += _1.to_string();
        result += ",{";
        if(_2.size() == 0)
        {
            result += "})";
            return result;
        }
        for(int i = 1; i <= _2.size(); i++)
        {
            result += _2[i].to_string();
            result += ",";
        }
        result += "\b})";
        return result;
    }

    friend std::ostream & operator<<(std::ostream & out, const product_state & ps)
    {
        out << ps.to_string();
        return out;
    }

};

class tree_automaton
{

public:

    std::string _name;

    //ranked alphabet
    set<symbol> SIGMA;

    //finite set of states
    set<state> Q;

    //set of final states
    set<state> F;

    //set of transition rules
    set<transition> DELTA;

public:

    //constructor

    tree_automaton()
    {}

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
                    SIGMA.add(symbol(symbol_name, symbol_rank));
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
                    Q.add(state(state_name));
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
                    F.add(temp);
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
                    DELTA.add(temp);
                    nb_transitions++;
                }
            }
        }
        else
        {
            throw 7; ///no such file
        }
    }

    //destructor
    ~tree_automaton()
    {}

    //take value with =

    tree_automaton & operator=(const tree_automaton & ta)
    {
        Q = ta.Q;
        SIGMA = ta.SIGMA;
        DELTA = ta.DELTA;
        F = ta.F;
        return *this;
    }

    //comparison

    bool operator==(const tree_automaton & ta) const
    {
        return (Q == ta.Q) && (SIGMA == ta.SIGMA) && (DELTA == ta.DELTA) && (F == ta.F);
    }

    bool operator!=(const tree_automaton & ta) const
    {
        return !(*this == ta);
    }

    //union
    tree_automaton rename() const
    {
        tree_automaton result = *this;
        result._name += "'";
        for(int i = 1; i <= Q.size(); i++)
            result.Q[i]._name += "'";
        for(int i = 1; i <= F.size(); i++)
            result.F[i]._name += "'";
        for(int i = 1; i <= DELTA.size(); i++)
        {
            for(int j = 1; j <= DELTA[i]._input.size(); j++)
                result.DELTA[i]._input[j]._name += "'";
            result.DELTA[i]._output._name += "'";
        }
        return result;
    }

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

    //intersection
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

    //string and print

    std::string to_string() const
    {
        std::string result = "";
        int i;
        //automaton name
        result += "Automaton [";
        result += _name;
        result += "]\n";
        //ranked alphabet
        result += "--- RANKED ALPHABET---\n";
        for(i = 1; i <= SIGMA.size(); i++)
        {
            result += "\t";
            result += SIGMA[i].to_string();
            result += "\n";
        }
        //states
        result += "--- STATES ---\n";
        for(i = 1; i <= Q.size(); i++)
        {
            result += "\t";
            result += Q[i].to_string();
            result += "\n";
        }
        //final states
        result += "--- FINAL STATES ---\n";
        for(i = 1; i <= F.size(); i++)
        {
            result += "\t";
            result += F[i].to_string();
            result += "\n";
        }
        //transitions
        result += "--- TRANSITIONS ---\n";
        for(i = 1; i <= DELTA.size(); i++)
        {
            result += "\t";
            result += DELTA[i].to_string();
            result += "\n";
        }
        return result;
    }

    friend std::ostream & operator<<(std::ostream & out, const tree_automaton & ta)
    {
        out << ta.to_string();
        return out;
    }

    //inclusion checking

    bool accepts(const state & s) const
    {
        if(F.contain(s))
            return true;
        else
            return false;
    }

    bool accepts(const product_state & ps, const tree_automaton & B) const
    {
        if(accepts(ps._1) && B.rejects(ps._2))
            return true;

        else
            return false;
    }

    bool rejects(const set<state> & ms) const
    {
        for(int i = 1; i <= ms.size(); i++)
            if(accepts(ms[i]))
                return false;
        return true;
    }

    set<set<state> > possible_Pi(const state & qi, const set<product_state> & Processed) const
    {
        set<set<state> > result;
        for(int i = 1; i <= Processed.size(); i++)
        {
            if(Processed[i]._1 == qi)
                result.add(Processed[i]._2);
        }
        return result;
    }

    set<box<set<state> > > possible_combinations_Pi(box<state> cb_qi, const set<product_state> & Processed) const
    {
        set<box<set<state> > > result;
        if(cb_qi.size() > 1)
        {
            box<state> next_recursion = cb_qi;
            next_recursion._size--;
            state qn = cb_qi[cb_qi.size()];
            set<set<state> > possible_Pn = possible_Pi(qn, Processed);
            set<box<set<state> > > possible_combinations_first_n_minus_1_Pi = possible_combinations_Pi(next_recursion, Processed);
            for(int i = 1; i <= possible_combinations_first_n_minus_1_Pi.size(); i++)
            {
                for(int j = 1; j <= possible_Pn.size(); j++)
                {
                    box<set<state> > cb_Pi = possible_combinations_first_n_minus_1_Pi[i];
                    set<state> Pn = possible_Pn[j];
                    cb_Pi.add(Pn);
                    result.add(cb_Pi);
                }
            }
        }
        else
        {
            state q1 = cb_qi[1];
            set<set<state> > possible_P1 = possible_Pi(q1, Processed);
            for(int i = 1; i <= possible_P1.size(); i++)
            {
                box<set<state> > cb_P1;
                set<state> P1 = possible_P1[i];
                cb_P1.add(P1);
                result.add(cb_P1);
            }
        }
        return result;
    }

    set<state> post(const symbol & s, const box<set<state> > cb_Pi) const
    {
        set<state> result;
        if(s._rank != cb_Pi.size())
            return result;
        for(int i = 1; i <= DELTA.size(); i++)
        {
            transition t = DELTA[i];
            if(t._alpha == s)
            {
                bool ok = true;
                for(int j = 1; j <= s._rank; j++)
                {
                    if(!cb_Pi[j].contain(t._input[j]))
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

    set<product_state> post(const product_state & rR, const set<product_state> & Processed, const tree_automaton & B) const
    {
        state r = rR._1;
        set<state> R = rR._2;
        set<product_state> result;
        set<state> q_Processed;
        for(int i = 1; i <= Processed.size(); i++)
            q_Processed.add(Processed[i]._1);
        for(int i = 1; i <= DELTA.size(); i++)
        {
            transition t = DELTA[i];
            if(t._input.contain(r) && q_Processed.contain(t._input))
            {
                set<box<set<state> > > possible_cb_Pi = possible_combinations_Pi(t._input, Processed);
                for(int j = 1; j <= possible_cb_Pi.size(); j++)
                {
                    product_state temp(t._output, B.post(t._alpha, possible_cb_Pi[j]));
                    result.add(temp);
                }
            }
        }
        return result;
    }

    set<state> initial(const symbol & s) const
    {
        set<state> result;
        for(int i = 1; i <= DELTA.size(); i++)
        {
            if(DELTA[i]._alpha == s)
                result.add(DELTA[i]._output);
        }
        if(result.size() > 0)
            result._dimension = 0;
        return result;
    }

    bool inclusion_check(const tree_automaton & B) const
    {
        set<product_state> Processed;
        set<product_state> Next;
        for(int i = 1; i <= DELTA.size(); i++)
        {
            if(DELTA[i]._alpha._rank == 0)
            {
                product_state temp(DELTA[i]._output, B.initial(DELTA[i]._alpha));
                if(accepts(temp, B))
                    return false;
                Next.add(temp);
            }
        }
        while(Next.size() > 0)
        {
            product_state rR = Next[1];
            std::cout << rR << std::endl;
            Next.remove(rR);
            Processed.add(rR);
            set<product_state> rR_post = post(rR, Processed, B);
            for(int i = 1; i <= rR_post.size(); i++)
            {
                product_state pP = rR_post[i];
                state p = pP._1;
                set<state> P = pP._2;
                if(accepts(pP, B))
                    return false;
                else
                {
                    bool exist = false;
                    for(int j = 1; j <= Processed.size(); j++)
                    {
                        product_state qQ = Processed[j];
                        state q = qQ._1;
                        set<state> Q = qQ._2;
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
                        product_state qQ = Next[j];
                        state q = qQ._1;
                        set<state> Q = qQ._2;
                        if(p == q && P.contain(Q))
                        {
                            exist = true;
                            break;
                        }
                    }
                    if(!exist)
                    {
                        for(int j = 1; j <= Processed.size(); j++)
                        {
                            product_state qQ = Processed[j];
                            state q = qQ._1;
                            set<state> Q = qQ._2;
                            if(q == p && Q.contain(P))
                                Processed.remove(qQ);
                        }
                        for(int j = 1; j <= Next.size(); j++)
                        {
                            product_state qQ = Next[j];
                            state q = qQ._1;
                            set<state> Q = qQ._2;
                            if(q == p && Q.contain(P))
                                Next.remove(qQ);
                        }
                        Next.add(pP);
                    }
                }
            }
        }
        return true;
    }

    //inclusion checking with addition of dimension

    set<state> post(int k, const symbol & s, const box<set<state> > cb_Pi) const
    {
        set<state> result;
        result._dimension = -1;
        if(s._rank != cb_Pi.size())
            return result;
        bool d_add_1 = false;
        int max_dimension = -1;
        for(int i = 1; i <= cb_Pi.size(); i++)
        {
            if(cb_Pi[i]._dimension > max_dimension)
            {
                max_dimension = cb_Pi[i]._dimension;
                d_add_1 = false;
            }
            else if(cb_Pi[i]._dimension == max_dimension)
            {
                d_add_1 = true;
            }
        }
        if(d_add_1)
            result._dimension = max_dimension + 1;
        else
            result._dimension = max_dimension;
        if(result._dimension > k)
        {
            result._dimension = -1;
            return result;
        }
        for(int i = 1; i <= DELTA.size(); i++)
        {
            transition t = DELTA[i];
            if(t._alpha == s)
            {
                bool ok = true;
                for(int j = 1; j <= s._rank; j++)
                {
                    if(!cb_Pi[j].contain(t._input[j]))
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

    set<product_state> post(int k, const product_state & rR, const set<product_state> & Processed, const tree_automaton & B) const
    {
        state r = rR._1;
        set<state> R = rR._2;
        set<product_state> result;
        set<state> q_Processed;
        for(int i = 1; i <= Processed.size(); i++)
            q_Processed.add(Processed[i]._1);
        for(int i = 1; i <= DELTA.size(); i++)
        {
            transition t = DELTA[i];
            if(t._input.contain(r) && q_Processed.contain(t._input))
            {
                set<box<set<state> > > possible_cb_Pi = possible_combinations_Pi(t._input, Processed);
                for(int j = 1; j <= possible_cb_Pi.size(); j++)
                {
                    product_state temp(t._output, B.post(k, t._alpha, possible_cb_Pi[j]));
                    result.add(temp);
                }
            }
        }
        return result;
    }

    bool inclusion_check(int k, const tree_automaton & B) const
    {
        set<product_state> Processed;
        set<product_state> Next;
        for(int i = 1; i <= DELTA.size(); i++)
        {
            if(DELTA[i]._alpha._rank == 0)
            {
                product_state temp(DELTA[i]._output, B.initial(DELTA[i]._alpha));
                if(accepts(temp, B))
                    return false;
                Next.add(temp);
            }
        }
        while(Next.size() > 0)
        {
            product_state rR = Next[1];
            Next.remove(rR);
            Processed.add(rR);
            set<product_state> rR_post = post(k, rR, Processed, B);
            for(int i = 1; i <= rR_post.size(); i++)
            {
                product_state pP = rR_post[i];
                state p = pP._1;
                set<state> P = pP._2;
                if(accepts(pP, B))
                    return false;
                else
                {
                    bool exist = false;
                    for(int j = 1; j <= Processed.size(); j++)
                    {
                        product_state qQ = Processed[j];
                        state q = qQ._1;
                        set<state> Q = qQ._2;
                        if(q == p && P.contain(Q) && P._dimension >= Q._dimension)
                        {
                            exist = true;
                            break;
                        }
                    }
                    if(exist)
                        continue;
                    for(int j = 1; j <= Next.size(); j++)
                    {
                        product_state qQ = Next[j];
                        state q = qQ._1;
                        set<state> Q = qQ._2;
                        if(q == p && P.contain(Q) && P._dimension >= Q._dimension)
                        {
                            exist = true;
                            break;
                        }
                    }
                    if(!exist)
                    {
                        for(int j = 1; j <= Processed.size(); j++)
                        {
                            product_state qQ = Processed[j];
                            state q = qQ._1;
                            set<state> Q = qQ._2;
                            if(q == p && Q.contain(P) && P._dimension <= Q._dimension)
                            {
                                //std::cout << "REMOVE " << Q._dimension << " " << qQ << std::endl;
                                Processed.remove(qQ);
                            }

                        }
                        for(int j = 1; j <= Next.size(); j++)
                        {
                            product_state qQ = Next[j];
                            state q = qQ._1;
                            set<state> Q = qQ._2;
                            if(q == p && Q.contain(P) && P._dimension <= Q._dimension)
                            {
                                //std::cout << "REMOVE " << Q._dimension << " " << qQ << std::endl;
                                Next.remove(qQ);
                            }
                        }
                        //std::cout << "ADD " << P._dimension << " " << pP << std::endl;
                        Next.add(pP);
                    }
                }
            }
        }
        return true;
    }

    int max_dimension() const
    {
        for(int i = 0; i <= 10; i++)
        {
            if(inclusion_check(i, *this))
                return i;
        }
        return -1;
    }

};

#endif
