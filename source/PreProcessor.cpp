//
// Created by Alienson on 3.2.2024..
//

#include "PreProcessor.h"

#include <filesystem>
#include <iostream>
#include <mutex>

#define WtGET(endpoint) PreProcessor::getInstane()->registerEndpoint(endpoint, HttpMethod::GET);

PreProcessor* PreProcessor::instance = nullptr;
mutex PreProcessor::mutex_;

PreProcessor *PreProcessor::getInstance() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (instance == nullptr)
    {
        instance = new PreProcessor();
    }
    return instance;
}

void PreProcessor::registerEndpoint(const char* endpoint, HttpMethod* method) {
    std::cout<<"Registering endpoint " << endpoint;
    this->endpoints.emplace_back(endpoint, method);
}
