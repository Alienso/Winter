//
// Created by Alienson on 18.2.2024..
//

#ifndef WINTER_REFLECTIONPASS_H
#define WINTER_REFLECTIONPASS_H


#include "Pass.h"

#include "../source/reflect/Field.h"
#include "../source/reflect/Method.h"

#include <utility>

class ReflectionPass : public Pass{
public:
    void begin(std::string& fileName) override;
    void process(std::ifstream &inputFile, std::ofstream &outputFile, std::string& line, std::string& previousLine) override;
    void end(std::ifstream &inputFile, std::ofstream &outputFile, std::string& fileName) override;
    void processingFinished() override;
    [[nodiscard]] bool shouldProcess(string &fileName) const override;

private:
    void generateReflectOverrides(ofstream &outputFile);

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

    //Support for = ([a-zA-Z]+::)?([a-zA-Z_][a-zA-Z0-9_]*?)\s*(<.+?>)?\s*[\*&\s(const?)]+?\s*([a-zA-Z_][a-zA-Z0-9_]*)\s*(=\s*.+)?\s*(;|,)
    //TODO will catch syntactically inaccurate declarations (int & const * const & x;)
    //std::regex fieldDeclarationRegex{R"(([a-zA-Z]+::)?([a-zA-Z_][a-zA-Z0-9_]*?)\s*(<.+?>)?\s*[\*&\s(const?)]+?\s*([a-zA-Z_][a-zA-Z0-9_]*)\s*(;|,))"};
    //std::smatch match;
};


#endif //WINTER_REFLECTIONPASS_H
