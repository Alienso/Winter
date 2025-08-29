//
// Created by alienson on 22.3.24.
//

#ifndef WINTER_PLAYERENTITY_H
#define WINTER_PLAYERENTITY_H

#endif //WINTER_PLAYERENTITY_H

#include <string>
#include <Reflect.h>
#include <../sql/Entity.h>

class PlayerEntity : public Entity {
public:

    $Column("id")
    long id;

    $Column("name")
    std::string name;

    $Column("created_on")
    std::string createdOn;

    [[nodiscard]] std::string getTableName() const override {
        return "player";
    }

    [[nodiscard]] std::string getPrimaryKeyName() const override {
        return "id";
    }
};