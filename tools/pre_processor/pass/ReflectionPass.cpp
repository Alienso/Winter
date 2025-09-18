//
// Created by Alienson on 18.2.2024.
//

#include "ReflectionPass.h"
#include "util/stringUtils.h"
#include "serialize/FieldTypeUtil.h"

#include <fstream>

ReflectionPass::ReflectionPass(std::string &sourceDir, std::string &targetDir) {
    reflectionCppFile = targetDir + "/Reflect.cpp";
}

void ReflectionPass::begin(std::string& fileName) {
    shouldAddReflection = false;
    bracketCounter = 0;
    smallBracketCounter = 0;

    fields.resize(0);
    methods.resize(0);
    className = {};
}

void ReflectionPass::end(std::ifstream &inputFile, std::ofstream &outputFile, std::string& fileName) {
    if (!reflectClasses.empty()) {
        for (int i = reflectClasses.size() - 1; i >= 0; i++) {
            if (reflectClasses[i].filePath.empty())
                reflectClasses[i].filePath = fileName;
            else break;
        }
    }
}

bool ReflectionPass::shouldProcess(std::string &fileName) const {
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
            if (semicolonIndex != std::string::npos) {
                size_t reflectIndex = line.find("Reflect", semicolonIndex);
                if (reflectIndex != std::string::npos) {
                    if (className != "Entity") {
                        shouldAddReflection = true;
                        goto next;
                    }
                }

                if (className != "Entity") {
                    reflectIndex = line.find("Entity", semicolonIndex);
                    if (reflectIndex != std::string::npos) {
                        shouldAddReflection = true;
                        goto next;
                    }
                }
            }
        }
    }

    next:

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
                if (blank == std::string::npos)
                    return false;
                //Read all * or & that may be put together with the name
                while (line[blank + 1] == '*' || line[blank + 1] == '&')
                    blank++;
                blank++;
                std::string name = line.substr(blank, line.size() - blank - 1);
                name = StringUtils::trim(name);
                std::string typeStr = line.substr(0, blank);
                typeStr = StringUtils::stripBlankCharacters(typeStr);
                if (StringUtils::startsWith(typeStr,"std::"))
                    typeStr = typeStr.substr(5,typeStr.size() - 5);
                FieldType type = convertToFieldType(StringUtils::stripSpecialCharacters(typeStr));

                bool isPtr = false, isVec = false;
                if (typeStr[typeStr.size()-1] == '*')
                    isPtr = true;
                if (type == FIELD_TYPE_ARRAY || type == FIELD_TYPE_VECTOR)
                    isVec = true;
                Field f(name.data(), typeStr.data(), type, className.data(), 0, isPtr, isVec);
                fields.push_back(f);
            }
        }
    }

    return false;
}

void ReflectionPass::generateReflectOverrides(std::ofstream &outputFile) {

    std::string variableName = "_" + StringUtils::uncapitalize(className) + "_";

    //declaredFields,...
    outputFile << "\n\tstatic inline std::vector<Field> declaredFields = {};\n" <<
               "\tstatic inline std::vector<Method> declaredMethods = {};\n";

    outputFile << "\tField *getField(const char *fieldName) const override {\n"
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
                  "    int getClassSize() const override{\n"
                  "        return sizeof(" << className << ");\n"
                                                           "\t}\n\n"
                  "\tstatic Reflect* getInstance(){\n"
                  "    \treturn new " << className << "();\n"
                  "\t}\n\n";


    //clone
    outputFile << '\n';
    outputFile << "\t[[nodiscard]] Reflect* clone(CopyType copyType) const override{\n";
    outputFile << "\t\t" << className << "* copy = new " << className << "();\n";
    for (Field& f : fields){
        if (f.isPtr){
            //outputFile << "\t\tif (copyType == COPY_TYPE_DEEP){\n"; //TODO
            outputFile << "\t\tcopy->" << f.name << " = this->" << f.name << ";\n";
        }
        else
            outputFile << "\t\tcopy->" << f.name << " = this->" << f.name << ";\n";
    }
    outputFile << "\treturn copy;\n";
    outputFile << "\t}\n";

    //initialize reflection
    outputFile << '\n';
    outputFile << "\tstatic void initializeReflection(){\n";
    outputFile << "\t\t" << className << "* " << variableName << " = (" << className << "*) malloc(sizeof(" << className << "));\n";

    for (Field& x : fields){
        outputFile << "\t\t" << className << "::declaredFields.emplace_back(\"" << x.name << "\",\"" << x.typeStr << "\"," << x.type << ",\"" << x.className << "\","
                   << "(int*)(&" << variableName << "->" << x.name << ") - (int*)" << variableName << ',' << x.isPtr << ',' << x.isVec <<  ");\n";
    }
    outputFile << "\t\tfree(" << variableName << ");\n";
    outputFile << "\t}\n";
}

void ReflectionPass::processingFinished() {

    std::ofstream outputFile = std::ofstream(reflectionCppFile, std::ios::trunc | std::fstream::out);
    if (!outputFile.is_open()) {
        std::cout << "Error while opening Reflect.cpp!\n";
    }

    //add includes
    for (auto &x: reflectClasses){
        std::string path = StringUtils::replace(x.filePath, '\\', '/');
        outputFile << "#include \"" << path << "\"\n";
    }
    outputFile << '\n';

    //generate initializeClassMap()
    outputFile << "void Reflect::initializeClassMap(){\n";
    for (auto &x : reflectClasses){
        outputFile << "\tReflect::classMap[\"" << x.className << "\"] = &" << x.className << "::getInstance;\n";
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


