#ifndef _tree_automaton_h
#define _tree_automaton_h

#include "transition.h"
#include "product_state.h"

class tree_automaton
{

variables:

    str name;

    vec<transition> I;

    vec<str> F;

    int max_arity;

    hmp<int, vec<transition> > DELTA;

    hmp<str, vec<transition>, hash_str> RMAP;

functions:

    tree_automaton(const str & file_name)
    {
        max_arity = 0;
        ifs file(file_name);
        str temp;
        while(file >> temp)
            if(temp == "Automaton")
                break;
        file >> name;
        while(file >> temp)
            if(temp == "Final")
                break;
        file >> temp;
        while(file >> temp)
            if(temp != "Transitions")
                F.push_back(temp);
            else
                break;
        char c;
        int step = 0;
        temp = "";
        transition t;
        bool no_argument = false;
        while(file >> c)
        {
            if(step == 0)
            {
                if(c != '-' && c != '(')
                {
                    temp += c;
                }
                else if(c == '(')
                {
                    step = 1;
                    t.symbol = temp;
                }
                else if(c == '-')
                {
                    step = 2;
                    t.symbol = temp;
                    no_argument = true;
                }
            }
            else if(step == 1)
            {
                if(c == ')')
                {
                    file >> c;
                    step = 2;
                    no_argument = true;
                }
                else
                {
                    temp = c;
                    while(file >> c)
                    {
                        if(c == ')')
                        {
                            file >> c;
                            step = 2;
                            t.input.push_back(temp);
                            break;
                        }
                        else if(c == ',')
                        {
                            t.input.push_back(temp);
                            temp = "";
                        }
                        else
                        {
                            temp += c;
                        }
                    }
                }
            }
            else if(step == 2)
            {
                file >> temp;
                t.output = temp;
                if(no_argument)
                    I.push_back(t);
                else
                {
                    for(int i = 0; i < t.input.size(); i++)
                    {
                        bool already = false;
                        for(int j = 0; j < i; j++)
                            if(t.input[i] == t.input[j])
                            {
                                already = true;
                                break;
                            }
                        if(!already)
                            RMAP[t.input[i]].push_back(t);
                    }
                }
                if(t.input.size() > max_arity)
                    max_arity = t.input.size();
                DELTA[t.input.size()].push_back(t);
                t.input.clear();
                no_argument = false;
                temp = "";
                step = 0;
            }
        }
        file.close();
    }

    ~tree_automaton()
    {}

    str to_string() const
    {
        str result = "Ops\nAutomaton ";
        result += name;
        result += "\nStates\nFinal States ";
        for(int i = 0; i < F.size(); i++)
        {
            result += F[i];
            result += ' ';
        }
        result += "\nTransitions\n";
        for(int i = 0; i <= max_arity; i++)
            if(DELTA.find(i) != DELTA.end())
                for(int j = 0; j < DELTA.find(i)->second.size(); j++)
                {
                    result += DELTA.find(i)->second[j].to_string();
                    result += '\n';
                }
        return result;
    }

    void save_into_file(const str & file_name) const
    {
        ofs file(file_name);
        file << to_string();
        file.close();
    }

    bool accept(const product_state & ps, const tree_automaton & B) const
    {
        if(!exist_in(ps.left, F))
            return false;
        for(int i = 0; i < ps.right.size(); i++)
            if(exist_in(ps.right[i], B.F))
                return false;
        return true;
    }

    vec<str> post(const transition & t, const vec<vec<str> > & cb_Qi) const
    {
        vec<str> result;
        if(t.input.size() != cb_Qi.size())
        {
            println("###");
            return result;
        }
        if(DELTA.find(t.input.size()) == DELTA.end())
            return result;
        const vec<transition> * pt_possible_transitions = &(DELTA.find(t.input.size())->second);
        for(int i = 0; i < pt_possible_transitions->size(); i++)
        {
            transition t2 = (*pt_possible_transitions)[i];
            if(t2.symbol == t.symbol)
            {
                bool ok = true;
                for(int j = 0; j < t2.input.size(); j++)
                {
                    if(!exist_in(t2.input[j], cb_Qi[j]))
                    {
                        ok = false;
                        break;
                    }
                }
                if(ok)
                    result.push_back(t2.output);
            }
        }
        return result;
    }

    vec<vec<str> > possible_Qi(const str & qi, const vec<product_state> & Processed) const
    {
        vec<vec<str> > result;
        for(int i = 0; i < Processed.size(); i++)
            if(Processed[i].left == qi)
            {
                if(!exist_in(Processed[i].right, result))
                    result.push_back(Processed[i].right);
            }
        return result;
    }

