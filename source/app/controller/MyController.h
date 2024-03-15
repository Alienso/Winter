//
// Created by Alienson on 3.2.2024..
//

#ifndef WINTER_MYCONTROLLER_H
#define WINTER_MYCONTROLLER_H


#include "../../http/ControllerMapping.h"



$RestController
class MyController : public Component {

    //this needs to be done post init since as components are being initialized and added to the array these assignments are happening
    MyController* myController;

    $GET("/home")
    static HttpResponse* home(HttpRequest* request);

    //$PostConstruct
    void postConstruct(){
        myController = (MyController *)(getById(MyController::_componentId_));
    }

private:
    static JsonDeserializer deserializer;

};


#endif //WINTER_MYCONTROLLER_H
