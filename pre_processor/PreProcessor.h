//
// Created by Alienson on 5.2.2024..
//

#ifndef WINTER_PREPROCESSOR_H
#define WINTER_PREPROCESSOR_H

#include <fstream>
#include <regex>
#include <vector>

class PreProcessor {
public:
    void process(std::ifstream &inputFile, std::ofstream &outputFile);
private:
    //Support for = ([a-zA-Z]+::)?([a-zA-Z_][a-zA-Z0-9_]*?)\s*(<.+?>)?\s*[\*&\s(const?)]+?\s*([a-zA-Z_][a-zA-Z0-9_]*)\s*(=\s*.+)?\s*(;|,)
    //TODO will catch syntactically inaccurate declarations (int & const * const & x;)
    std::regex fieldDeclarationRegex{R"(([a-zA-Z]+::)?([a-zA-Z_][a-zA-Z0-9_]*?)\s*(<.+?>)?\s*[\*&\s(const?)]+?\s*([a-zA-Z_][a-zA-Z0-9_]*)\s*(;|,))"};
    std::smatch match;
    std::string line;
    bool declaringMethod;
    bool declaringClass;
    bool hasSpace;
    int bracketCounter;
    int smallBracketCounter;
};


#endif //WINTER_PREPROCESSOR_H
