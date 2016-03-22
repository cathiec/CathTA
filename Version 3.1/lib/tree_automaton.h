#ifndef _tree_automaton_h
#define _tree_automaton_h

#include "transition.h"
#include "symbol.h"
#include <fstream>
#include "product_state.h"

class tree_automaton
{

private:

    // name of automaton
    std::string _name;

    // set of states
    std::vector<state> Q;

    // set of symbols (ranked alphabet)
    std::vector<symbol> SIGMA;

    // set of final states
    std::vector<state> F;

    // set of transition rules
    std::vector<transition> DELTA;

public:

    // default constructor
    tree_automaton()
    {}

    // constructor by loading a file
    tree_automaton(std::string f)
    {
        load_from_file(f);
    }

    // reference recopy constructor
    tree_automaton(const tree_automaton & ta)
    :_name(ta._name), Q(ta.Q), SIGMA(ta.SIGMA), F(ta.F), DELTA(ta.DELTA)
    {}

    // destructor
    ~tree_automaton()
    {}

    // load the automaton from a file
    void load_from_file(std::string f)
    {
        std::ifstream file(f);
        std::string t;
        // SIGMA
        while(file >> t)
            if(t == "Ops")
                break;
        char c;
        int i;
        t = "";
        while(file >> c)
            if(c != ':')
            {
                t += c;
                if(t == "Automaton")
                    break;
            }
            else
            {
                file >> i;
                SIGMA.push_back(symbol(t, i));
                t = "";
            }
        // name
        file >> _name;
        // states
        while(file >> t)
            if(t == "States")
                break;
        t = "";
        while(file >> t)
            if(t != "Final")
            {
                int p2_pos = t.find(':');
                if(p2_pos != std::string::npos)
                    Q.push_back(state(t.substr(0, p2_pos)));
                else
                    Q.push_back(state(t));
            }
            else
                break;
        // final states
        while(file >> t)
            if(t == "States")
                break;
        while(file >> t)
            if(t != "Transitions")
            {
                F.push_back(state(t));
            }
            else
                break;
        // transitions
        while(file >> c)
        {
            t = "";
            bool has_argument = false;
            t += c;
            while(file >> c)
            {
                if(c == '(')
                {
                    has_argument = true;
                    break;
                }
                else if(c == '-')
                {
                    has_argument = false;
                    break;
                }
                else
                    t += c;
            }
            transition trstn;
            trstn.set_name(t);
            t = "";
            if(has_argument)
            {
                while(file >> c)
                {
                    if(c != ',' && c != ')')
                        t += c;
                    else
                    {
                        trstn.add_input(state(t));
                        t = "";
                    }
                    if(c == ')')
                        break;
                }
                while(file >> c)
                    if(c == '-')
                        break;
            }
            while(file >> c)
                if(c == '>')
                    break;
            file >> t;
            trstn.set_output(state(t));
            DELTA.push_back(trstn);
        }
        file.close();
    }

    // comparison
    bool operator==(const tree_automaton & ta) const
    {
        return (_name == ta._name && Q == ta.Q && SIGMA == ta.SIGMA && F == ta.F && DELTA == ta.DELTA);
    }

    // assignment
    tree_automaton & operator=(const tree_automaton & ta)
    {
        _name = ta._name;
        Q = ta.Q;
        SIGMA = ta.SIGMA;
        F = ta.F;
        DELTA = ta.DELTA;
        return *this;
    }

    // get name
    std::string get_name() const
    {
        return _name;
    }

    // set name
    void set_name(std::string n)
    {
        _name = n;
    }

    // get number of states
    int number_of_states() const
    {
        return Q.size();
    }

    // get number of symbols
    int number_of_symbols() const
    {
        return SIGMA.size();
    }

    // get number of final states
    int number_of_final_states() const
    {
        return F.size();
    }

    // get number of transitions
    int number_of_transitions() const
    {
        return DELTA.size();
    }

    // get set of states
    const std::vector<state> & get_Q() const
    {
        return Q;
    }

    // get set of symbols
    const std::vector<symbol> & get_SIGMA() const
    {
        return SIGMA;
    }

    // get set of final states
    const std::vector<state> & get_F() const
    {
        return F;
    }

