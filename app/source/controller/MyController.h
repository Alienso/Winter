//
// Created by Alienson on 3.2.2024.
//

#ifndef WINTER_MYCONTROLLER_H
#define WINTER_MYCONTROLLER_H


#include "../repository/PlayerRepository.h"
#include "../service/MyService.h"
#include "http/ControllerMapping.h"


$RestController
class MyController : public Component {

    $Autowired
    MyService* myService;
    $Autowired
    PlayerRepository* playerRepository;

    $GET("/home")
    HttpResponse* home(HttpRequest* request);

    $GET("/player")
    HttpResponse* player(HttpRequest* request);

private:
    static JsonDeserializer deserializer;

};


#endif //WINTER_MYCONTROLLER_H
