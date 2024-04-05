//
// Created by Alienson on 14.2.2024..
//

#include "JsonSerializer.h"

string* JsonSerializer::serialize(Reflect* obj){
    auto* s = new string("{\n");
    size_t n = obj->getDeclaredFields().size();
    for (size_t i=0; i<n; i++){
        Field& field = obj->getDeclaredFields()[i];
        s->push_back('\"');
        s->append(field.name);
        s->append("\":");

        string* fieldValue = convertToJsonString(field, obj);
        s->append(*fieldValue);
        if (i != n - 1)
            s->push_back(',');
        s->push_back('\n');
        delete fieldValue;
    }

    s->push_back('}');
    return s;
}

string* JsonSerializer::convertToJsonString(const Field &field, Reflect* obj){
    switch (field.type) {
        case FIELD_TYPE_INT:
            return serializeField<int>(field, obj, &Field::getInt, to_string);
        case FIELD_TYPE_SHORT:
            return serializeField<short>(field, obj, &Field::getShort, StringUtils::to_string);
        case FIELD_TYPE_LONG:
            return serializeField<long>(field, obj, &Field::getLong, to_string);
        case FIELD_TYPE_FLOAT:
            return serializeField<float>(field, obj, &Field::getFloat, to_string);
        case FIELD_TYPE_DOUBLE:
            return serializeField<double>(field, obj, &Field::getDouble, to_string);
        case FIELD_TYPE_BOOL:
            return serializeField<bool>(field, obj, &Field::getBool, StringUtils::to_string);
        case FIELD_TYPE_CHAR:
            if (!field.isPtr) {
                char c = field.getChar(obj);
                return new string("\"" + to_string(c) + "\"");
            }
            else {
                char* cPtr = *((char **) field.getPtr(obj));
                if(cPtr == nullptr)
                    return new string("null");
                char c = **((char **) field.getPtr(obj));
                return new string("\"" + to_string(c) + "\"");
            }

        case FIELD_TYPE_STRING:
            if (!field.isPtr) {
                string str = field.getString(obj);
                return new string("\"" + str + "\"");
            }
            else {
                string* str = *((string**)field.getPtr(obj));
                if (str == nullptr)
                    return new string("null");
                return new string ("\"" + *str + "\"");
            }
        case FIELD_TYPE_OBJ:
            if (!field.isPtr)
                return serialize((Reflect*)field.getAddress(obj));
            else{
                Reflect* ptr = *((Reflect**)field.getPtr(obj));
                if (ptr == nullptr) return new string("null");
                return serialize(ptr);
            }
        //case FIELD_TYPE_ARRAY:
        case FIELD_TYPE_VECTOR:
            return convertVectorToJsonString(field, obj);
        default:
            wtLogError("Unknown fieldType: %d", field.type);
            return new string("\"\"");
    }
}

