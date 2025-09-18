//
// Created by Alienson on 18.2.2024.
//

#ifndef WINTER_REFLECTIONPASS_H
#define WINTER_REFLECTIONPASS_H

/**
 * This pass is used to generate Reflection data for each Reflect class. Modifications are done on class level.
 * Also modifies Reflect.cpp file to generate map of classes and class instance generators
 */

#include "Pass.h"
#include "reflect/Field.h"
#include "reflect/Method.h"

#include <utility>
#include <vector>

class ReflectionPass : public Pass{
public:
    explicit ReflectionPass(std::string& sourceDir, std::string &targetDir);
    void begin(std::string& fileName) override;
    bool process(std::ifstream &inputFile, std::ofstream &outputFile, std::string& line, std::string& previousLine) override;
    void end(std::ifstream &inputFile, std::ofstream &outputFile, std::string& fileName) override;
    void processingFinished() override;
    [[nodiscard]] bool shouldProcess(std::string &fileName) const override;

private:
    void generateReflectOverrides(std::ofstream &outputFile);

    bool shouldAddReflection = false;
    bool declaringMethod;
    bool declaringClass;
    bool hasSpace;
    int bracketCounter = 0;
    int smallBracketCounter = 0;

    std::vector<Field> fields;
    std::vector<Method> methods;
    std::string className;

    std::string reflectionCppFile;

    struct reflectFileData{
        std::string filePath;
        std::string className;
        std::string alternativeName;
        reflectFileData(std::string path, std::string className_, std::string alt) :
        filePath(std::move(path)), className(std::move(className_)), alternativeName(std::move(alt)) {}
    };
    std::vector<reflectFileData> reflectClasses;
};


#endif //WINTER_REFLECTIONPASS_H
