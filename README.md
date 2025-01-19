# Deterministic finite automaton (DFA)
## Dumitrascu Filip-Teodor

## Implementation steps

1. Se genereaza trie-ul si se marcheaza 2 flaguri din cadrul fiecarui nod
ce determina daca din nodul respectiv se pot obtine doar cuvinte acceptate,
cuvinte respinse sau de ambele tipuri. 

2. Se reduce numarul de stari ale trie-ului pastrandu-le doar pe cele ce pot 
duce cuvantul catre respins sau acceptat simultan, starea initiala, si 2 stari
inlocuitoare, una finala si una care respinge tot. Practic trie-ul a devenit
primul cel mai usor de implementat dfa.


