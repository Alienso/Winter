//
// Created by Alienson on 3.2.2024..
//

#ifndef WINTER_ROUTER_H
#define WINTER_ROUTER_H


#include <memory>
#include "http/HttpRequest.h"
#include "reflect/Reflect.h"
#include "mutex"
#include "http/HttpResponse.h"
#include "util/ThreadPool.h"

class Endpoint{
public:
    Endpoint() : method(nullptr), func(nullptr){}
    Endpoint(const char* url, HttpMethod* _method, HttpResponse* (*f)(HttpRequest* request)) : method(_method), uri(url), func(f){}
    HttpMethod* method;
    URI uri;
    HttpResponse* (*func)(HttpRequest* request);
};

class Router {
public:
    Router(Router& other) = delete;
    void operator=(const Router&) = delete;
    static Router* getInstance();

    void routeRequest(shared_ptr<HttpRequest> &request);
    void registerEndpoint(Endpoint* endpoint);
    void registerEndpoint(const char* url, HttpMethod* _method, HttpResponse* (*f)(HttpRequest* request));

    vector<Endpoint*> endpoints;

private:
    Router() = default;
    ~Router();
    ThreadPool threadPool{20};
    static Router* instance;
    static mutex mutex_;
};


#endif //WINTER_ROUTER_H
