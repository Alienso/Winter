//
// Created by Alienson on 18.2.2024..
//

#include "ReflectionPass.h"

#include <fstream>

void ReflectionPass::begin() {
    shouldAddReflection = false;
    bracketCounter = 0;
    smallBracketCounter = 0;

    fields.resize(0);
    methods.resize(0);
    className = {};
}

void ReflectionPass::end(ifstream &inputFile, ofstream &outputFile) {

}

bool ReflectionPass::shouldProcess(string &fileName) {
    return StringUtils::endsWith(fileName, ".h") || StringUtils::endsWith(fileName, ".hpp");
}

void ReflectionPass::process(std::ifstream &inputFile, std::ofstream &outputFile, std::string& line, std::string& previousLine) {

    //TODO add comments support
    declaringMethod = false;
    declaringClass = false;
    hasSpace = false;

    //We are outside class
    if (bracketCounter == 0) {
        size_t index = line.find("class");
        if (index != std::string::npos) {
            index += 6; //to offset the "class" + 1
            size_t indexLast = line.find(' ', index);
            className = line.substr(index, indexLast - index);

            //Check if we need to add reflection or not
            size_t semicolonIndex = line.find(':');
            if (semicolonIndex != string::npos) {
                size_t reflectIndex = line.find("Reflect", semicolonIndex);
                if (reflectIndex != string::npos)
                    shouldAddReflection = true;
            }
        }
    }

    //Bracket counters
    for (char c: line) {
        if (c == '{') {
            if (bracketCounter == 0)
                declaringClass = true;
            bracketCounter++;
        } else if (c == '}') {
            //We finished reading a class, so we write the data and reset everything
            if (bracketCounter == 1) {
                if (shouldAddReflection) {
                    shouldAddReflection = false;
                    insertFieldsAndMethods(outputFile);
                }
                fields.resize(0);
                methods.resize(0);
            }
            bracketCounter--;
        } else if (c == '(') {
            smallBracketCounter++;
            declaringMethod = true;
        } else if (c == ')')
            smallBracketCounter--;
        else if (c == ' ')
            hasSpace = true;
    }

    if (!shouldAddReflection)
        return;

    //If we are not in a function body
    if (bracketCounter == 1 && !declaringClass && hasSpace && StringUtils::trim(line).size() > 2) {
        //declaring a method
        if (declaringMethod) {
            //multiline declaration
            if (smallBracketCounter != 0) {

            } else {

            }

        }
            //declaring a field/s
        else {
            /*if (std::regex_search(line, match, fieldDeclarationRegex)) { //We dont need to check this since we know this IS a field declaration
                std::cout << "Field Declaration: " << match[0] << std::endl;
            }*/

            //multi variable definition
            if (line.find(',') != std::string::npos) {

            }
                //single line declaration
            else {
                //Temp solution, find last ' ' separating type from name
                size_t blank = line.find_last_of(' ');
                if (blank == string::npos)
                    return;
                //Read all * or & that may be put together with the name
                while (line[blank + 1] == '*' || line[blank + 1] == '&')
                    blank++;
                blank++;
                std::string name = line.substr(blank, line.size() - blank - 1);
                name = StringUtils::trim(name);
                std::string typeStr = line.substr(0, blank);
                typeStr = StringUtils::stripBlankCharacters(typeStr);
                FieldType type = convertToFieldType(StringUtils::stripSpecialCharacters(typeStr).data());
                Field f(name, typeStr, type, className, 0);
                fields.push_back(f);
            }
        }
    }
}

void ReflectionPass::insertFieldsAndMethods(std::ofstream &outputFile){

    outputFile << '\n';
    outputFile << "\tint __reflection__data__helper__ = ([this]() {\n";

    for (Field& x : fields){
        outputFile << "\t\tdeclaredFields.emplace_back(\"" << x.name << "\",\"" << x.typeStr << "\"," << x.type << ",\"" << x.className << "\","
                   << "(int*)(&this->" << x.name << ") - (int*)this" << ");\n";
    }

    outputFile << "\t\treturn 0;\n" << "\t})();\n";

    //TODO try make __reflection_data_helper__ static
}

void ReflectionPass::processingFinished() {

}


