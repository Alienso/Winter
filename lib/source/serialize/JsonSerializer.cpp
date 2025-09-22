//
// Created by Alienson on 14.2.2024.
//

#include "serialize/JsonSerializer.h"

std::string* JsonSerializer::serialize(Reflect* obj){
    auto* s = new std::string("{\n");
    size_t n = obj->getDeclaredFields().size();
    for (size_t i=0; i<n; i++){
        Field& field = obj->getDeclaredFields()[i];
        s->push_back('\"');
        s->append(field.name);
        s->append("\":");

        std::string* fieldValue = convertToJsonString(field, obj);
        s->append(*fieldValue);
        if (i != n - 1)
            s->push_back(',');
        s->push_back('\n');
        delete fieldValue;
    }

    s->push_back('}');
    return s;
}

std::string* JsonSerializer::convertToJsonString(const Field &field, Reflect* obj){
    switch (field.type) {
        case FIELD_TYPE_INT:
            return serializeField<int>(field, obj, &Field::getInt, std::to_string);
        case FIELD_TYPE_SHORT:
            return serializeField<short>(field, obj, &Field::getShort, StringUtils::to_string);
        case FIELD_TYPE_LONG:
            return serializeField<long>(field, obj, &Field::getLong, std::to_string);
        case FIELD_TYPE_FLOAT:
            return serializeField<float>(field, obj, &Field::getFloat, std::to_string);
        case FIELD_TYPE_DOUBLE:
            return serializeField<double>(field, obj, &Field::getDouble, std::to_string);
        case FIELD_TYPE_BOOL:
            return serializeField<bool>(field, obj, &Field::getBool, StringUtils::to_string);
        case FIELD_TYPE_CHAR:
            if (!field.isPtr) {
                char c = field.getChar(obj);
                return new std::string("\"" + std::to_string(c) + "\"");
            }
            else {
                char* cPtr = *((char **) field.getPtr(obj));
                if(cPtr == nullptr)
                    return new std::string("null");
                char c = **((char **) field.getPtr(obj));
                return new std::string("\"" + std::to_string(c) + "\"");
            }

        case FIELD_TYPE_STRING:
            if (!field.isPtr) {
                std::string str = field.getString(obj);
                return new std::string("\"" + str + "\"");
            }
            else {
                std::string* str = *((std::string**)field.getPtr(obj));
                if (str == nullptr)
                    return new std::string("null");
                return new std::string ("\"" + *str + "\"");
            }
        case FIELD_TYPE_OBJ:
            if (!field.isPtr)
                return serialize((Reflect*)field.getAddress(obj));
            else{
                Reflect* ptr = *((Reflect**)field.getPtr(obj));
                if (ptr == nullptr) return new std::string("null");
                return serialize(ptr);
            }
        //case FIELD_TYPE_ARRAY:
        case FIELD_TYPE_VECTOR:
            return convertVectorToJsonString(field, obj);
        default:
            wtLogError("Unknown fieldType: {}", field.type);
            return new std::string("\"\"");
    }
}

