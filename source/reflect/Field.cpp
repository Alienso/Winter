//
// Created by Alienson on 6.2.2024..
//

#include "Field.h"

Field Field::INVALID = {};

void Field::setValue(void *object, void* data, unsigned int size) const {
    memcpy(getAddress(object), data, size); //TODO this is shallow copy!
}

void Field::setInt(void *object, int value) const {
    *((int*)getAddress(object)) = value;
}

void Field::setLong(void *object, long value) const {
    *((long*)getAddress(object)) = value;
}

void Field::setChar(void *object, char value) const {
    *((char*)getAddress(object)) = value;
}

void Field::setFloat(void *object, float value) const {
    *((float*)getAddress(object)) = value;
}

void Field::setDouble(void *object, double value) const {
    *((double*)getAddress(object)) = value;
}

void Field::setShort(void *object, short value) const {
    *((short*)getAddress(object)) = value;
}

void Field::setString(void *object, std::string& value) const {
    *((std::string*)getAddress(object)) = value;
}

void Field::setString(void *object, const char *value) const {
    *((std::string*)getAddress(object)) = std::string(value);
}

void Field::setPtr(void *object, void *value) const {
    int** ptr = ((int**) getPtr(object));
    *ptr = (int*)value;
}

void *Field::getAddress(void *object) const {
    return (int*)(object) + offset;
}

void **Field::getPtr(void *object) const {
    return (void**)((int*)(object) + offset);
}

int Field::getInt(void* object) const {
    return *(int*)getAddress(object);
}

long Field::getLong(void* object) const {
    return *(long*)getAddress(object);
}

char Field::getChar(void* object) const {
    return *(char*)getAddress(object);
}

float Field::getFloat(void* object) const{
    return *(float*)getAddress(object);
}

double Field::getDouble(void* object) const{
    return *(double *)getAddress(object);
}

short Field::getShort(void* object) const{
    return *(short*)getAddress(object);
}

string Field::getString(void* object) const{
    return *(string*)getAddress(object);
}