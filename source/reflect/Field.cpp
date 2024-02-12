//
// Created by Alienson on 6.2.2024..
//

#include "Field.h"

Field Field::INVALID = {};

void *Field::getAddress(void *object) const {
    return (int*)(object) + offset;
}

void Field::setValue(void *object, void* data, unsigned int size) {
    memcpy(getAddress(object), data, size);
}

void Field::setInt(void *object, int value) {
    *((int*)getAddress(object)) = value;
}

void Field::setLong(void *object, long value) {
    *((long*)getAddress(object)) = value;
}

void Field::setChar(void *object, char value) {
    *((char*)getAddress(object)) = value;
}

void Field::setFloat(void *object, float value) {
    *((float*)getAddress(object)) = value;
}

void Field::setDouble(void *object, double value) {
    *((double*)getAddress(object)) = value;
}

void Field::setShort(void *object, short value) {
    *((short*)getAddress(object)) = value;
}

void Field::setString(void *object, std::string& value) {
    *((std::string*)getAddress(object)) = value;
}

void Field::setString(void *object, const char *value) {
    *((std::string*)getAddress(object)) = std::string(value);
}

void Field::setPtr(void *object, void *value) {
    *((int*)getAddress(object)) = (unsigned long long)value;
}
