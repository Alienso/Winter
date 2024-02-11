//
// Created by Alienson on 3.2.2024..
//

#ifndef WINTER_ROUTER_H
#define WINTER_ROUTER_H


#include <memory>
#include "http/HttpRequest.h"

class Router {
public:
    void routeRequest(shared_ptr<HttpRequest> &request);
};


#endif //WINTER_ROUTER_H
