#ifndef _tree_automaton_h
#define _tree_automaton_h

#include "transition.h"

class tree_automaton
{

variables:

    str name;

    vec<transition> I;

    vec<str> F;

    vec<transition> DELTA;

    hmp<str, vec<transition>, hash_str> R;

functions:

    tree_automaton(str file_name)
    {
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
                            R[t.input[i]].push_back(t);
                    }
                }
                DELTA.push_back(t);
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
        for(int i = 0; i < DELTA.size(); i++)
        {
            result += DELTA[i].to_string();
            result += '\n';
        }
        return result;
    }

    void save_into_file(str file_name) const
    {
        ofs file(file_name);
        file << to_string();
        file.close();
    }

};

os & operator<<(os & out, const tree_automaton & ta)
{
    out << ta.to_string();
    return out;
}

#endif