    // get set of transitions
    const std::vector<transition> & get_DELTA() const
    {
        return DELTA;
    }

    // return a string describing the automaton in timbuk format
    std::string to_string() const
    {
        std::string result;
        // SIGMA
        result += "Ops ";
        for(int i = 0; i < SIGMA.size(); i++)
        {
            result += SIGMA[i].to_string();
            result += ' ';
        }
        result += '\n';
        // name
        result += "Automaton ";
        result += _name;
        result += '\n';
        // states
        result += "States ";
        for(int i = 0; i < Q.size(); i++)
        {
            result += Q[i].to_string();
            result += ":0 ";
        }
        result += '\n';
        // final states
        result += "Final States ";
        for(int i = 0; i < F.size(); i++)
        {
            result += F[i].to_string();
            result += ' ';
        }
        result += '\n';
        // transitions
        result += "Transitions\n";
        for(int i = 0; i < DELTA.size(); i++)
        {
            result += DELTA[i].to_string();
            result += '\n';
        }
        return result;
    }

    // save the automaton into a file by giving the filename
    void save_into_file(std::string f) const
    {
        std::ofstream file(f);
        file << to_string();
        file.close();
    }

    /// classical anti chain
    // check if a state is accepted
    bool accept(const state & s) const
    {
        if(has_value(F, s))
            return true;
        else
            return false;
    }

    // check if a macro state is rejected
    bool reject(const std::set<state> & ms) const
    {
        for(std::set<state>::const_iterator it = ms.begin(); it != ms.end(); it++)
            if(accept(*it))
                return false;
        return true;
    }

    // check if a product state is accepted
    bool accept(const product_state & ps, const tree_automaton & B) const
    {
        if(accept(ps._1) && B.reject(ps._2))
        {
            std::cout << "Height: " << ps._1._height << std::endl;
            return true;
        }
        else
            return false;
    }

    // compute the initial states for a given symbol
    std::set<state> initial_states(std::string s) const
    {
        std::set<state> result;
        for(int i = 0; i < DELTA.size(); i++)
        {
            if(DELTA[i].get_name() == s)
                result.insert(DELTA[i].get_output());
        }
        return result;
    }

    // compute the set of post images with a given symbol of a given combination of micro states
    std::set<state> post(const transition & t, const std::vector<std::set<state> > cb) const
    {
        std::set<state> result;
        if(t.get_arity() != cb.size())
            return result;
        for(int i = 0; i < DELTA.size(); i++)
        {
            transition tt = DELTA[i];
            if(tt.get_name() == t.get_name() && tt.get_arity() == t.get_arity())
            {
                bool ok = true;
                for(int j = 0; j < t.get_arity(); j++)
                {
                    std::set<state>::const_iterator it_found = cb[j].find(tt.get_input(j));
                    if(it_found == cb[j].end())
                    {
                        ok = false;
                        break;
                    }
                }
                if(ok)
                    result.insert(tt.get_output());
            }
        }
        return result;
    }

    // compute possible Pi according to qi
    std::set<std::set<state> > possible_Pi(const state & qi, const std::set<product_state> & Processed) const
    {
        std::set<std::set<state> > result;
        for(std::set<product_state>::const_iterator it = Processed.begin(); it != Processed.end(); it++)
        {
            if(it->_1 == qi)
                result.insert(it->_2);
        }
        return result;
    }