std::string* JsonSerializer::convertVectorToJsonString(const Field &f, Reflect *obj) {

    bool isElemPtr = false;
    std::string subTypeStr;
    getArraySubType(f.typeStr, subTypeStr, &isElemPtr);

    FieldType subType = convertToFieldType(subTypeStr);

    switch (subType) {
        case FIELD_TYPE_INT:
            return handleVecData<int>(f, obj, isElemPtr, std::to_string);
        case FIELD_TYPE_SHORT:
            return handleVecData<short>(f, obj, isElemPtr, StringUtils::to_string);
        case FIELD_TYPE_LONG:
            return handleVecData<long>(f, obj, isElemPtr, std::to_string);
        case FIELD_TYPE_FLOAT:
            return handleVecData<float>(f, obj, isElemPtr, std::to_string);
        case FIELD_TYPE_DOUBLE:
            return handleVecData<double>(f, obj, isElemPtr, std::to_string);
        case FIELD_TYPE_BOOL:
            return handleVecData<bool>(f, obj, isElemPtr, StringUtils::to_string);
        case FIELD_TYPE_CHAR:
            if (!f.isPtr) {
                if (!isElemPtr) {
                    auto* vec = static_cast<std::vector<char> *>(f.getAddress(obj));
                    return vectorToString(*vec);
                }else{
                    auto* vecPtr = static_cast<std::vector<char*> *>(f.getAddress(obj));
                    return vectorPtrToString(*vecPtr);
                }
            }else {
                if (!isElemPtr) {
                    auto* vec = static_cast<std::vector<char> *>(*(f.getPtr(obj)));
                    return vectorToString(*vec);
                }else{
                    auto* vecPtr = static_cast<std::vector<char*> *>(*(f.getPtr(obj)));
                    return vectorPtrToString(*vecPtr);
                }
            }
        case FIELD_TYPE_STRING:
            if (!f.isPtr) {
                if (!isElemPtr) {
                    auto* vec = static_cast<std::vector<std::string> *>(f.getAddress(obj));
                    return vectorToString(*vec);
                }else{
                    auto* vecPtr = static_cast<std::vector<std::string*> *>(f.getAddress(obj));
                    return vectorPtrToString(*vecPtr);
                }
            }else {
                if (!isElemPtr) {
                    auto* vec = static_cast<std::vector<std::string> *>(*(f.getPtr(obj)));
                    return vectorToString(*vec);
                }else{
                    auto* vecPtr = static_cast<std::vector<std::string*> *>(*(f.getPtr(obj)));
                    return vectorPtrToString(*vecPtr);
                }
            }
        case FIELD_TYPE_OBJ:
            if (!f.isPtr) {
                if (!isElemPtr) {
                    auto *vecPtr = static_cast<std::vector<std::byte> *>(f.getAddress(obj));
                    return vectorToString(*vecPtr);
                }else{
                    auto *vecPtr = static_cast<std::vector<Reflect*> *>(f.getAddress(obj));
                    return vectorToString(*vecPtr);
                }
            }else {
                if (!isElemPtr){
                    auto* vecPtr = static_cast<std::vector<std::byte> *>(*(f.getPtr(obj)));
                    if (vecPtr == nullptr)
                        return new std::string("[]");
                    return vectorToString(*vecPtr);
                }else{
                    auto* vecPtr = static_cast<std::vector<Reflect*> *>(*(f.getPtr(obj)));
                    if (vecPtr == nullptr)
                        return new std::string("[]");
                    return vectorToString(*vecPtr);
                }
            }
        default:
            wtLogError("Unknown FieldType Type {} in vec", subType);
            return new std::string("\"\"");
    }

}

std::string* JsonSerializer::vectorToString(const std::vector<std::byte>& source) {

    size_t vecSize = source.size();
    if (vecSize == 0){
        return new std::string("[]");
    }

    auto* elemPtr = (Reflect*)(&(source[0]));
    size_t elemSize = elemPtr->getClassSize();
    size_t offset = 0;

    auto* res = new std::string("[");
    while(offset < vecSize){
        elemPtr = (Reflect*)(&(source[offset]));

        *res+=*serialize(elemPtr);
        *res+=',';

        offset += elemSize;
    }
    (*res)[res->size()-1] = ']';
    return res;

}

std::string* JsonSerializer::vectorToString(const std::vector<Reflect*>& source) {

    if (source.empty())
        return new std::string("[]");

    auto* res = new std::string("[");
    for(Reflect* x : source){
        *res+=*serialize(x);
        *res+=',';
    }
    (*res)[res->size()-1] = ']';
    return res;
}

std::string* JsonSerializer::vectorToString(const std::vector<char>& source) {

    if (source.empty())
        return new std::string("[]");

    auto* res = new std::string("[");
    for(char c : source){
        *res += "\"";
        res->push_back(c);
        *res += "\",";
    }
    (*res)[res->size()-1] = ']';
    return res;
}

std::string* JsonSerializer::vectorToString(const std::vector<std::string>& source) {

    if (source.empty())
        return new std::string("[]");

    auto* res = new std::string("[");
    for(const std::string& s : source){
        *res += "\"" + s + "\",";
    }
    (*res)[res->size()-1] = ']';
    return res;
}

std::string* JsonSerializer::vectorPtrToString(const std::vector<char*>& source) {

    if (source.empty())
        return new std::string("[]");

    auto* res = new std::string("[");
    for(char *c : source){
        *res += "\"";
        res->push_back(*c);
        *res += "\",";
    }
    (*res)[res->size()-1] = ']';
    return res;
}

std::string* JsonSerializer::vectorPtrToString(const std::vector<std::string*>& source) {

    if (source.empty())
        return new std::string("[]");

    auto* res = new std::string("[");
    for(std::string* s : source){
        *res += "\"" + *s + "\",";
    }
    (*res)[res->size()-1] = ']';
    return res;
}
