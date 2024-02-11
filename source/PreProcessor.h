//
// Created by Alienson on 3.2.2024..
//

#ifndef WINTER_PREPROCESSOR_H
#define WINTER_PREPROCESSOR_H

#include <vector>
#include <string>
#include <mutex>

#include "http/httpConstants.h"

using namespace std;

struct endpoint{
    string name;
    HttpMethod* method;

    endpoint(const char* _endpoint, HttpMethod* _method){
        name = string(_endpoint);
        method = _method;
    }
};

class PreProcessor {
public:
    PreProcessor(PreProcessor& other) = delete;
    void operator=(const PreProcessor&) = delete;
    static PreProcessor* getInstance();

    void registerEndpoint(const char* endpoint, HttpMethod* method);

private:
    PreProcessor() = default;
    static PreProcessor* instance;
    static mutex mutex_;

    vector<endpoint> endpoints;
};


#endif //WINTER_PREPROCESSOR_H