    // compute all the possible combinations of P1 to Pn
    std::set<std::vector<std::set<state> > > possible_combinations_Pi(const std::vector<state> & cb_qi,
                                                                      const std::set<product_state> & Processed) const
    {
        std::set<std::vector<std::set<state> > > result;
        if(cb_qi.size() > 1)
        {
            std::vector<state> next_recursion = cb_qi;
            next_recursion.erase(next_recursion.end() - 1);
            state qn = cb_qi[cb_qi.size() - 1];
            std::set<std::set<state> > possible_Pn = possible_Pi(qn, Processed);
            std::set<std::vector<std::set<state> > > possible_combinations_P1_to_Pn_minus_1 = possible_combinations_Pi(next_recursion, Processed);
            for(std::set<std::vector<std::set<state> > >::const_iterator it = possible_combinations_P1_to_Pn_minus_1.begin(); it != possible_combinations_P1_to_Pn_minus_1.end(); it++)
            {
                for(std::set<std::set<state> >::const_iterator it2 = possible_Pn.begin(); it2 != possible_Pn.end(); it2++)
                {
                    std::vector<std::set<state> > cb_Pi = *it;
                    std::set<state> Pn = *it2;
                    cb_Pi.push_back(Pn);
                    result.insert(cb_Pi);
                }
            }
        }
        else
        {
            state q1 = cb_qi[0];
            std::set<std::set<state> > possible_P1 = possible_Pi(q1, Processed);
            for(std::set<std::set<state> >::const_iterator it = possible_P1.begin(); it != possible_P1.end(); it++)
            {
                std::vector<std::set<state> > cb_P1;
                std::set<state> P1 = *it;
                cb_P1.push_back(P1);
                result.insert(cb_P1);
            }
        }
        return result;
    }

    // compute the post image of a given set of product states w.r.t. a given product state
    std::set<product_state> post(const product_state & rR,
                                 const std::set<product_state> & Processed,
                                 const tree_automaton & B) const
    {
        std::set<product_state> result;
        state r = rR._1;
        std::set<state> R = rR._2;
        std::set<state> q_Processed;
        for(std::set<product_state>::const_iterator it = Processed.begin(); it != Processed.end(); it++)
            q_Processed.insert(it->_1);
        for(int i = 0; i < DELTA.size(); i++)
        {
            transition t = DELTA[i];
            std::set<int> occurrences_r;
            for(int j = 0; j < t.get_arity(); j++)
                if(t.get_input(j) == r)
                    occurrences_r.insert(j);
            if(occurrences_r.size() > 0 && is_subset(t.get_inputs(), q_Processed))
            {
                std::set<std::vector<std::set<state> > > possible_P1_to_Pn = possible_combinations_Pi(t.get_inputs(), Processed);
                for(std::set<std::vector<std::set<state> > >::const_iterator it = possible_P1_to_Pn.begin(); it != possible_P1_to_Pn.end(); it++)
                {
                    std::set<int> occurrences_R;
                    for(int j = 0; j < it->size(); j++)
                        if((*it)[j] == R)
                            occurrences_R.insert(j);
                    bool have_same_index = false;
                    for(std::set<int>::const_iterator it2 = occurrences_r.begin(); it2 != occurrences_r.end(); it2++)
                        if(occurrences_R.find(*it2) != occurrences_R.end())
                        {
                            have_same_index = true;
                            break;
                        }
                    if(have_same_index)
                        result.insert(product_state(t.get_output(), B.post(t, *it)));
                }
            }
        }
        return result;
    }

    // determine if the language is included in the language of another automaton
    // classical antichains
    bool is_included_in_classical_antichains(const tree_automaton & B) const
    {
        std::set<product_state> Processed, Next;
        for(int i = 0; i < DELTA.size(); i++)
            if(DELTA[i].get_arity() == 0)
            {
                product_state temp(DELTA[i].get_output(), B.initial_states(DELTA[i].get_name()));
                if(accept(temp, B))
                    return false;
                Next.insert(temp);
            }
        while(Next.size() != 0)
        {
            product_state rR = *(Next.begin());
            std::cout << rR._1 << std::endl;
            Next.erase(rR);
            Processed.insert(rR);
            std::set<product_state> rR_post = post(rR, Processed, B);
            for(std::set<product_state>::const_iterator it = rR_post.begin(); it != rR_post.end(); it++)
            {
                product_state pP = *it;
                state p = pP._1;
                std::set<state> P = pP._2;
                if(accept(pP, B))
                    return false;
                bool exist = false;
                for(std::set<product_state>::const_iterator it2 = Processed.begin(); it2 != Processed.end(); it2++)
                {
                    state q = it2->_1;
                    std::set<state> Q = it2->_2;
                    if(p == q && is_subset(Q, P))
                    {
                        exist = true;
                        break;
                    }
                }
                if(exist)
                    continue;
                for(std::set<product_state>::const_iterator it2 = Next.begin(); it2 != Next.end(); it2++)
                {
                    state q = it2->_1;
                    std::set<state> Q = it2->_2;
                    if(p == q && is_subset(Q, P))
                    {
                        exist = true;
                        break;
                    }
                }
                if(exist)
                    continue;
                std::vector<product_state> to_remove;
                for(std::set<product_state>::const_iterator it2 = Processed.begin(); it2 != Processed.end(); it2++)
                {
                    state q = it2->_1;
                    std::set<state> Q = it2->_2;
                    if(q == p && is_subset(P, Q))
                        to_remove.push_back(*it2);
                }
                for(int i = 0; i < to_remove.size(); i++)
                    Processed.erase(to_remove[i]);
                to_remove.clear();
                for(std::set<product_state>::const_iterator it2 = Next.begin(); it2 != Next.end(); it2++)
                {
                    state q = it2->_1;
                    std::set<state> Q = it2->_2;
                    if(q == p && is_subset(P, Q))
                        to_remove.push_back(*it2);
                }
                for(int i = 0; i < to_remove.size(); i++)
                    Next.erase(to_remove[i]);
                Next.insert(pP);
            }
        }
        return true;
    }

