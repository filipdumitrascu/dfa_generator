#include "dfa.h"

#include <iostream>
#include <fstream>

std::pair<std::vector<std::string>, std::vector<std::string>> read_languages()
{
    std::ifstream file("input.txt");
    if (!file.is_open()) {
        std::cerr << "Error: Could't open input file.\n";
        exit(1);    
    }

    size_t cntAccept{};
    size_t cntFail{};
    int lenString{};

    file >> cntAccept >> cntFail >> lenString;

    std::vector<std::string> accept(cntAccept);
    std::vector<std::string> fail(cntFail);

    for (size_t i = 0; i < cntAccept; ++i) {
        file >> accept[i];
    }

    for (size_t i = 0; i < cntFail; ++i) {
        file >> fail[i];
    }

    file.close();

    return {accept, fail};
}

int main()
{
    auto [accept, fail] = read_languages();

    DFA dfa;
    dfa.createTrie(accept, fail);

    dfa.minimize();
    dfa.writeToFile("output.txt");

    return 0;
}
