//
// Created by Alienson on 18.2.2024..
//

#ifndef WINTER_ANNOTATIONPASS_H
#define WINTER_ANNOTATIONPASS_H

#include "Pass.h"
#include <vector>

class AnnotationPass : public Pass{
public:
    void begin() override;
    void process(std::ifstream &inputFile, std::ofstream &outputFile, std::string &line, std::string &previousLine) override;
    void end(std::ifstream &inputFile, std::ofstream &outputFile) override;

    bool shouldProcess(std::string &fileName) override;

private:
    struct EndpointData{
        EndpointData() = default;
        EndpointData(std::string& _method, std::string& _uri, std::string& _f, std::string& _type) : method(_method), uri(_uri), functionName(_f), bodyType(_type){}
        std::string method;
        std::string uri;
        std::string functionName;
        std::string bodyType;
    };

    void generateMethod(std::ofstream &outputFile, EndpointData& endpoint);
    void generateEndpoint(std::ofstream &outputFile, EndpointData& endpoint);
    void registerEndpoints(std::ofstream &outputFile);

    std::vector<EndpointData> endpointData;
    int bracketCounter;
};


#endif //WINTER_ANNOTATIONPASS_H
