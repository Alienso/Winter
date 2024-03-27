//
// Created by alienson on 22.3.24..
//

#ifndef WINTER_PLAYERENTITY_H
#define WINTER_PLAYERENTITY_H

#endif //WINTER_PLAYERENTITY_H

#include <string>
#include <Reflect.h>

//TODO type* defaults to reflect* in deserializer
class PlayerEntity : public Reflect {
public:
    long id;
    std::string name;
    std::string createdOn;
};