#ifndef DFA_H
#define DFA_H

#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>

struct DfaState {
    int id;
    bool isEndOfWord;
    std::unordered_map<char, DfaState*> children;

    DfaState(int _id) : id(_id), isEndOfWord(false) {}
};

class DFA {
 private:
    int statesCount;
    DfaState* initialState;

    std::vector<DfaState*> allStates;
    std::vector<DfaState*> finalStates;
    std::unordered_set<char> alphabet;

    // Helper functions for Hopcroft
    std::vector<std::unordered_set<DfaState*>> splitPartition(
        const std::unordered_set<DfaState*>& Y, 
        const std::unordered_set<DfaState*>& X);

    void mergeEquivalentStates(
        const std::vector<std::unordered_set<DfaState*>>& P);

 public:
    DFA();
    ~DFA();

    // Insert letter in trie.
    void insertInTrie(const std::string& word);

    // Creates trie.
    void createTrie(std::vector<std::string>& words);

    // Hopcroft's DFA minimization algorithm.
    void minimize();

    // Outputs Dfa to file.
    void writeToFile(const std::string& filename) const;
};

#endif /* DFA_H */
