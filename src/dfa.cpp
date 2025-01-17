#include "dfa.h"

#include <iostream>
#include <fstream>
#include <algorithm>

DFA::DFA() {
    initialState = new DfaState(0);
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
            DfaState* newState = new DfaState(statesCount++);
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
    
    std::vector<std::unordered_set<DfaState*>> result;
    // Only return non-empty sets
    if (!intersection.empty()) result.push_back(std::move(intersection));
    if (!difference.empty()) result.push_back(std::move(difference));
    return result;
}

void DFA::minimize() {
    // Step 1: Initialize partitions P and waiting set W
    std::unordered_set<DfaState*> finalStatesSet;
    std::unordered_set<DfaState*> nonFinalStatesSet;
    
    for (DfaState* state : allStates) {
        if (state->isEndOfWord) {
            finalStatesSet.insert(state);
        } else {
            nonFinalStatesSet.insert(state);
        }
    }
    
    std::vector<std::unordered_set<DfaState*>> P;
    if (!finalStatesSet.empty()) P.push_back(finalStatesSet);
    if (!nonFinalStatesSet.empty()) P.push_back(nonFinalStatesSet);
    
    std::vector<std::unordered_set<DfaState*>> W = P;
    
    while (!W.empty()) {
        std::unordered_set<DfaState*> A = W.back();
        W.pop_back();
        
        for (char c : alphabet) {
            // Find states that can reach A with symbol c
            std::unordered_set<DfaState*> X;
            for (DfaState* state : allStates) {
                auto it = state->children.find(c);
                if (it != state->children.end() && A.find(it->second) != A.end()) {
                    X.insert(state);
                }
            }
            
            std::vector<std::unordered_set<DfaState*>> newP;
            bool partitionChanged = false;
            
            for (const auto& Y : P) {
                auto splits = splitPartition(Y, X);
                if (splits.size() > 1) { // Split occurred
                    partitionChanged = true;
                    for (auto& split : splits) {
                        newP.push_back(split);
                        
                        // Update waiting set W
                        auto wIt = std::find(W.begin(), W.end(), Y);
                        if (wIt != W.end()) {
                            // If Y is in W, replace it with both splits
                            W.erase(wIt);
                            W.push_back(splits[0]);
                            W.push_back(splits[1]);
                        } else {
                            // Add smaller split to W
                            if (splits[0].size() <= splits[1].size()) {
                                W.push_back(splits[0]);
                            } else {
                                W.push_back(splits[1]);
                            }
                        }
                    }
                } else {
                    newP.push_back(Y);
                }
            }
            
            if (partitionChanged) {
                P = std::move(newP);
            }
        }
    }
    
    mergeEquivalentStates(P);
}

void DFA::mergeEquivalentStates(const std::vector<std::unordered_set<DfaState*>>& P) {
    // Create mapping from states to their representatives
    std::unordered_map<DfaState*, DfaState*> stateMapping;
    for (const auto& partition : P) {
        DfaState* rep = *partition.begin();
        for (DfaState* state : partition) {
            stateMapping[state] = rep;
        }
    }
    
    // Create new states list with only representatives
    std::vector<DfaState*> newAllStates;
    std::vector<DfaState*> newFinalStates;
    std::unordered_set<DfaState*> representatives;
    
    for (const auto& partition : P) {
        DfaState* rep = *partition.begin();
        representatives.insert(rep);
        newAllStates.push_back(rep);
        if (rep->isEndOfWord) {
            newFinalStates.push_back(rep);
        }
    }
    
    // Update transitions using the mapping
    for (DfaState* state : representatives) {
        std::unordered_map<char, DfaState*> newChildren;
        for (const auto& [symbol, target] : state->children) {
            newChildren[symbol] = stateMapping[target];
        }
        state->children = std::move(newChildren);
    }
    
    // Update initial state
    initialState = stateMapping[initialState];
    
    // Delete non-representative states
    for (DfaState* state : allStates) {
        if (representatives.find(state) == representatives.end()) {
            delete state;
        }
    }
    
    // Update class members
    allStates = std::move(newAllStates);
    finalStates = std::move(newFinalStates);
    statesCount = allStates.size();
    
    // Reassign IDs
    for (size_t i = 0; i < allStates.size(); i++) {
        allStates[i]->id = i;
    }
}

void DFA::writeToFile(const std::string &filename) const
{
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could't open output file.\n";
        exit(1);
    }

    file << "dfa\n" << statesCount + 1 << " " << finalStates.size() << " "
         << initialState->id << '\n';

    for (size_t i = 0; i < finalStates.size(); ++i) {
        i == finalStates.size() - 1 ? file << finalStates[i]->id << '\n' :
            file << finalStates[i]->id << " ";
    }

    int dummyState{ statesCount };

    for (const auto& state : allStates) {
        for (char c = 'a'; c <= 'z'; ++c) {
            if (state->isEndOfWord) {
                file << state->id;
            
            } else if (state->children.find(c) != state->children.end()) {
                file << state->children[c]->id;                

            } else {
                file << dummyState;
            }

            c == 'z' ? file << "\n" : file << " "; 
        }
    }


    for (int i = 0; i < 26; ++i) {
        i == 25 ? file << dummyState << "\n" :
            file << dummyState << " ";
    }

    file.close();
}
