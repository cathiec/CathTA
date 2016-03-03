#ifndef _tree_automaton_h
#define _tree_automaton_h

#include "transition.h"
#include "symbol.h"
#include <set>
#include <fstream>

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

};

std::ostream & operator<<(std::ostream & out, const tree_automaton & ta)
{
    out << ta.to_string();
    return out;
}

#endif
