#include "dfa.h"

#include <iostream>
#include <fstream>
#include <queue>

DFA::DFA()
{
    DfaState* rejectState = new DfaState(false, true);
    rejectState->id = 0;
    allStates.push_back(rejectState);

    DfaState* finalState = new DfaState(true, false);
    finalState->id = 1;
    allStates.push_back(finalState);

    initialState = new DfaState(true, true);
    initialState->id = 2;
    allStates.push_back(initialState);

    statesCount = 3;
}

void DFA::insertInTrie(const std::string& word, WordStatus status)
{
    DfaState* current = initialState;
    for (char c : word) {
        if (current->children.find(c) == current->children.end()) {
            current->children[c] = new DfaState(false, false);
        }

        current = current->children[c];

        status == ACCEPT ? current->canFindAccepted = true : 
            current->canFindFailed = true;
    }
}

void DFA::processTrie() 
{
    // Assigning Id's for the important states
    std::queue<DfaState*> queue;
    std::unordered_set<DfaState*> visited;
    queue.push(initialState);
    
    while (!queue.empty()) {
        DfaState* current = queue.front();
        queue.pop();
        
        if (visited.count(current)) {
            continue;
        }

        visited.insert(current);
        
        for (auto& pair : current->children) {
            DfaState* child = pair.second;
            if (child->canFindAccepted && child->canFindFailed && child->id == -1) {
                child->id = statesCount++;
                allStates.push_back(child);
            }
            queue.push(child);
        }
    }

    // Cleanup
    visited.clear();
    queue.push(initialState);
    
    while (!queue.empty()) {
        DfaState* current = queue.front();
        queue.pop();
        
        if (visited.count(current)) {
            continue;
        }
        visited.insert(current);
        
        std::vector<char> chars_to_process;
        for (const auto& pair : current->children) {
            chars_to_process.push_back(pair.first);
        }

        for (char c : chars_to_process) {
            DfaState* child = current->children[c];
            
            if (child->id == -1) {
                if (child->canFindAccepted) {
                    current->children[c] = allStates[1];
                } else {
                    current->children[c] = allStates[0];
                }
                
                delete child;
            } else {
                queue.push(child);
            }
        }
    }
}

void DFA::createTrie(std::vector<std::string>& accept, std::vector<std::string>& fail)
{
    for (const std::string& word : accept) {
        insertInTrie(word, ACCEPT);
    }

    for (const std::string& word : fail) {
        insertInTrie(word, REJECT);
    }

    processTrie();
}

void DFA::writeToFile(const std::string &filename) const
{
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could't open output file.\n";
        exit(1);
    }

    /* only one final state is required and the initial is always 2
    two more states for the one which accepts and the one which rejects. */
    file << "dfa\n" << allStates.size() << " 1 2\n";

    file << "1\n";

    for (const auto& state : allStates) {
        for (char c = 'a'; c <= 'z'; ++c) {
            if (state->children.find(c) != state->children.end()) {
                if (state->children[c]->canFindAccepted && state->children[c]->canFindFailed) {
                    file << state->children[c]->id;
                
                } else if (state->children[c]->canFindAccepted) {
                    file << "1";

                } else if (state->children[c]->canFindFailed) {
                    file << "0";
                }

            } else {
                state->id == 1 ? file << "1" : file << "0";
            }

            c == 'z' ? file << "\n" : file << " "; 
        }
    }

    file.close();
}
