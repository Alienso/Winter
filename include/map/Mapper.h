//
// Created by alienson on 27.3.24.
//

#ifndef WINTER_MAPPER_H
#define WINTER_MAPPER_H

#include "Reflect.h"

/*
 * Strict = exact match
 * Relaxed = letters are the same
 */
enum FieldMatchType{
    FIELD_MATCH_TYPE_STRICT,
    FIELD_MATCH_TYPE_RELAXED
};


class Mapper {
public:
    Mapper() : failOnUnknownProperty(false) {}
    void map(Reflect* source, Reflect* dest, FieldMatchType = FIELD_MATCH_TYPE_RELAXED) const;

    void failOnUnknownProperties(bool val);

private:
    bool failOnUnknownProperty;
};


#endif //WINTER_MAPPER_H
