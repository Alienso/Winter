//
// Created by Alienson on 18.2.2024..
//

#include "fstream"

#include "AnnotationPass.h"
#include "stringUtils.h"

void AnnotationPass::begin(std::string& fileName) {
    bracketCounter = 0;

    size_t start = fileName.rfind("Router.h");
    if (start != string::npos) {
        string path = fileName.substr(0, fileName.size() - 2) + ".cpp";
        routerCppFile = StringUtils::replace(path , '\\', '/');
    }
}

bool AnnotationPass::process(std::ifstream &inputFile, std::ofstream &outputFile, std::string &line, std::string &previousLine) {

    if (bracketCounter == 0) {
        if (StringUtils::trim(previousLine) == "$RestController")
            handleRestController(line);
    }

    if (bracketCounter == 1){
        if (StringUtils::trim(previousLine) == "$PostConstruct")
            handlePostConstruct(line);
        else if (StringUtils::trim(previousLine) == "$Autowired")
            return handleAutoWire(line, outputFile);
    }

    for (char c: line) {
        if (c == '{')
            bracketCounter++;
        else if (c == '}') {
            //We finished reading a class, so we write the data and reset everything
            if (bracketCounter == 1) {
                registerEndpoints(outputFile);
                addPostConstruct(outputFile);
                endpointData.resize(0);
                postConstructMethodName = {};
            }
            bracketCounter--;
        }
    }

    if (bracketCounter != 1)
        return false;

    //Endpoint annotation
    size_t beginIndex = previousLine.find('$');
    if (beginIndex == string::npos)
        return false;
    size_t endIndex = previousLine.find('(', beginIndex);
    if (endIndex == string::npos)
        return false;

    string methodName = previousLine.substr(beginIndex + 1, endIndex - beginIndex - 1);

    beginIndex = previousLine.find('"', endIndex);
    endIndex = previousLine.find_last_of('"');
    if (beginIndex == string::npos || endIndex == string::npos || beginIndex == endIndex)
        return false;

    string path = previousLine.substr(beginIndex + 1, endIndex - beginIndex - 1);

    endIndex = line.find('(');
    if (endIndex == string::npos)
        return false;
    beginIndex = line.rfind(' ', endIndex);
    if (beginIndex == string::npos)
        return false;

    string functionName = line.substr(beginIndex + 1, endIndex - beginIndex - 1);

    beginIndex = endIndex;
    endIndex = line.find('*', endIndex);
    if (endIndex == string::npos)
        return false;

    string bodyType = line.substr(beginIndex + 1, endIndex - beginIndex - 1);
    bodyType = StringUtils::trim(bodyType);

    EndpointData endpoint(methodName, path, functionName, bodyType);
    endpointData.push_back(endpoint);

    return false;
}

void AnnotationPass::handleRestController(string &line) {
    size_t beginIndex = line.find("class");
    if (beginIndex == string::npos)
        return;

    beginIndex+=5;

    while(line[beginIndex] == ' '){
        if (beginIndex == line.size())
            return;
        beginIndex++;
    }

    if (line[beginIndex] == '['){
        //TODO handleAttributes
        return;
    }

    if (StringUtils::startsWith(&(line[beginIndex]), "alignas")){
        beginIndex+=8;
        while(line[beginIndex] != ')') {
            if (beginIndex == line.size())
                return;
            beginIndex++;
        }

        while(line[beginIndex] == ' '){
            if (beginIndex == line.size())
                return;
            beginIndex++;
        }
    }

    size_t endIndex = line.find(' ', beginIndex);
    if (endIndex == string::npos)
        return;

    restControllers.emplace_back(line.substr(beginIndex, endIndex - beginIndex));
}

void AnnotationPass::handlePostConstruct(string &line) {
    size_t endIndex, beginIndex;
    endIndex = line.find('(');
    if (endIndex == string::npos)
        return;
    beginIndex = line.rfind(' ', endIndex);
    if (beginIndex == string::npos)
        return;

    postConstructMethodName = line.substr(beginIndex + 1, endIndex - beginIndex - 1);
}

