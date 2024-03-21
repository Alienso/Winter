//
// Created by Alienson on 11.3.2024..
//

#ifndef WINTER_COMPONENTPASS_H
#define WINTER_COMPONENTPASS_H

/**
 * This pass is used for registering all classes that extend Component, so we can generate data that is needed for
 * Spring-like @Autowiring. It fills an array with instances of each component which can later be used to get that
 * instance
 */

#include "Pass.h"

#include <vector>

class ComponentPass : public Pass{
public:
    void begin(std::string &fileName) override;
    void process(std::ifstream &inputFile, std::ofstream &outputFile, std::string &line, std::string &previousLine) override;
    void end(std::ifstream &inputFile, std::ofstream &outputFile, std::string &fileName) override;
    void processingFinished() override;
    [[nodiscard]] bool shouldProcess(std::string &fileName) const override;


private:
    int bracketCounter = 0;
    bool isClassComponent;
    std::string className;
    std::string componentCppFile;

    struct componentFileData{
        std::string filePath;
        std::string className;
        std::string alternativeName;
        componentFileData(std::string path, std::string className_, std::string alt):
        filePath(std::move(path)), className(std::move(className_)), alternativeName(std::move(alt)) {}
    };
    std::vector<componentFileData> componentClasses;

    void generateComponentId(std::ofstream &outputFile);
};


#endif //WINTER_COMPONENTPASS_H
