//
// Created by Alienson on 3.2.2024..
//

#ifndef WINTER_MYCONTROLLER_H
#define WINTER_MYCONTROLLER_H


#include "../../http/ControllerMapping.h"



$RestController
class MyController : public Component {

    MyController* myController = (MyController *)(getById(MyController::_componentId_));

    $GET("/home")
    static HttpResponse* home(HttpRequest* request);

    /*$PostConstruct
    void postConstruct(){
        myController = (MyController *)(getById(MyController::_componentId_));
    }*/

private:
    static JsonDeserializer deserializer;

};


#endif //WINTER_MYCONTROLLER_H
