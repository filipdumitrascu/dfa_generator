#include "dfa.h"
#include "utils.h"

#include <fstream>
#include <algorithm>

DFA::DFA() {
    initialState = new DfaState(1);
    statesCount = 1;

    allStates.push_back(initialState);
}

DFA::~DFA() {
    for (DfaState* state : allStates) {
        delete state;
    }
}

void DFA::insertInTrie(const std::string& word) {
    DfaState* current = initialState;
    for (char c : word) {
        alphabet.insert(c);

        if (current->children.find(c) == current->children.end()) {
            DfaState* newState = new DfaState(++statesCount);
            current->children[c] = newState;
            allStates.push_back(newState);
        }

        current = current->children[c];
    }

    current->isEndOfWord = true;
    finalStates.push_back(current);
}

void DFA::createTrie(std::vector<std::string>& words) {
    for (const std::string& word : words) {
        insertInTrie(word);
    }
}

std::vector<std::unordered_set<DfaState*>> DFA::splitPartition(
    const std::unordered_set<DfaState*>& Y, 
    const std::unordered_set<DfaState*>& X) {
    
    std::unordered_set<DfaState*> intersection;
    std::unordered_set<DfaState*> difference;
    
    for (DfaState* state : Y) {
        if (X.find(state) != X.end()) {
            intersection.insert(state);
        } else {
            difference.insert(state);
        }
    }
    
    return {std::move(intersection), std::move(difference)};
}

void DFA::minimize() {
    std::unordered_set<DfaState*> finalStatesSet(finalStates.begin(), finalStates.end());
    std::unordered_set<DfaState*> nonFinalStatesSet;
    
    for (DfaState* state : allStates) {
        if (!state->isEndOfWord) {
            nonFinalStatesSet.insert(state);
        }
    }
    
    std::vector<std::unordered_set<DfaState*>> P = {finalStatesSet, nonFinalStatesSet};
    std::vector<std::unordered_set<DfaState*>> W = {finalStatesSet, nonFinalStatesSet};
    
    while (!W.empty()) {
        std::unordered_set<DfaState*> A = std::move(W.back());
        W.pop_back();
        
        for (char c : alphabet) {
            // the set of states for which a transition on c leads to a state in A
            std::unordered_set<DfaState*> X;

            for (DfaState* state : allStates) {
                auto it = state->children.find(c);
                if (it != state->children.end() && A.find(it->second) != A.end()) {
                    X.insert(state);
                }
            }
            
            std::vector<std::unordered_set<DfaState*>> newP;
            
            for (const auto& Y : P) {
                auto splits = splitPartition(Y, X);
                if (!splits[0].empty() && !splits[1].empty()) { // intersec and diff
                    newP.push_back(std::move(splits[0]));
                    newP.push_back(std::move(splits[1]));
                    
                    auto wIt = std::find(W.begin(), W.end(), Y);
                    if (wIt != W.end()) {
                        W.erase(wIt);
                        W.push_back(std::move(splits[0]));
                        W.push_back(std::move(splits[1]));

                    } else {
                        W.push_back(splits[0].size() <= splits[1].size() ? 
                                  std::move(splits[0]) : std::move(splits[1]));
                    }
                } else {
                    newP.push_back(Y);
                }
            }

            P = std::move(newP);
        }
    }

    mergeEquivalentStates(P);
}

DfaState* DFA::findRepresentativeState(DfaState* state, 
                                     const std::vector<std::unordered_set<DfaState*>>& P) {
    for (const auto& partition : P) {
        if (partition.find(state) != partition.end()) {
            return *partition.begin();  // Return the first state in the partition
        }
    }
    return state;
}

void DFA::mergeEquivalentStates(const std::vector<std::unordered_set<DfaState*>>& P) {
    // Create mapping from states to their representatives
    std::unordered_map<DfaState*, DfaState*> stateMapping;
    for (DfaState* state : allStates) {
        stateMapping[state] = findRepresentativeState(state, P);
    }
    
    // Update transitions to use representative states
    for (DfaState* state : allStates) {
        std::unordered_map<char, DfaState*> newChildren;
        for (const auto& transition : state->children) {
            newChildren[transition.first] = stateMapping[transition.second];
        }
        state->children = newChildren;
    }
    
    // Clean up non-representative states and build new state list
    std::unordered_set<DfaState*> toDelete;
    std::vector<DfaState*> newAllStates;
    std::vector<DfaState*> newFinalStates;
    
    for (DfaState* state : allStates) {
        if (stateMapping[state] != state) {
            // If this is not a representative state, mark for deletion
            toDelete.insert(state);
        } else {
            // This is a representative state - keep it
            newAllStates.push_back(state);
            if (state->isEndOfWord) {
                newFinalStates.push_back(state);
            }
        }
    }
    
    // Update the initial state
    initialState = stateMapping[initialState];
    
    // Delete non-representative states
    for (DfaState* state : toDelete) {
        delete state;
    }
    
    // Update class fields
    allStates = std::move(newAllStates);
    finalStates = std::move(newFinalStates);
    statesCount = allStates.size();
    
    // Reassign IDs sequentially
    for (size_t i = 0; i < allStates.size(); i++) {
        allStates[i]->id = i;
    }
}

void DFA::writeToFile(const std::string &filename) const
{
    std::ofstream file(filename);
    DIE(!file, "Error opening output file!\n");

    file << "dfa\n" << statesCount + 1 << " " << finalStates.size() << " "
         << initialState->id << '\n';

    for (size_t i = 0; i < finalStates.size(); ++i) {
        i == finalStates.size() - 1 ? file << finalStates[i]->id << '\n' :
            file << finalStates[i]->id << " ";
    }

    int dummyState{};

    for (int i = 0; i < 26; ++i) {
        i == 25 ? file << dummyState << "\n" :
            file << dummyState << " ";
    }

    for (const auto& state : allStates) {
        for (char c = 'a'; c <= 'z'; ++c) {
            if (state->children.find(c) != state->children.end()) {
                file << state->children[c]->id;                

            } else {
                file << dummyState;
            }

            c == 'z' ? file << "\n" : file << " "; 
        }
    }

    file.close();
}
