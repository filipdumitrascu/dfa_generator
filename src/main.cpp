#include "dfa.h"
#include "utils.h"

#include <fstream>

std::pair<std::vector<std::string>, std::vector<std::string>> read_languages()
{
    std::ifstream file("input.txt");
    DIE(!file, "Error opening input file!\n");

    size_t cntAccept{};
    size_t cntFail{};
    int lenString{};

    file >> cntAccept >> cntFail >> lenString;

    std::vector<std::string> accept(cntAccept);
    std::vector<std::string> fail(cntFail);

    for (size_t i = 0; i < cntAccept; ++i) {
        std::string word;
        file >> word;
        
        accept[i] = word;
    }

    for (size_t i = 0; i < cntFail; ++i) {
        std::string word;
        file >> word;

        fail[i] = word;
    }

    file.close();

    return {accept, fail};
}

int main()
{
    auto [accept, fail] = read_languages();

    DFA dfa;
    dfa.createTrie(accept);

    dfa.minimize();
    dfa.writeToFile("output.txt");

    return 0;
}
