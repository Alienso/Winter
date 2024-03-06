//
// Created by Alienson on 4.2.2024..
//

#include <iostream>
#include <filesystem>

#include "../source/util/stringUtils.h"
#include "PreProcessor.h"
#include "pass/ReflectionPass.h"
#include "pass/AnnotationPass.h"

int processFile(const std::filesystem::directory_entry &entry);
void copyFileContents(ifstream &inputFile, ofstream &outputFile);

std::string outputDirectory;
std::string inputDirectory;

PreProcessor preProcessor;

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cout << "Please provide source and target directory";
        return -1;
    }

    std::cout << "source provided: " << argv[1] << '\n';
    std::cout << "target is: " << argv[2] << '\n';

    inputDirectory = std::string (argv[1]);
    outputDirectory = std::string(argv[2]);

    Pass* reflectionPass = new ReflectionPass();
    preProcessor.addPass(reflectionPass);

    Pass* annotationPass = new AnnotationPass();
    preProcessor.addPass(annotationPass);

    for (const auto &dirEntry: std::filesystem::recursive_directory_iterator(argv[1])){
        if (processFile(dirEntry) != 0) {
            std::cerr << "Error while parsing file: " << dirEntry << '\n';
            return -1;
        }
    }

    preProcessor.finish();

    return 0;
}

int processFile(const std::filesystem::directory_entry &entry) {
    std::cout << "Processing File: " << entry << '\n';

    std::string inputFileName = entry.path().string();
    std::string outputFileName = outputDirectory + (entry.path().string()).substr(inputDirectory.size(), inputFileName.size());

    if (entry.is_directory()){
        std::filesystem::create_directory(outputFileName);
    } else if (entry.is_regular_file()){

        std::ifstream inputFile(inputFileName);

        if (!inputFile.is_open()) {
            std::cerr << "Error: Unable to open input file " << inputFileName << std::endl;
            return 1;
        }

        if (!std::filesystem::exists(outputFileName)) {
            std::filesystem::copy_file(inputFileName, outputFileName);
        }
        std::ofstream outputFile(outputFileName, std::ios::trunc | std::ios::out);

        if (!outputFile.is_open()) {
            std::cerr << "Error: Unable to open output file " << outputFileName << std::endl;
            inputFile.close();
            return 1;
        }

        string currentFileName = entry.path().string();
        preProcessor.process(inputFile, outputFile, currentFileName);

        inputFile.close();
        outputFile.close();
    }

    return 0;
}

void copyFileContents(ifstream &inputFile, ofstream &outputFile) {
    std::string line;

    while (std::getline(inputFile, line)) {
        outputFile << line << std::endl;
    }
}
