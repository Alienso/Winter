//
// Created by alienson on 27.3.24..
//

#ifndef WINTER_MAPPER_H
#define WINTER_MAPPER_H

#include "Reflect.h"

enum FIELD_MATCH_TYPE{
    FIELD_MATCH_TYPE_STRICT,
    FILED_MATCH_TYPE_RELAXED
};


class Mapper {
public:
    Mapper() : failOnUnknownProperty(false) {}
    void map(Reflect* source, Reflect* dest) const;

    void failOnUnknownProperties(bool val);

private:
    bool failOnUnknownProperty;
};


#endif //WINTER_MAPPER_H
