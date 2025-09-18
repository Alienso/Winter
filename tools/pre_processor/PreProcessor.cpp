//
// Created by Alienson on 5.2.2024.
//

#include "PreProcessor.h"

void PreProcessor::process(std::ifstream &inputFile, std::ofstream &outputFile, std::string& inputFileName, std::string& outputFileName) {

    std::vector<Pass*> requiredPasses;
    std::string formattedInputFileName = inputFileName;
    std::replace(formattedInputFileName.begin(), formattedInputFileName.end(), '\\', '/');

    for (auto pass : passes){
        if (pass->shouldProcess(outputFileName))
            requiredPasses.push_back(pass);
    }

    for (auto pass : requiredPasses){
        pass->begin(outputFileName);
    }

    bool lineConsumed = false;
    for (int lineNumber = 1; std::getline(inputFile, line); lineNumber++) {
        for(auto pass : requiredPasses) {
            lineConsumed = pass->process(inputFile, outputFile, line, lastLine);
            if (lineConsumed) break;
        }

        if (!lineConsumed && !line.empty()) {
            outputFile << "#line " << lineNumber << " \"" << formattedInputFileName << "\"\n";
            outputFile << line << std::endl;
        }
        lastLine = line;
    }

    for (auto pass : requiredPasses){
        pass->end(inputFile, outputFile, outputFileName);
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