    // compute the set of post image of a given set of product states w.r.t. a given product state
    // this function does not work for dimension 0
    std::set<product_state> post(const product_state & rR,
                                 std::set<product_state> * Processed,
                                 int d,
                                 const tree_automaton & B) const
    {
        std::set<product_state> result;
        state r = rR._1;
        std::set<state> R = rR._2;
        std::set<state> q_Processed;
        std::set<product_state> safe_Processed;
        safe_Processed.insert(rR);
        for(int j = 0; j < d; j++)
            for(std::set<product_state>::const_iterator it = Processed[j].begin(); it != Processed[j].end(); it++)
                safe_Processed.insert(*it);
        for(std::set<product_state>::const_iterator it = safe_Processed.begin(); it != safe_Processed.end(); it++)
            q_Processed.insert(it->_1);
        for(int i = 0; i < DELTA.size(); i++)
        {
            transition t = DELTA[i];
            std::set<int> occurrences_r;
            for(int j = 0; j < t.get_arity(); j++)
                if(t.get_input(j) == r)
                    occurrences_r.insert(j);
            bool inputs_ok = true;
            int max_height = 0;
            for(int j = 0; j < t.get_inputs().size(); j++)
            {
                std::set<state>::const_iterator it_found = q_Processed.find(t.get_input(j));
                if(it_found == q_Processed.end())
                {
                    inputs_ok = false;
                    break;
                }
                else if(it_found->_height > max_height)
                    max_height = it_found->_height;
            }
            if(occurrences_r.size() == 1 && inputs_ok)
            {
                std::set<std::vector<std::set<state> > > possible_P1_to_Pn = possible_combinations_Pi(t.get_inputs(), safe_Processed);
                for(std::set<std::vector<std::set<state> > >::const_iterator it = possible_P1_to_Pn.begin(); it != possible_P1_to_Pn.end(); it++)
                {
                    std::set<int> occurrences_R;
                    for(int j = 0; j < it->size(); j++)
                        if((*it)[j] == R)
                            occurrences_R.insert(j);
                    bool have_same_index = false;
                    for(std::set<int>::const_iterator it2 = occurrences_r.begin(); it2 != occurrences_r.end(); it2++)
                        if(occurrences_R.find(*it2) != occurrences_R.end())
                        {
                            have_same_index = true;
                            break;
                        }
                    if(have_same_index)
                    {
                        state post_computed(t.get_output());
                        post_computed._height = max_height + 1;
                        result.insert(product_state(post_computed, B.post(t, *it)));
                    }
                }
            }
        }
        return result;
    }

