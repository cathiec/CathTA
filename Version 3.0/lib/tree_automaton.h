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
            return true;
        else
            return false;
    }

    // compute the initial states for a given symbol
    std::set<state> initial_states(std::string s) const
    {
        std::set<state> result;
        for(int i = 0; i < DELTA.size(); i++)
            if(DELTA[i].get_name() == s)
                result.insert(DELTA[i].get_output());
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
                    if(cb[j].find(tt.get_input(j)) == cb[j].end())
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
    bool is_included_in(const tree_automaton & B) const
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

};

std::ostream & operator<<(std::ostream & out, const tree_automaton & ta)
{
    out << ta.to_string();
    return out;
}

#endif
