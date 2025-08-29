//
// Created by Alienson on 4.2.2024.
//

/**
 * Given input and output directory, executes all 'Passes' on the files from the source directory and writes the output
 * to the output directory
 */

#include <iostream>
#include <filesystem>

#include "stringUtils.h"
#include "PreProcessor.h"
#include "pass/ReflectionPass.h"
#include "pass/AnnotationPass.h"
#include "pass/ComponentPass.h"

int processFile(const std::filesystem::directory_entry &entry);

std::string outputDirectory;
std::string inputDirectory;

PreProcessor preProcessor;

int main(int argc, char** argv) {

    if (argc != 3) {
        std::cout << "Please use 'clean [dir]' or provide source and target directory\n";
        return -1;
    }

    if (argv[1] == std::string("clean")){
        auto path = std::filesystem::absolute(argv[2]);
        std::cout << "Are you sure you want to delete: " << path << " (y/n)\n";
        char c = (char)getchar();
        if (c == 'y' || c == 'Y'){
            std::filesystem::remove_all(path);
            std::cout << "Deleted " << path << '\n';
            return 0;
        }
        std::cout << "Cancelling cleaning\n";
        return 0;
    }

    std::cout << "source is: " << argv[1] << '\n';
    std::cout << "target is: " << argv[2] << '\n';

    if (!std::filesystem::exists(argv[2]))
        std::filesystem::create_directory(argv[2]);

    inputDirectory = std::string (argv[1]);
    outputDirectory = std::string(argv[2]);

    Pass* reflectionPass = new ReflectionPass();
    preProcessor.addPass(reflectionPass);

    Pass* annotationPass = new AnnotationPass();
    preProcessor.addPass(annotationPass);

    Pass* componentPass = new ComponentPass();
    preProcessor.addPass(componentPass);

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

        preProcessor.process(inputFile, outputFile, outputFileName);

        inputFile.close();
        outputFile.close();
    }

    return 0;
}
