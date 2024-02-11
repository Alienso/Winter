//
// Created by Alienson on 6.2.2024..
//

#ifndef WINTER_FIELD_H
#define WINTER_FIELD_H

#include <string>

class Field {
public:

    Field(std::string& _name, std::string& _type, std::string& _className, int _offset) : name(_name), type(_type), className(_className), offset(_offset){}
    Field(const char* _name, const char* _type, const char* _className, int _offset) : name(_name), type(_type), className(_className), offset(_offset){}
    Field() : offset(0){}

    void* getAddress(void* object) const;

    std::string name;
    std::string type;
    std::string className;
    int offset;
};


#endif //WINTER_FIELD_H
