//
// Created by Alienson on 11.3.2024..
//

#include "ComponentPass.h"
#include "../../source/util/stringUtils.h"

#include "fstream"

void ComponentPass::begin(std::string &fileName) {
    bracketCounter = 0;
    className = {};

    size_t start = fileName.rfind("Component.h");
    if (start != string::npos) {
        string path = fileName.substr(0, fileName.size() - 2) + ".cpp";
        componentCppFile = StringUtils::replace(path , '\\', '/');
    }
}

void ComponentPass::process(std::ifstream &inputFile, std::ofstream &outputFile, std::string &line, std::string &previousLine) {
    if (bracketCounter == 0) {
        size_t index = line.find("class");
        if (index != std::string::npos) {
            index += 6; //to offset the "class" + 1
            size_t indexLast = line.find(' ', index);
            className = line.substr(index, indexLast - index);

            size_t semicolonIndex = line.find(':');
            if (semicolonIndex != string::npos) {
                size_t componentIndex = line.find("Component", semicolonIndex);
                if (componentIndex != string::npos)
                    componentClasses.emplace_back("", className, "_" + className + "_");
            }
        }
    }

    for (char c: line) {
        switch (c) {
            case '{':
                bracketCounter++;
                break;
            case '}':
                bracketCounter--;
                break;
            default:
                break;
        }
    }
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
    ofstream outputFile = ofstream(componentCppFile, std::ios::app);
    if (!outputFile.is_open()) {
        cout << "Error while opening Component.cpp!";
    }

    //add includes
    outputFile << '\n';
    for (auto &x: componentClasses){
        string path = StringUtils::replace(x.filePath, '\\', '/');
        outputFile << "#include \"" << path << "\"\n";
    }
    outputFile << '\n';


    //generate initializeComponents()
    outputFile << "void Component::initializeComponents() {\n";
    outputFile << "\twtLogTrace(\"Initializing components\");\n";
    outputFile << "\tint i=0;\n\n";
    for (auto &x: componentClasses){
        outputFile << "auto* " << x.alternativeName << " = new " << x.className << "();\n";
        outputFile << "\t" << x.alternativeName << "->_componentId_ = i++;\n";
        outputFile << "\tcomponents.push_back((Component*)" << x.alternativeName << ");\n\n";
    }
    outputFile << "}";
}

bool ComponentPass::shouldProcess(std::string &fileName) {
    return StringUtils::endsWith(fileName, ".h") || StringUtils::endsWith(fileName, ".hpp");
}
