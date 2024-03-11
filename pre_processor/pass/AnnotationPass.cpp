//
// Created by Alienson on 18.2.2024..
//

#include "fstream"

#include "AnnotationPass.h"
#include "../source/util/stringUtils.h"

void AnnotationPass::begin(std::string& fileName) {
    bracketCounter = 0;
}

void AnnotationPass::process(std::ifstream &inputFile, std::ofstream &outputFile, std::string &line, std::string &previousLine) {

    for (char c: line) {
        if (c == '{')
            bracketCounter++;
        else if (c == '}') {
            //We finished reading a class, so we write the data and reset everything
            if (bracketCounter == 1) {
                registerEndpoints(outputFile);
                endpointData.resize(0);
            }
            bracketCounter--;
        }
    }

    if (bracketCounter == 0){
        if (StringUtils::trim(previousLine) != "$RestController")
            return;
        handleRestController(line);
    }

    if (bracketCounter != 1)
        return;

    size_t beginIndex = previousLine.find('$');
    if (beginIndex == string::npos)
        return;
    size_t endIndex = previousLine.find('(', beginIndex);
    if (endIndex == string::npos)
        return;

    string methodName = previousLine.substr(beginIndex + 1, endIndex - beginIndex - 1);

    beginIndex = previousLine.find('"', endIndex);
    endIndex = previousLine.find_last_of('"');
    if (beginIndex == string::npos || endIndex == string::npos || beginIndex == endIndex)
        return;

    string path = previousLine.substr(beginIndex + 1, endIndex - beginIndex - 1);

    endIndex = line.find('(');
    if (endIndex == string::npos)
        return;
    beginIndex = line.rfind(' ', endIndex);
    if (beginIndex == string::npos)
        return;

    string functionName = line.substr(beginIndex + 1, endIndex - beginIndex - 1);

    beginIndex = endIndex;
    endIndex = line.find('*', endIndex);
    if (endIndex == string::npos)
        return;

    string bodyType = line.substr(beginIndex + 1, endIndex - beginIndex - 1);
    bodyType = StringUtils::trim(bodyType);

    EndpointData endpoint(methodName, path, functionName, bodyType);
    endpointData.push_back(endpoint);
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

void AnnotationPass::generateMethod(std::ofstream &outputFile, EndpointData& endpoint){
    outputFile << "}\n";
    outputFile << "\tstd::string* __" << endpoint.functionName << "__(shared_ptr<HttpRequest> &httpRequest, Reflect* (*f)(Reflect*)) {\n";
    outputFile << "\t\tstring requestStr = httpRequest->getRequestBody();\n";
    outputFile << "\t\tJsonDeserializer<" << endpoint.bodyType << "> deserializer;\n";
    outputFile << "\t\tReflect* request = deserializer.deserialize(requestStr);\n";
    outputFile << "\t\tReflect* res = f(request);\n";
    outputFile << "\t\tJsonSerializer serializer;\n";
    outputFile << "\t\treturn serializer.serialize(res);\n";
    outputFile << "\t}\n";
}

void AnnotationPass::generateEndpoint(std::ofstream &outputFile, EndpointData& endpoint){
    outputFile << '\n';
    outputFile << "\tint __endpoint_data__helper__ = ([this]() {\n";

    outputFile << "\t\tURI uri{\"" << endpoint.uri << "\"};\n";
    outputFile << "\t\tHttpMethod* method = HttpMethod::fromString(\"" << endpoint.method.data() << "\");\n";

    outputFile << "\t\tauto* endpoint = new Endpoint();\n";
    outputFile << "\t\tendpoint->f = reinterpret_cast<Reflect *(*)(Reflect *)>(&" << endpoint.functionName <<");\n";
    outputFile << "\t\tendpoint->g = __" << endpoint.functionName << "__;\n";
    outputFile << "\t\tendpoint->method = method;\n";
    outputFile << "\t\tendpoint->uri = uri;\n";
    outputFile << "\t\tRouter::getInstance()->registerEndpoint(endpoint);\n";

    outputFile << "\t\treturn 0;\n" << "\t})();\n";
}

void AnnotationPass::registerEndpoints(std::ofstream &outputFile) {
    for (auto& endpoint: endpointData){
        outputFile << '\n';
        outputFile << "\tint __endpoint_data__helper__ = ([this]() {\n";

        outputFile << "\t\tURI uri{\"" << endpoint.uri << "\"};\n";
        outputFile << "\t\tHttpMethod* method = HttpMethod::fromString(\"" << endpoint.method.data() << "\");\n";

        outputFile << "\t\tauto* endpoint = new Endpoint();\n";
        outputFile << "\t\tendpoint->func = &" << endpoint.functionName <<";\n";
        outputFile << "\t\tendpoint->method = method;\n";
        outputFile << "\t\tendpoint->uri = uri;\n";
        outputFile << "\t\tRouter::getInstance()->registerEndpoint(endpoint);\n";

        outputFile << "\t\treturn 0;\n" << "\t})();\n";
    }
}

void AnnotationPass::end(std::ifstream &inputFile, std::ofstream &outputFile, std::string& fileName) {

}

bool AnnotationPass::shouldProcess(string &fileName) {
    return StringUtils::endsWith(fileName, ".h") || StringUtils::endsWith(fileName, ".hpp");
}

void AnnotationPass::processingFinished() {
    //TODO write Component.Cpp file (remember handle to Component.h file so we can open Component.cpp)

    /*MyController* controller = new MyController();
    components.push_back(controller);
    MyController::id = 0;

    MyController2* controller2 = new MyController2();
    components.push_back(controller2);
    MyController2::id = 1;*/

    //getByClass => return components[T::id]
}