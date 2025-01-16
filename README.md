# Deterministic finite automaton (DFA)
## Dumitrascu Filip-Teodor

## Implementation steps

1. Generate a Trie containing all of the input strings. This is a tree structure
where each edge represents a single character. This removes redundancies at the
start of the strings, but common branches further down are not merged.

2. A trie can be seen as a tree-shaped deterministic finite automaton (DFA), so
DFA algorithms can be applied. In this case, we apply Hopcroft's DFA
minimization algorithm to merge the nondistinguishable states.
