# Deterministic finite automaton (DFA)
## Dumitrascu Filip-Teodor

## Implementation steps

1. Generate a `Trie` containing all of the input strings. This is a tree structure
where each edge represents a single character. This removes redundancies at the
start of the strings, but common branches further down are not merged.

2. A trie can be seen as a tree-shaped deterministic finite automaton (DFA) (if the
missing tansitions are added). `Hopcroft's DFA minimization algorithm` is applied
to merge the nondistinguishable states and have a less memory dfa stored. 