    vec<vec<vec<str> > > combinations_Qi(const vec<str> & cb_qi,
                                         const vec<product_state> & Processed) const
    {
        vec<vec<vec<str> > > result;
        if(cb_qi.size() > 1)
        {
            vec<str> next_recursion = cb_qi;
            next_recursion.erase(next_recursion.end() - 1);
            str qn = cb_qi[cb_qi.size() - 1];
            vec<vec<str> > possible_Qn = possible_Qi(qn, Processed);
            vec<vec<vec<str> > > combinations_Q1_to_Qn_minus_1 = combinations_Qi(next_recursion, Processed);
            for(int i = 0; i < combinations_Q1_to_Qn_minus_1.size(); i++)
                for(int j = 0; j < possible_Qn.size(); j++)
                {
                    vec<vec<str> > cb_Qi = combinations_Q1_to_Qn_minus_1[i];
                    vec<str> Qn = possible_Qn[j];
                    cb_Qi.push_back(Qn);
                    result.push_back(cb_Qi);
                }
        }
        else
        {
            str q1 = cb_qi[0];
            vec<vec<str> > possible_Q1 = possible_Qi(q1, Processed);
            for(int i = 0; i < possible_Q1.size(); i++)
            {
                vec<vec<str> > cb_Q1;
                cb_Q1.push_back(possible_Q1[i]);
                result.push_back(cb_Q1);
            }
        }
        return result;
    }

    vec<product_state> post(const product_state & rR,
                            const vec<product_state> & Processed,
                            const tree_automaton & B) const
    {
        vec<product_state> result;
        str r = rR.left;
        vec<str> q_Processed;
        for(int i = 0; i < Processed.size(); i++)
            if(!exist_in(Processed[i].left, q_Processed))
                q_Processed.push_back(Processed[i].left);
        if(RMAP.find(r) == RMAP.end())
            return result;
        for(int i = 0; i < RMAP.find(r)->second.size(); i++)
        {
            transition t = RMAP.find(r)->second[i];
            set<int> occurrences_r;
            for(int j = 0; j < t.input.size(); j++)
                if(t.input[j] == r)
                    occurrences_r.insert(j);
            bool ok = true;
            for(int j = 0; j < t.input.size(); j++)
                if(!exist_in(t.input[j], q_Processed))
                {
                    ok = false;
                    break;
                }
            if(ok)
            {
                vec<vec<vec<str> > > possible_Q1_to_Qn = combinations_Qi(t.input, Processed);
                for(int j = 0; j < possible_Q1_to_Qn.size(); j++)
                {
                    set<int> occurrences_R;
                    for(int k = 0; k < possible_Q1_to_Qn[j].size(); k++)
                        if(rR.right.size() == possible_Q1_to_Qn[j][k].size()
                                && is_subset(rR.right, possible_Q1_to_Qn[j][k]))
                            occurrences_R.insert(k);
                    bool have_one_occurence_rR = false;
                    for(set<int>::const_iterator it = occurrences_r.begin(); it != occurrences_r.end(); it++)
                        if(occurrences_R.find(*it) != occurrences_R.end())
                        {
                            have_one_occurence_rR = true;
                            break;
                        }
                    if(have_one_occurence_rR)
                    {
                        vec<str> P = B.post(t, possible_Q1_to_Qn[j]);
                        product_state pP(t.output, P);
                        if(!exist_in(pP, result))
                            result.push_back(pP);
                    }
                }
            }
        }
        return result;
    }

    bool classical_antichains(const tree_automaton & B) const
    {
        vec<product_state> Processed;
        vec<product_state> Next;
        for(int i = 0; i < I.size(); i++)
        {
            vec<str> IaB;
            for(int j = 0; j < B.I.size(); j++)
                if(B.I[j].symbol == I[i].symbol)
                    IaB.push_back(B.I[j].output);
            product_state i_IaB(I[i].output, IaB);
            if(accept(i_IaB, B))
                return false;
            if(!exist_in(i_IaB, Next))
                Next.push_back(i_IaB);
        }
        while(Next.size() > 0)
        {
            product_state rR = *(Next.begin());
            Next.erase(Next.begin());
            if(!exist_in(rR, Processed))
                Processed.push_back(rR);
            vec<product_state> rR_post = post(rR, Processed, B);
            for(int i = 0; i < rR_post.size(); i++)
            {
                if(accept(rR_post[i], B))
                    return false;
                str p = rR_post[i].left;
                vec<str> P = rR_post[i].right;
                bool exist = false;
                for(int j = 0; j < Processed.size(); j++)
                {
                    if(p == Processed[j].left && is_subset(Processed[j].right, P))
                    {
                        exist = true;
                        break;
                    }
                }
                if(exist)
                    continue;
                for(int j = 0; j < Next.size(); j++)
                    if(p == Next[j].left && is_subset(Next[j].right, P))
                    {
                        exist = true;
                        break;
                    }
                if(exist)
                    continue;
                vec<product_state> to_keep;
                for(int j = 0; j < Processed.size(); j++)
                    if(!(Processed[j].left == p && is_subset(P, Processed[j].right)))
                        to_keep.push_back(Processed[j]);
                Processed = to_keep;
                to_keep.clear();
                for(int j = 0; j < Next.size(); j++)
                    if(!(Next[j].left == p && is_subset(P, Next[j].right)))
                        to_keep.push_back(Next[j]);
                Next = to_keep;
                Next.push_back(rR_post[i]);
            }
        }
        return true;
    }

};

os & operator<<(os & out, const tree_automaton & ta)
{
    out << ta.to_string();
    return out;
}

#endif
