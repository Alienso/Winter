//
// Created by Alienson on 6.2.2024..
//

#include "Field.h"
#include "Reflect.h"

#include "stringUtils.h"

Field Field::INVALID = {"","",FIELD_TYPE_INT, "", 0, false, false};

void Field::setValue(void *object, const void* data, const unsigned int size) const {
    memcpy(getAddress(object), data, size);
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
            setValueInternal<short>((short)stoi(value), object, &Field::setShort);
            break;
        case FIELD_TYPE_INT:
            setValueInternal<int>(stoi(value), object, &Field::setInt);
            break;
        case FIELD_TYPE_LONG:
            setValueInternal<long>(stol(value), object, &Field::setLong);
            break;
        case FIELD_TYPE_FLOAT:
            setValueInternal<float>(stof(value), object, &Field::setFloat);
            break;
        case FIELD_TYPE_DOUBLE:
            setValueInternal<double>(stod(value), object, &Field::setDouble);
            break;
        case FIELD_TYPE_CHAR:
            setValueInternal<char>(value[0], object, &Field::setChar);
            break;
        case FIELD_TYPE_BOOL:
            setValueInternal<bool>(StringUtils::parseBoolean(value), object, &Field::setBool);
            break;
        case FIELD_TYPE_BYTE:
            setValueInternal<byte>((byte)value[0], object, &Field::setByte);
            break;
        case FIELD_TYPE_STRING:
            if (this->isPtr){
                auto *valuePtr = new string(value);
                this->setPtr(object, valuePtr);
            }
            else
                this->setString(object, value);
            break;
        default:
            wtLogError("Unknown type encountered: ", type);
    }
}

void Field::copyValue(Reflect *source, const Field &sourceField, Reflect *dest, const Field &destField, CopyType copyType) {
    if (sourceField.type != destField.type){
        wtLogError("Source and Field types dont match!. Source: %d, Dest: %d", sourceField.type, destField.type);
        return;
    }

    //all_types_helper types;
    copyDetails copyDetails(source, sourceField, dest, destField, copyType);

    switch (destField.type) {
        case FIELD_TYPE_SHORT:
            copyValue<short>(copyDetails, &Field::getShort, &Field::setShort);
            break;
        case FIELD_TYPE_INT:
            copyValue<int>(copyDetails, &Field::getInt, &Field::setInt);
            break;
        case FIELD_TYPE_LONG:
            copyValue<long>(copyDetails, &Field::getLong, &Field::setLong);
            break;
        case FIELD_TYPE_FLOAT:
            copyValue<float>(copyDetails, &Field::getFloat, &Field::setFloat);
            break;
        case FIELD_TYPE_DOUBLE:
            copyValue<double>(copyDetails, &Field::getDouble, &Field::setDouble);
            break;
        case FIELD_TYPE_CHAR:
            copyValue<char>(copyDetails, &Field::getChar, &Field::setChar);
            break;
        case FIELD_TYPE_BOOL:
            copyValue<bool>(copyDetails, &Field::getBool, &Field::setBool);
            break;
        case FIELD_TYPE_BYTE:
            copyValue<byte>(copyDetails, &Field::getByte, &Field::setByte);
            break;
        case FIELD_TYPE_STRING:
            if (sourceField.isPtr) {
                string helper = **((string **) sourceField.getPtr(source));
                if (destField.isPtr) {
                    auto* sptr = new string(helper);
                    destField.setPtr(dest, sptr);
                }
                else destField.setString(dest, helper);
            }
            else {
                string helper = sourceField.getString(source);
                if (destField.isPtr) {
                    auto* sptr = new string(helper);
                    destField.setPtr(dest, sptr);
                }
                else destField.setString(dest, helper);
            }
            break;
        case FIELD_TYPE_VECTOR:
            destField.setValue(dest, sourceField.getAddress(source), 1); //TODO try getting size from vector<int> s
            break; //TODO!
        case FIELD_TYPE_OBJ:
            /*auto* sourceObj = static_cast<Reflect *>(sourceField.getAddress(source));
            auto* destObj = static_cast<Reflect *>(destField.getAddress(dest));
            *destObj = *sourceObj;*/
            //TODO call clone() here;
            if (sourceField.isPtr){
                if (destField.isPtr)
                    **((Reflect**)sourceField.getPtr(source)) = **((Reflect**)destField.getPtr(dest));
                else
                    **((Reflect**)sourceField.getPtr(source)) = *((Reflect*)destField.getAddress(dest));
            }else{
                if (destField.isPtr)
                    *((Reflect*)sourceField.getAddress(source)) = **((Reflect**)destField.getPtr(dest));
                else
                    *((Reflect*)sourceField.getAddress(source)) = *((Reflect*)destField.getAddress(dest));
            }
            //*((Reflect*)sourceField.getAddress(source)) = *((Reflect*)destField.getAddress(dest));
            break; //TODO which copy constructor is called
        case FIELD_TYPE_ARRAY:
            break; //TODO
        default:
            wtLogError("Unknown type encountered: ", destField.type);
    }
}
