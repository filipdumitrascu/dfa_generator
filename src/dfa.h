#ifndef DFA_H
#define DFA_H

#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>

enum WordStatus{
    ACCEPT,
    REJECT
};

struct DfaState {
    int id;
    bool canFindAccepted;
    bool canFindFailed;
    std::unordered_map<char, DfaState*> children;

    DfaState(bool _canFindAccepted, bool _canFindFalse) :
        id(-1), canFindAccepted(_canFindAccepted), canFindFailed(_canFindFalse) {}
};

class DFA {
 private:
    int statesCount;
    DfaState* initialState;

    std::vector<DfaState*> allStates;

    void processTrie();

 public:
    DFA();

    // Insert letter in trie.
    void insertInTrie(const std::string& word, WordStatus status);

    // Creates trie.
    void createTrie(std::vector<std::string>& accept, std::vector<std::string>& fail);

    // Outputs Dfa to file.
    void writeToFile(const std::string& filename) const;
};

#endif /* DFA_H */
