//
// Created by Alienson on 5.2.2024..
//

#include "PreProcessor.h"
#include <iostream>

#include "../source/reflect/Field.h"
#include "../source/reflect/Method.h"
#include "../source/util/stringUtils.h"

void insertFieldsAndMethods(std::ofstream &outputFile, std::vector<Field>& fields, std::vector<Method>& methods);

void PreProcessor::process(std::ifstream &inputFile, std::ofstream &outputFile) {
    bracketCounter = 0;
    smallBracketCounter = 0;

    std::vector<Field> fields;
    std::vector<Method> methods;
    std::string className;

    bool shouldAddReflection = false;

    while (std::getline(inputFile, line)) {

        declaringMethod = false;
        declaringClass = false;
        hasSpace = false;

        //We are outside class
        if (bracketCounter == 0){
            int index = line.find("class");
            if (index != std::string::npos){
                index += 6; //to offset the "class" + 1
                int indexLast = line.find(' ', index);
                className = line.substr(index, indexLast - index);

                //Check if we need to add reflection or not
                int semicolonIndex = line.find(':');
                if (semicolonIndex != string::npos){
                    int reflectIndex = line.find("Reflect", semicolonIndex);
                    if (reflectIndex != string::npos)
                        shouldAddReflection = true;
                }
            }
        }

        //Bracket counters
        for (char c : line){
            if (c == '{') {
                if (bracketCounter == 0)
                    declaringClass = true;
                bracketCounter++;
            }
            else if (c == '}') {
                //We finished reading a class, so we write the data and reset everything
                if (bracketCounter == 1){
                    if (shouldAddReflection) {
                        shouldAddReflection = false;
                        insertFieldsAndMethods(outputFile, fields, methods);
                    }
                    fields.resize(0);
                    methods.resize(0);
                }
                bracketCounter--;
            }
            else if (c == '('){
                smallBracketCounter++;
                declaringMethod = true;
            }
            else if (c == ')')
                smallBracketCounter--;
            else if (c == ' ')
                hasSpace = true;
        }

        if (!shouldAddReflection)
            goto end; //just copy input line to output file

        //If we are not in a function body
        if (bracketCounter == 1 && !declaringClass && hasSpace && StringUtils::trim(line).size() > 2){
            //declaring a method
            if (declaringMethod){
                //multiline declaration
                if (smallBracketCounter != 0){

                }else{

                }

            }
            //declaring a field/s
            else {
                /*if (std::regex_search(line, match, fieldDeclarationRegex)) { //We dont need to check this since we know this IS a field declaration
                    std::cout << "Field Declaration: " << match[0] << std::endl;
                }*/

                //multi variable definition
                if (line.find(',') != std::string::npos){

                }
                //single line declaration
                else{
                    //Temp solution, find last ' ' separating type from name
                    int blank = line.find_last_of(' ');
                    //Read all * or & that may be put together with the name
                    while(line[blank+1] == '*' || line[blank+1] == '&')
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

        end:
        outputFile << line << std::endl;
    }
}

void insertFieldsAndMethods(std::ofstream &outputFile, std::vector<Field>& fields, std::vector<Method>& methods){

    outputFile << '\n';
    outputFile << "\tint __reflection__data__helper__ = ([this]() {\n";

    for (Field& x : fields){
        outputFile << "\t\tdeclaredFields.emplace_back(\"" << x.name << "\",\"" << x.typeStr << "\"," << x.type << ",\"" << x.className << "\","
        << "(int*)(&this->" << x.name << ") - (int*)this" << ");\n";
    }

    outputFile << "\t\treturn 0;\n" << "\t})();\n";

    //TODO try make __reflection_data_helper__ static
}
