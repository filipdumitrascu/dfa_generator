# Deterministic finite automaton (DFA)
## Dumitrascu Filip-Teodor

## Implementation steps

1. Se genereaza trie-ul si se marcheaza 2 flaguri din cadrul fiecarui nod
ce determina daca din nodul respectiv se pot obtine doar cuvinte acceptate,
cuvinte respinse sau de ambele tipuri. 

2. Se reduce numarul de stari ale trie-ului pastrand doar:
- starile ce pot duce cuvantul catre respins sau acceptat (nu doar catre una din ele)
- starea initiala
- 2 stari inlocuitoare, una finala si una care respinge tot
Practic trie-ul a devenit primul dfa evident.

3. Urmatoarea minimizare consta in combinarea starilor echivalente:
Daca doua stari au tranzitii identice catre aceleasi stari (nu doar catre
rejectState si finalState), acestea pot fi consolidate intr-o singura stare.
Se repeta pana cand numarul de stari scade.

4. Pentru un numar de stari mai mic decat cel de referinta, am incercat algorimti
precum hopcroft sau brzozowski dar mai optim de atat nu au reusit nici ei.
