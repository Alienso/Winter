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

void Router::routeRequest(shared_ptr<HttpRequest> &request) {
    for (auto endpoint : endpoints){
        if (endpoint->uri == request->getUri()){
            if (endpoint->method == request->getMethod()) {
                threadPool.queueTask([endpoint, request] {
                    HttpResponse *response = endpoint->func(request.get());
                    response->setConnection(request->getConnection());
                    response->send();
                    delete response;
                });
                return;
            } else {
                HttpResponse httpResponse(HttpCode::METHOD_NOT_ALLOWED);
                httpResponse.setConnection(request->getConnection());
                httpResponse.send();
                return;
            }
        }
    }

    HttpResponse httpResponse(HttpCode::NOT_FOUND);
    httpResponse.setConnection(request->getConnection());
    httpResponse.send();
}

void Router::registerEndpoint(Endpoint *endpoint) {
    wtLogTrace("Registered endpoint %s", endpoint->uri.getPath().data());
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
