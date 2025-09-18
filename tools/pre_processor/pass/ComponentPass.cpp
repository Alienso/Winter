//
// Created by Alienson on 11.3.2024.
//

#include "ComponentPass.h"
#include "util/stringUtils.h"

#include "fstream"

ComponentPass::ComponentPass(std::string& sourceDir, std::string &targetDir) {
    isClassComponent = false;
    componentCppFile = targetDir + "/Component.cpp";
}

void ComponentPass::begin(std::string &fileName) {
    bracketCounter = 0;
    className = {};
    isClassComponent = false;
}

bool ComponentPass::process(std::ifstream &inputFile, std::ofstream &outputFile, std::string &line, std::string &previousLine) {
    if (bracketCounter == 0) {
        size_t index = line.find("class");
        if (index != std::string::npos) {
            index += 6; //to offset the "class" + 1
            size_t indexLast = line.find(' ', index);
            className = line.substr(index, indexLast - index);

            size_t semicolonIndex = line.find(':');
            if (semicolonIndex != std::string::npos) {
                size_t componentIndex = line.find("Component", semicolonIndex);
                if (componentIndex != std::string::npos) {
                    componentClasses.emplace_back("", className, "_" + className + "_");
                    isClassComponent = true;
                    goto next;
                }
                componentIndex = line.find("Repository", semicolonIndex);
                if (componentIndex != std::string::npos) {
                    componentClasses.emplace_back("", className, "_" + className + "_");
                    isClassComponent = true;
                    goto next;
                }
            }
        }
    }

    next:

    for (char c: line) {
        switch (c) {
            case '{':
                bracketCounter++;
                break;
            case '}':
                //We finished reading a class
                if (bracketCounter == 1 && isClassComponent){
                    generateComponentId(outputFile);
                    isClassComponent = false;
                }
                bracketCounter--;
                break;
            default:
                break;
        }
    }

    return false;
}

void ComponentPass::end(std::ifstream &inputFile, std::ofstream &outputFile, std::string &fileName) {
    if (!componentClasses.empty()) {
        for (int i = componentClasses.size() - 1; i >= 0; i++) {
            if (componentClasses[i].filePath.empty())
                componentClasses[i].filePath = fileName;
            else break;
        }
    }
}

void ComponentPass::processingFinished() {
    std::ofstream outputFile = std::ofstream(componentCppFile, std::ios::trunc | std::fstream::out);
    if (!outputFile.is_open()) {
        std::cout << "Error while opening Component.cpp!\n";
    }

    //add includes
    outputFile << '\n';
    for (auto &x: componentClasses){
        std::string path = StringUtils::replace(x.filePath, '\\', '/');
        outputFile << "#include \"" << path << "\"\n";
    }
    outputFile << '\n';


    //generate initializeComponents()
    outputFile << "void Component::initializeComponents() {\n";
    outputFile << "\twtLogTrace(\"Initializing components\");\n";
    outputFile << "\tcomponents.resize(" << componentClasses.size() << ");\n";
    outputFile << "\tint i=0;\n\n";
    for (auto &x: componentClasses){
        outputFile << "\tauto* " << x.alternativeName << " = new " << x.className << "();\n";
        outputFile << "\tcomponents[i] = ((Component*)" << x.alternativeName << ");\n\n";
        outputFile << "\t" << x.className << "::_componentId_ = i++;\n";
    }
    outputFile << "}";
}

bool ComponentPass::shouldProcess(std::string &fileName) const {
    return StringUtils::endsWith(fileName, ".h") || StringUtils::endsWith(fileName, ".hpp");
}

void ComponentPass::generateComponentId(std::ofstream &outputFile) {
    outputFile << "public:\n";
    outputFile << "\tstatic inline int _componentId_ = 0;\n";
}