bool AnnotationPass::handleAutoWire(string &line, std::ofstream &outputFile) {
    size_t startIndex = 0, endIndex = 0;
    while(startIndex<line.size() && isspace(line[startIndex])) startIndex++;
    endIndex = line.find('*', startIndex + 1);
    if (endIndex == string::npos)
        return false;

    string classNamePtr = line.substr(startIndex, endIndex - startIndex + 1);
    while(endIndex > startIndex && !isalnum(line[endIndex])) endIndex--;
    if (endIndex == startIndex)
        return false;
    string className = line.substr(startIndex, endIndex - startIndex + 1);

    endIndex = line.rfind(';');
    if (endIndex == string::npos)
        return false;

    endIndex--;
    startIndex = endIndex - 1;
    while(startIndex > 0 && isalnum(line[startIndex])) startIndex--;
    string varName = line.substr(startIndex, endIndex - startIndex + 1);

    outputFile << '\t' << classNamePtr << ' ' << varName << " = (" << classNamePtr << ")(Component::getById(" << className << "::_componentId_));\n";
    return true;
}

void AnnotationPass::registerEndpoints(std::ofstream &outputFile) {

    if (endpointData.empty())
        return;

    outputFile << '\n';
    outputFile << "\tint _endpoint_data__helper_ = ([this]() {\n\n";
    outputFile << "\t\tURI uri;\n";
    outputFile << "\t\tHttpMethod* method;\n";
    outputFile << "\t\tEndpoint* endpoint;\n\n";

    for (auto& endpoint: endpointData){
        outputFile << "\t\turi = URI{\"" << endpoint.uri << "\"};\n";
        outputFile << "\t\tmethod = HttpMethod::fromString(\"" << endpoint.method.data() << "\");\n";
        outputFile << "\t\tendpoint = new Endpoint();\n";
        outputFile << "\t\tendpoint->func = std::function([this](HttpRequest* req){ return " << endpoint.functionName << "(req);});\n";
        outputFile << "\t\tendpoint->method = method;\n";
        outputFile << "\t\tendpoint->uri = uri;\n";
        outputFile << "\t\tRouter::getInstance()->registerEndpoint(endpoint);\n\n";
    }

    outputFile << "\t\treturn 0;\n" << "\t})();\n";
}

void AnnotationPass::addPostConstruct(std::ofstream &outputFile){
    if (postConstructMethodName.empty())
        return;
    outputFile << '\n';
    outputFile << "\tint _post_construct_helper_ = ([this]() {\n";
    outputFile << "\t\t" << postConstructMethodName << "();\n";
    outputFile << "\t\treturn 0;\n" << "\t})();\n";
}

void AnnotationPass::end(std::ifstream &inputFile, std::ofstream &outputFile, std::string& fileName) {
}

bool AnnotationPass::shouldProcess(string &fileName) const {
    return StringUtils::endsWith(fileName, ".h") || StringUtils::endsWith(fileName, ".hpp");
}

//Not in use currently
void AnnotationPass::processingFinished() {

    //TODO how to get method reference?
    endpointData.resize(0);

    ofstream outputFile = ofstream(routerCppFile, std::ios::app);
    if (!outputFile.is_open()) {
        cout << "Error while opening Router.cpp!";
    }

    if(endpointData.empty()) {
        outputFile.close();
        return;
    }

    outputFile << "void Router::initializeEndpoints(){\n";
    outputFile << "\tURI uri;\n";
    outputFile << "\tHttpMethod* method = nullptr;\n";
    outputFile << "\tEndpoint* endpoint = new Endpoint();\n";

    for (auto& endpoint: endpointData){
        outputFile << "\turi = {\"" << endpoint.uri << "\"};\n";
        outputFile << "\tmethod = HttpMethod::fromString(\"" << endpoint.method.data() << "\");\n";

        outputFile << "\tendpoint = new Endpoint();\n";
        outputFile << "\tendpoint->func = std::function([this](HttpRequest* req){return" << endpoint.functionName << "(req);});";
        outputFile << "\tendpoint->method = method;\n";
        outputFile << "\tendpoint->uri = uri;\n";
        outputFile << "\tRouter::getInstance()->registerEndpoint(endpoint);\n";
    }

    outputFile << "}\n";

    outputFile.close();
}