string* JsonSerializer::convertVectorToJsonString(const Field &f, Reflect *obj) {

    bool isElemPtr = false;
    string subTypeStr;
    getArraySubType(f.typeStr, subTypeStr, &isElemPtr);

    FieldType subType = convertToFieldType(subTypeStr);

    switch (subType) {
        case FIELD_TYPE_INT:
            return handleVecData<int>(f, obj, isElemPtr, to_string);
        case FIELD_TYPE_SHORT:
            return handleVecData<short>(f, obj, isElemPtr, StringUtils::to_string);
        case FIELD_TYPE_LONG:
            return handleVecData<long>(f, obj, isElemPtr, to_string);
        case FIELD_TYPE_FLOAT:
            return handleVecData<float>(f, obj, isElemPtr, to_string);
        case FIELD_TYPE_DOUBLE:
            return handleVecData<double>(f, obj, isElemPtr, to_string);
        case FIELD_TYPE_BOOL:
            return handleVecData<bool>(f, obj, isElemPtr, StringUtils::to_string);
        case FIELD_TYPE_CHAR:
            if (!f.isPtr) {
                if (!isElemPtr) {
                    auto* vec = static_cast<vector<char> *>(f.getAddress(obj));
                    return vectorToString(*vec);
                }else{
                    auto* vecPtr = static_cast<vector<char*> *>(f.getAddress(obj));
                    return vectorPtrToString(*vecPtr);
                }
            }else {
                if (!isElemPtr) {
                    auto* vec = static_cast<vector<char> *>(*(f.getPtr(obj)));
                    return vectorToString(*vec);
                }else{
                    auto* vecPtr = static_cast<vector<char*> *>(*(f.getPtr(obj)));
                    return vectorPtrToString(*vecPtr);
                }
            }
        case FIELD_TYPE_STRING:
            if (!f.isPtr) {
                if (!isElemPtr) {
                    auto* vec = static_cast<vector<string> *>(f.getAddress(obj));
                    return vectorToString(*vec);
                }else{
                    auto* vecPtr = static_cast<vector<string*> *>(f.getAddress(obj));
                    return vectorPtrToString(*vecPtr);
                }
            }else {
                if (!isElemPtr) {
                    auto* vec = static_cast<vector<string> *>(*(f.getPtr(obj)));
                    return vectorToString(*vec);
                }else{
                    auto* vecPtr = static_cast<vector<string*> *>(*(f.getPtr(obj)));
                    return vectorPtrToString(*vecPtr);
                }
            }
        case FIELD_TYPE_OBJ:
            if (!f.isPtr) {
                if (!isElemPtr) {
                    auto *vecPtr = static_cast<vector<std::byte> *>(f.getAddress(obj));
                    return vectorToString(*vecPtr);
                }else{
                    auto *vecPtr = static_cast<vector<Reflect*> *>(f.getAddress(obj));
                    return vectorToString(*vecPtr);
                }
            }else {
                if (!isElemPtr){
                    auto* vecPtr = static_cast<vector<std::byte> *>(*(f.getPtr(obj)));
                    if (vecPtr == nullptr)
                        return new string("[]");
                    return vectorToString(*vecPtr);
                }else{
                    auto* vecPtr = static_cast<vector<Reflect*> *>(*(f.getPtr(obj)));
                    if (vecPtr == nullptr)
                        return new string("[]");
                    return vectorToString(*vecPtr);
                }
            }
        default:
            wtLogError("Unknown FieldType Type %d in vec", subType);
            return new string("\"\"");
    }

}

string* JsonSerializer::vectorToString(const vector<std::byte>& source) {

    size_t vecSize = source.size();
    if (vecSize == 0){
        return new string("[]");
    }

    auto* elemPtr = (Reflect*)(&(source[0]));
    size_t elemSize = elemPtr->getClassSize();
    size_t offset = 0;

    auto* res = new string("[");
    while(offset < vecSize){
        elemPtr = (Reflect*)(&(source[offset]));

        *res+=*serialize(elemPtr);
        *res+=',';

        offset += elemSize;
    }
    (*res)[res->size()-1] = ']';
    return res;

}

string* JsonSerializer::vectorToString(const vector<Reflect*>& source) {

    if (source.empty())
        return new string("[]");

    auto* res = new string("[");
    for(Reflect* x : source){
        *res+=*serialize(x);
        *res+=',';
    }
    (*res)[res->size()-1] = ']';
    return res;
}

string* JsonSerializer::vectorToString(const vector<char>& source) {

    if (source.empty())
        return new string("[]");

    auto* res = new string("[");
    for(char c : source){
        *res += "\"";
        res->push_back(c);
        *res += "\",";
    }
    (*res)[res->size()-1] = ']';
    return res;
}

string* JsonSerializer::vectorToString(const vector<string>& source) {

    if (source.empty())
        return new string("[]");

    auto* res = new string("[");
    for(const string& s : source){
        *res += "\"" + s + "\",";
    }
    (*res)[res->size()-1] = ']';
    return res;
}

string* JsonSerializer::vectorPtrToString(const vector<char*>& source) {

    if (source.empty())
        return new string("[]");

    auto* res = new string("[");
    for(char *c : source){
        *res += "\"";
        res->push_back(*c);
        *res += "\",";
    }
    (*res)[res->size()-1] = ']';
    return res;
}

string* JsonSerializer::vectorPtrToString(const vector<string*>& source) {

    if (source.empty())
        return new string("[]");

    auto* res = new string("[");
    for(string* s : source){
        *res += "\"" + *s + "\",";
    }
    (*res)[res->size()-1] = ']';
    return res;
}
