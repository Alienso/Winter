//
// Created by Alienson on 5.2.2024.
//

#ifndef WINTER_PREPROCESSOR_H
#define WINTER_PREPROCESSOR_H

#include <fstream>
#include <regex>
#include <vector>
#include "pass/Pass.h"

class PreProcessor {
public:
    void process(std::ifstream &inputFile, std::ofstream &outputFile, std::string& inputFileName, std::string& outputFileName);
    void addPass(Pass* pass);
    void finish();

private:
    std::string line;
    std::string lastLine;
    std::vector<Pass*> passes;
};


#endif //WINTER_PREPROCESSOR_H
