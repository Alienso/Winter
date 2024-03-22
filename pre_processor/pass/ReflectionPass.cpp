//
// Created by Alienson on 18.2.2024..
//

#include "ReflectionPass.h"

#include <fstream>

void ReflectionPass::begin(std::string& fileName) {
    shouldAddReflection = false;
    bracketCounter = 0;
    smallBracketCounter = 0;

    fields.resize(0);
    methods.resize(0);
    className = {};

    size_t start = fileName.rfind("Reflect.h");
    if (start != string::npos) {
        string path = fileName.substr(0, fileName.size() - 2) + ".cpp";
        reflectionCppFile = StringUtils::replace(path , '\\', '/');
    }
}

void ReflectionPass::end(ifstream &inputFile, ofstream &outputFile, std::string& fileName) {
    if (!reflectClasses.empty()) {
        for (int i = reflectClasses.size() - 1; i >= 0; i++) {
            if (reflectClasses[i].filePath.empty())
                reflectClasses[i].filePath = fileName;
            else break;
        }
    }
}

bool ReflectionPass::shouldProcess(string &fileName) const {
    return StringUtils::endsWith(fileName, ".h") || StringUtils::endsWith(fileName, ".hpp");
}

bool ReflectionPass::process(std::ifstream &inputFile, std::ofstream &outputFile, std::string& line, std::string& previousLine) {

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
                    reflectClasses.emplace_back("", className, "_" + className + "_");
                    generateReflectOverrides(outputFile);
                }
                shouldAddReflection = false;
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
        return false;

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
                    return false;
                //Read all * or & that may be put together with the name
                while (line[blank + 1] == '*' || line[blank + 1] == '&')
                    blank++;
                blank++;
                std::string name = line.substr(blank, line.size() - blank - 1);
                name = StringUtils::trim(name);
                std::string typeStr = line.substr(0, blank);
                typeStr = StringUtils::stripBlankCharacters(typeStr);
                FieldType type;
                if (typeStr[typeStr.size()-1] == '*')
                    type = FIELD_TYPE_PTR;
                else
                    type = convertToFieldType(StringUtils::stripSpecialCharacters(typeStr).data());
                Field f(name, typeStr, type, className, 0);
                fields.push_back(f);
            }
        }
    }

    return false;
}

void ReflectionPass::generateReflectOverrides(ofstream &outputFile) {

    string variableName = "_" + StringUtils::uncapitalize(className) + "_";

    outputFile << "\n\tstatic inline std::vector<Field> declaredFields = {};\n" <<
               "\tstatic inline std::vector<Method> declaredMethods = {};\n";

    outputFile << "\tField *getField(const char *fieldName) override {\n"
                  "        for (Field& f : declaredFields){\n"
                  "            if (f.name == fieldName)\n"
                  "                return &f;\n"
                  "        }\n"
                  "        return &Field::INVALID;\n"
                  "    }\n"
                  "\n"
                  "    std::vector<Field> &getDeclaredFields() override {\n"
                  "        return declaredFields;\n"
                  "    }\n\n"
                  "    int getClassSize() override{\n"
                  "        return sizeof(" << className << ");\n"
                                                           "\t}\n\n";


    outputFile << '\n';
    outputFile << "\tstatic void initializeReflection(){\n";
    outputFile << "\t\t" << className << "* " << variableName << " = (" << className << "*) malloc(sizeof(" << className << "));\n";

    for (Field& x : fields){
        outputFile << "\t\t" << className << "::declaredFields.emplace_back(\"" << x.name << "\",\"" << x.typeStr << "\"," << x.type << ",\"" << x.className << "\","
                   << "(int*)(&" << variableName << "->" << x.name << ") - (int*)" << variableName << ");\n";
    }
    outputFile << "\t\tfree(" << variableName << ");\n";
    outputFile << "\t}\n";
}

void ReflectionPass::processingFinished() {

    ofstream outputFile = ofstream(reflectionCppFile, std::ios::app);
    if (!outputFile.is_open()) {
        cout << "Error while opening Reflect.cpp!";
    }

    //add includes
    for (auto &x: reflectClasses){
        string path = StringUtils::replace(x.filePath, '\\', '/');
        outputFile << "#include \"" << path << "\"\n";
    }
    outputFile << '\n';

    //generate class generators
    for (auto &x: reflectClasses){
        outputFile << "Reflect* " << x.alternativeName << "(){\n"
        << "\treturn new " << x.className << "();\n"
        << "}\n\n";
    }

    //generate initializeClassMap()
    outputFile << "void Reflect::initializeClassMap(){\n";
    for (auto &x : reflectClasses){
        outputFile << "\tReflect::classMap[\"" << x.className << "\"] = &" << x.alternativeName << ";\n";
    }
    outputFile << "}\n";

    //generate initializeReflection()
    outputFile << "void Reflect::initializeReflection() {\n";
    for (auto &x : reflectClasses){
        outputFile << '\t' << x.className << "::initializeReflection();\n";
    }
    outputFile << "\tinitializeClassMap();\n}";

    outputFile.close();
}


