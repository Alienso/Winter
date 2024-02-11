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

    while (std::getline(inputFile, line)) {

        declaringMethod = false;
        declaringClass = false;
        hasSpace = false;

        if (bracketCounter == 0){
            int index = line.find("class");
            if (index != std::string::npos){
                index += 6; //to offset the "class" + 1
                int indexLast = line.find(' ', index);
                className = line.substr(index, indexLast - index);
            }
        }

        for (char c : line){
            if (c == '{') {
                if (bracketCounter == 0)
                    declaringClass = true;
                bracketCounter++;
            }
            else if (c == '}') {
                if (bracketCounter == 1){
                    insertFieldsAndMethods(outputFile, fields, methods);
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

        //If we are not in a function body
        if (bracketCounter == 1 && !declaringClass && hasSpace && trim(line).size() > 2){
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
                    //int star = line.find_last_of('*');
                    //int ampersand = line.find_last_of('&');
                    int blank = line.find_last_of(' ');
                    while(line[blank+1] == '*' || line[blank+1] == '&')
                        blank++;
                    blank++;
                    std::string name = line.substr(blank, line.size() - blank - 1);
                    name = trim(name);
                    std::string type = line.substr(0, blank);
                    type = stripBlankCharacters(type);
                    Field f(name, type, className, 0);
                    fields.push_back(f);
                }
            }
        }

        outputFile << line << std::endl;
    }
}

void insertFieldsAndMethods(std::ofstream &outputFile, std::vector<Field>& fields, std::vector<Method>& methods){

    outputFile << '\n';
    outputFile << "\tint __reflection__data__helper__ = ([this]() {\n";

    for (Field& x : fields){
        outputFile << "\t\tdeclaredFields.emplace_back(\"" << x.name << "\",\"" << x.type << "\",\"" << x.className << "\","
        << "(int*)(&this->" << x.name << ") - (int*)this" << ");\n";
    }

    outputFile << "\t\treturn 0;\n" << "\t})();\n";

    //TODO reset all data for potential new class definition
    //TODO fix names and types
    //TODO try make __reflection_data_helper__ static

}
