//
// Created by Alienson on 3.2.2024..
//

#ifndef WINTER_MYCONTROLLER_H
#define WINTER_MYCONTROLLER_H


#include "../../app/service/MyService.h"
#include "../../http/ControllerMapping.h"



$RestController
class MyController : public Component {

    $Autowired
    MyService* myService;

    $GET("/home")
    HttpResponse* home(HttpRequest* request);

private:
    static JsonDeserializer deserializer;

};


#endif //WINTER_MYCONTROLLER_H
