//
// Created by Alienson on 18.2.2024..
//

#ifndef WINTER_ANNOTATIONPASS_H
#define WINTER_ANNOTATIONPASS_H

/**
 * General purpose pass that recognized $Annotation and acts accordingly
 */

#include "Pass.h"
#include <vector>

class AnnotationPass : public Pass{
public:
    void begin(std::string& fileName) override;
    bool process(std::ifstream &inputFile, std::ofstream &outputFile, std::string &line, std::string &previousLine) override;
    void end(std::ifstream &inputFile, std::ofstream &outputFile, std::string& fileName) override;
    void processingFinished() override;

    [[nodiscard]] bool shouldProcess(std::string &fileName) const override;

private:
    struct EndpointData{
        EndpointData() = default;
        EndpointData(std::string& _method, std::string& _uri, std::string& _f, std::string& _type) : method(_method), uri(_uri), functionName(_f), bodyType(_type){}
        std::string method;
        std::string uri;
        std::string functionName;
        std::string bodyType;
    };

    void registerEndpoints(std::ofstream &outputFile);
    void addPostConstruct(std::ofstream &outputFile);
    void handleRestController(std::string &line);
    void handlePostConstruct(std::string &basicString);

    std::vector<EndpointData> endpointData;
    std::vector<std::string> restControllers;
    int bracketCounter;
    std::string routerCppFile;
    std::string postConstructMethodName;

    bool handleAutoWire(std::string &line, std::ofstream &outputFile);
};


#endif //WINTER_ANNOTATIONPASS_H
