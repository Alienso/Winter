//
// Created by Alienson on 6.2.2024..
//

#include "Field.h"
#include "Logger.h"
#include "Reflect.h"

#include "stringUtils.h"

Field Field::INVALID = {"","",FIELD_TYPE_INT, "", 0};

void Field::setValue(void *object, const void* data, const unsigned int size) const {
    memcpy(getAddress(object), data, size); //TODO this is shallow copy!
}

void Field::setInt(void *object, const int value) const {
    *((int*)getAddress(object)) = value;
}

void Field::setLong(void *object, const long value) const {
    *((long*)getAddress(object)) = value;
}

void Field::setChar(void *object, const char value) const {
    *((char*)getAddress(object)) = value;
}

void Field::setFloat(void *object, const float value) const {
    *((float*)getAddress(object)) = value;
}

void Field::setDouble(void *object, const double value) const {
    *((double*)getAddress(object)) = value;
}

void Field::setShort(void *object, const short value) const {
    *((short*)getAddress(object)) = value;
}

void Field::setBool(void *object, const bool value) const {
    *((bool*)getAddress(object)) = value;
}

void Field::setByte(void *object, const byte value) const {
    *((byte*)getAddress(object)) = value;
}

void Field::setString(void *object, const std::string& value) const {
    *((std::string*)getAddress(object)) = value;
}

void Field::setString(void *object, const char *value) const {
    *((std::string*)getAddress(object)) = std::string(value);
}

void Field::setPtr(void *object, const void *value) const {
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

bool Field::getBool(void* object) const{
    return *(bool*)getAddress(object);
}

byte Field::getByte(void* object) const{
    return *(byte*)getAddress(object);
}

string Field::getString(void* object) const{
    return *(string*)getAddress(object);
}


void Field::set(void *object, const char *value) const {
    switch (type) {
        case FIELD_TYPE_SHORT:
            setShort(object, (short)stoi(value));
            break;
        case FIELD_TYPE_INT:
            setInt(object, stoi(value));
            break;
        case FIELD_TYPE_LONG:
            setLong(object, stol(value));
            break;
        case FIELD_TYPE_FLOAT:
            setFloat(object, stof(value));
            break;
        case FIELD_TYPE_DOUBLE:
            setDouble(object, stod(value));
            break;
        case FIELD_TYPE_CHAR:
            setChar(object, value[0]);
            break;
        case FIELD_TYPE_STRING:
            setString(object, value);
            break;
        case FIELD_TYPE_BOOL:
            setBool(object, StringUtils::parseBoolean(value));
            break;
        case FIELD_TYPE_BYTE:
            setByte(object, (byte)value[0]);
            break;
        default:
            wtLogError("Unknown type encountered: ", type);
    }
}

void Field::copyValue(Reflect *source, Field &sourceField, Reflect *dest, Field &destField) {
    if (sourceField.type != destField.type){
        wtLogError("Source and Field types dont match!. Source: %d, Dest: %d", sourceField.type, destField.type);
        return;
    }

    switch (destField.type) {
        case FIELD_TYPE_SHORT:
            destField.setShort(dest, sourceField.getShort(source));
            break;
        case FIELD_TYPE_INT:
            destField.setInt(dest, sourceField.getInt(source));
            break;
        case FIELD_TYPE_LONG:
            destField.setLong(dest, sourceField.getLong(source));
            break;
        case FIELD_TYPE_FLOAT:
            destField.setFloat(dest, sourceField.getFloat(source));
            break;
        case FIELD_TYPE_DOUBLE:
            destField.setDouble(dest, sourceField.getDouble(source));
            break;
        case FIELD_TYPE_CHAR:
            destField.setChar(dest, sourceField.getChar(source));
            break;
        case FIELD_TYPE_STRING:
            destField.setString(dest, sourceField.getString(source));
            break;
        case FIELD_TYPE_BOOL:
            destField.setBool(dest, sourceField.getBool(source));
            break;
        case FIELD_TYPE_BYTE:
            destField.setByte(dest, sourceField.getByte(source));
            break;
        case FIELD_TYPE_PTR:
            destField.setPtr(dest, sourceField.getPtr(source)); //TODO allocate new ptr?
            break;
        case FIELD_TYPE_VECTOR:
        case FIELD_TYPE_ARRAY:
            break; //TODO!
        case FIELD_TYPE_OBJ:
            break; //TODO
        default:
            wtLogError("Unknown type encountered: ", destField.type);
    }
}
