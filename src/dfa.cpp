#include "dfa.h"

#include <fstream>
#include <vector>
#include <string>

int main(int argc, char const *argv[])
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
        
        accept.push_back(word);
    }

    for (size_t i = 0; i < cntFail; ++i) {
        std::string word;
        file >> word;

        fail.push_back(word);
    }

    file.close();

    return 0;
}
