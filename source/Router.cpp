//
// Created by Alienson on 3.2.2024..
//

#include "Router.h"
#include "http/HttpResponse.h"

Router* Router::instance = nullptr;
mutex Router::mutex_;

Router *Router::getInstance() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (instance == nullptr)
        instance = new Router();
    return instance;
}

//endpoint->g(endpoint->f, request);
//HttpResponse response = HttpResponse::generateResponse(request);
void Router::routeRequest(shared_ptr<HttpRequest> &request) {
    for (auto endpoint : endpoints){
        if (endpoint->method == request->getMethod() && endpoint->uri == request->getUri()){
            threadPool.queueTask([endpoint, request]{
                HttpResponse* response = endpoint->func(request.get());
                response->send();
                delete response;
            });
            return; //TODO add method not supported
        }
    }

    //TODO add 404;
}

void Router::registerEndpoint(Endpoint *endpoint) {
    endpoints.push_back(endpoint);
}

void Router::registerEndpoint(const char *url, HttpMethod *method, HttpResponse *(*f)(HttpRequest *)) {
    auto* endpoint = new Endpoint(url, method, f);
    endpoints.push_back(endpoint);
}

Router::~Router() {
    for (auto x: endpoints){
        delete x;
    }
}
