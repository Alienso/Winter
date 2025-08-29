//
// Created by Alienson on 5.2.2024.
//

#include "PreProcessor.h"

void PreProcessor::process(std::ifstream &inputFile, std::ofstream &outputFile, std::string& fileName) {

    std::vector<Pass*> requiredPasses;
    for (auto pass : passes){
        if (pass->shouldProcess(fileName))
            requiredPasses.push_back(pass);
    }

    for (auto pass : requiredPasses){
        pass->begin(fileName);
    }

    bool lineConsumed = false;
    while (std::getline(inputFile, line)) {
        for(auto pass : requiredPasses) {
            lineConsumed = pass->process(inputFile, outputFile, line, lastLine);
            if (lineConsumed) break;
        }

        if (!lineConsumed)
            outputFile << line << std::endl;
        lastLine = line;
    }

    for (auto pass : requiredPasses){
        pass->end(inputFile,outputFile, fileName);
    }
}

void PreProcessor::addPass(Pass *pass) {
    passes.push_back(pass);
}

void PreProcessor::finish() {
    for (auto pass: passes){
        pass->processingFinished();
    }
}