    // optimization
    int optimize(const state & p,
                  const std::set<state> & P,
                  int d,
                  std::set<product_state> * Processed,
                  std::set<product_state> & Next) const
    {
        bool exist = false;
        for(int i = 0; i < d; i++)
        {
            product_state to_remove;
            bool has_to_remove = false;
            for(std::set<product_state>::const_iterator it = Processed[i].begin(); it != Processed[i].end(); it++)
            {
                if(p == it->_1 && is_subset(it->_2, P))
                {
                    exist = true;
                    has_to_remove = true;
                    product_state to_add(*it);
                    to_add._1._height = p._height;
                    Processed[d].insert(to_add);
                    to_remove = *it;
                    break;
                }
            }
            if(has_to_remove)
                Processed[i].erase(to_remove);
        }
        for(std::set<product_state>::const_iterator it = Processed[d].begin(); it != Processed[d].end(); it++)
        {
            state q = it->_1;
            std::set<state> Q = it->_2;
            if(p == q && is_subset(Q, P))
            {
                exist = true;
                break;
            }
        }
        if(exist)
            return 0;
        for(std::set<product_state>::const_iterator it = Next.begin(); it != Next.end(); it++)
        {
            state q = it->_1;
            std::set<state> Q = it->_2;
            if(p == q && is_subset(Q, P))
            {
                exist = true;
                break;
            }
        }
        if(exist)
            return 0;
        for(int i = 0; i < d; i++)
        {
            std::set<product_state> to_remove;
            for(std::set<product_state>::const_iterator it = Processed[i].begin(); it != Processed[i].end(); it++)
            {
                if(p == it->_1 && is_subset(P, it->_2))
                    to_remove.insert(*it);
            }
            for(std::set<product_state>::const_iterator it = to_remove.begin(); it != to_remove.end(); it++)
                Processed[i].erase(*it);
            to_remove.clear();
        }
        std::set<product_state> to_remove;
        for(std::set<product_state>::const_iterator it = Next.begin(); it != Next.end(); it++)
        {
            state q = it->_1;
            std::set<state> Q = it->_2;
            if(q == p && is_subset(P, Q))
                to_remove.insert(*it);
        }
        for(std::set<product_state>::const_iterator it = to_remove.begin(); it != to_remove.end(); it++)
            Next.erase(*it);
        return 1;
    }

