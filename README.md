# Deterministic finite automaton (DFA)
#### Contributor: Dumitrascu Filip-Teodor

## Implementation steps

1. Generate the trie and mark 2 flags within each node that determine whether
only accepted words can be obtained from that node, rejected words or both. 

2. Reduce the number of trie states by keeping only:
- the states that can lead the word to rejected or accepted (not just one of them)
- initial state
- 2 replacement states, one final and one that rejects everything
Basically the trie became the first obvious dfa.

3. The next minimization consists in combining equivalent states:
If two states have identical transitions to the same states (not just to
rejectState and finalState), they can be consolidated into a single state.
Repeat until the number of states decreases.

4. For a smaller number of states than the reference, have been tried algorithms
such as hopcroft or brzozowski but even more optimally they failed.
