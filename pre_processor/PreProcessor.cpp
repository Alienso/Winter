//
// Created by Alienson on 5.2.2024..
//

#include "PreProcessor.h"

void PreProcessor::process(std::ifstream &inputFile, std::ofstream &outputFile, std::string& fileName) {

    for (auto pass : passes){
        pass->begin();
    }

    while (std::getline(inputFile, line)) {
        for(auto pass : passes){
            if (pass->shouldProcess(fileName))
                pass->process(inputFile, outputFile, line, lastLine);
        }
        outputFile << line << std::endl;
        lastLine = line;
    }

    for (auto pass : passes){
        pass->end(inputFile,outputFile);
    }
}

void PreProcessor::addPass(Pass *pass) {
    passes.push_back(pass);
}