    // check inclusion by giving a dimension
    bool is_included_in_with_dimension(const tree_automaton & B,
                                       int d,
                                       std::set<product_state> * Processed) const
    {
        std::set<product_state> Next;
        if(d == 0)
        {
            for(int i = 0; i < DELTA.size(); i++)
                if(DELTA[i].get_arity() == 0)
                {
                    product_state temp(DELTA[i].get_output(), B.initial_states(DELTA[i].get_name()));
                    if(accept(temp, B))
                        return false;
                    Next.insert(temp);
                }
        }
        else if(Processed[d - 1].size() > 0)
        {
            std::set<state> q_Processed;
            std::set<std::set<state> > Q_Processed;
            for(std::set<product_state>::const_iterator it = Processed[d - 1].begin(); it != Processed[d - 1].end(); it++)
            {
                q_Processed.insert(it->_1);
                Q_Processed.insert(it->_2);
            }
            for(int i = 0; i < DELTA.size(); i++)
                if(DELTA[i].get_arity() >= 2)
                {
                    int in_Processed_d_minus_1 = 0;
                    bool at_least_2_in_Processed_d_minus_1 = false;
                    for(int j = 0; j < DELTA[i].get_arity(); j++)
                    {
                        if(q_Processed.find(DELTA[i].get_input(j)) != q_Processed.end())
                        {
                            in_Processed_d_minus_1++;
                            if(in_Processed_d_minus_1 == 2)
                                at_least_2_in_Processed_d_minus_1 = true;
                        }
                        if(at_least_2_in_Processed_d_minus_1)
                            break;
                    }
                    if(at_least_2_in_Processed_d_minus_1)
                    {
                        std::set<product_state> safe_Processed;
                        std::set<state> safe_q_Processed;
                        for(int j = 0; j < d; j++)
                            for(std::set<product_state>::const_iterator it = Processed[j].begin(); it != Processed[j].end(); it++)
                            {
                                safe_Processed.insert(*it);
                                safe_q_Processed.insert(it->_1);
                            }
                        bool inputs_ok = true;
                        int max_height = 0;
                        for(int j = 0; j < DELTA[i].get_arity(); j++)
                        {
                            std::set<state>::const_iterator it_found = safe_q_Processed.find(DELTA[i].get_input(j));
                            if(it_found == safe_q_Processed.end())
                            {
                                inputs_ok = false;
                                break;
                            }
                            else if(it_found->_height > max_height)
                                max_height = it_found->_height;
                        }
                        if(!inputs_ok)
                            break;
                        state post_computed(DELTA[i].get_output());
                        post_computed._height = max_height + 1;
                        std::set<std::vector<std::set<state> > > possible_P1_to_Pn = possible_combinations_Pi(DELTA[i].get_inputs(), safe_Processed);
                        /*std::cout << DELTA[i].get_inputs() << std::endl;
                        std::cout << safe_Processed << std::endl;
                        std::cout << possible_P1_to_Pn << std::endl;*/
                        for(std::set<std::vector<std::set<state> > >::const_iterator it = possible_P1_to_Pn.begin(); it != possible_P1_to_Pn.end(); it++)
                        {
                            in_Processed_d_minus_1 = 0;
                            at_least_2_in_Processed_d_minus_1 = false;
                            for(int j = 0; j < it->size(); j++)
                            {
                                //std::cout << Q_Processed << std::endl;
                                //std::cout << (*it)[j] << std::endl;
                                if(Q_Processed.find((*it)[j]) != Q_Processed.end())
                                {
                                    in_Processed_d_minus_1++;
                                    if(in_Processed_d_minus_1 == 2)
                                        at_least_2_in_Processed_d_minus_1 = true;
                                }
                                if(at_least_2_in_Processed_d_minus_1)
                                    break;
                            }
                            if(at_least_2_in_Processed_d_minus_1)
                            {
                                product_state temp(post_computed, B.post(DELTA[i], *it));
                                if(accept(temp, B))
                                    return false;
                                Next.insert(temp);
                            }
                        }
                    }
                }
        }
        while(Next.size() > 0)
        {
            product_state rR = *(Next.begin());
            Processed[d].insert(rR);
            std::cout << rR._1 << std::endl;
            Next.erase(rR);
            if(d == 0)
            {
                for(int i = 0; i < DELTA.size(); i++)
                {
                    transition t = DELTA[i];
                    if(t.get_arity() != 1)
                        continue;
                    if(has_value(t.get_inputs(), rR._1))
                    {
                        state p = t.get_output();
                        p._height = rR._1._height + 1;
                        std::set<state> P;
                        for(std::set<state>::const_iterator it = rR._2.begin(); it != rR._2.end(); it++)
                        {
                            state Ri = *it;
                            for(int j = 0; j < B.DELTA.size(); j++)
                                if(B.DELTA[j].get_name() == t.get_name() && B.DELTA[j].get_arity() == t.get_arity() && has_value(B.DELTA[j].get_inputs(), Ri))
                                    P.insert(B.DELTA[j].get_output());
                        }
                        if(accept(product_state(p, P), B))
                            return false;
                        if(optimize(p, P, d, Processed, Next) != 0)
                            Next.insert(product_state(p, P));
                    }
                }
            }
            else
            {
                std::set<product_state> rR_post = post(rR, Processed, d, B);
                for(std::set<product_state>::const_iterator it = rR_post.begin(); it != rR_post.end(); it++)
                {
                    product_state pP = *it;
                    state p = pP._1;
                    std::set<state> P = pP._2;
                    if(accept(pP, B))
                        return false;
                    if(optimize(p, P, d, Processed, Next) != 0)
                        Next.insert(pP);
                }
            }
        }
        return true;
    }

    // determine if the language is included in the language of another automaton
    // dimension based
    bool is_included_in_dimension_based(const tree_automaton & B, int b) const
    {
        std::set<product_state> * Processed = new std::set<product_state>[b + 1];
        for(int i = 0; i <= b; i++)
        {
            if(is_included_in_with_dimension(B, i, Processed) == false)
            {
                delete[] Processed;
                std::cout << "Dimension = " << i << std::endl;
                return false;
            }
            /*for(int j = 0; j <= i; j++)
            {
                std::cout << "Processed " << j << ": ";
                for(std::set<product_state>::const_iterator it = Processed[j].begin(); it != Processed[j].end(); it++)
                    std::cout << *it << " ";
            }
            std::cout << Processed[i] << std::endl;
            std::cout << std::endl << i << "---" << std::endl;*/
        }
        delete[] Processed;
        return true;
    }

};

std::ostream & operator<<(std::ostream & out, const tree_automaton & ta)
{
    out << ta.to_string();
    return out;
}

#endif
