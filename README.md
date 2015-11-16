# ***TA***

## C++ library for efficient manipulation with Tree Automata - Created by `Xiao XU`

##### For any question, please contact `xiao.xu.cathiec@gmail.com`.

#### Creation of Tree Automata
> Create a tree automaton according to a file
* Example: create a tree automaton "A0" by reading the definition in a texte file "A0.txt":  
        <pre><code>tree_automaton A0("A0.txt");</code></pre>

> Create a union tree automaton of 2 tree automata
* Example: create a union tree automaton "A0UA1" of 2 automata "A0" and "A1":  
        <pre><code>tree_automaton A0UA1 = A0.U(A1);</code></pre>

> Create an intersection tree automaton of 2 tree automata
* Example: create an intersection tree automaton "A0IA1" of 2 automata "A0" and "A1":  
        <pre><code>tree_automaton A0IA1 = A0.I(A1);</code></pre>

#### Language Inclusion of Tree Automata
> Check if the language of a tree automaton is included in the language of another tree automaton
* Example: check if the language of tree automaton "A0" is included in the language of tree automaton "A1":  
	<pre><code>if(A0.inclusion_check(A1))
	    cout << "Included" << endl;
	else
	    cout << "Not included" << endl;</code></pre>

> Check if the language of a tree automaton is included in the language of another tree automaton with restriction of maximum tree dimension
* Example: check if the language of tree automaton "A0" is included in the language of tree automaton "A1" with restriction of maximum 5 tree dimension:  
	<pre><code>if(A0.inclusion_check(5, A1))
	    cout << "Included" << endl;
	else
	    cout << "Not included" << endl;</code></pre>

#### Maximum Dimension of Tree Automata
> Calculate the maximum dimension of a tree automaton
* Example: calculate the maximum dimension of a tree automaton "A0":
        <pre><code>cout << A0.max_dimension() << endl;</code></pre>