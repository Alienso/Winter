//
// Created by Alienson on 11.2.2024.
//

#include "JsonDeserializer.h"

Reflect* JsonDeserializer::deserialize(const std::string& s, Reflect* response){
    std::string fieldName, fieldValue;
    JsonFieldType fieldType;

    size_t offset;
    size_t semicolonIndex;
    size_t commaIndex;

    int bracketCounter = 0, largeBracketCounter = 0;
    bool inStr = false;

    offset = s.find('{');
    if (offset == std::string::npos){
        wtLogWarn("Error reading json. Could not find {");
        return response;
    }
    offset++;

    while(true){

        semicolonIndex = s.find(':', offset);
        if (semicolonIndex == std::string::npos)
            return response;

        commaIndex = std::string::npos;
        size_t i;
        for (i=semicolonIndex; i<s.size(); i++){
            switch (s[i]) {
                case '[':
                    bracketCounter++;
                    break;
                case '{':
                    largeBracketCounter++;
                    break;
                case ']':
                    bracketCounter--;
                    break;
                case '}':
                    largeBracketCounter--;
                    break;
                default:
                    break;
            }
            if (s[i] == '"' && s[i-1] != '\\')
                inStr = !inStr;
            if (bracketCounter == 0 && largeBracketCounter == 0 && !inStr && s[i] == ',') {
                commaIndex = i;
                break;
            }
        }

        if (commaIndex == std::string::npos || i >= s.size()) {
            commaIndex = s.rfind('}', s.size());
            if (commaIndex == std::string::npos){
                wtLogError("Could not find } in json!");
                continue;
            }
        }

        fieldName = s.substr(offset, semicolonIndex - offset);
        fieldValue = s.substr(semicolonIndex + 1, commaIndex - semicolonIndex - 1);

        fieldName = StringUtils::trim(fieldName);
        fieldName = fieldName.substr(1,fieldName.size() - 2); //remove ""
        fieldValue = StringUtils::trim(fieldValue);

        offset = commaIndex + 1;

        fieldType = getJsonFieldType(fieldValue);
        Field* f = response->getField(fieldName.data());
        if (f == &Field::INVALID){
            continue;
        }
        FieldType expectedType = f->type;

        if (!areTypesCompatible(fieldType, expectedType)) {
            wtLogError("Incompatible types: %d and %d for field %s", fieldType, expectedType, fieldName.data());
            return response;
        }

        if(fieldType == JSON_FILED_TYPE_STRING) //remove ""
            fieldValue = fieldValue.substr(1, fieldValue.size() - 2);

        if (fieldType == JSON_FIELD_TYPE_ARRAY) {
            FieldType fieldSubType = getArraySubType(f->typeStr);
            JsonFieldType jsonFieldSubType = getJsonFieldSubType(fieldValue);
            if (!areTypesCompatible(jsonFieldSubType, fieldSubType)) {
                wtLogError("Incompatible vec sub types: %d and %d for field %s", jsonFieldSubType, expectedType, fieldName.data());
                return response;
            }
            setFieldValueArray(fieldValue, expectedType, response, f);
        }
        else {
            //setFieldValue(fieldValue, expectedType, response, f);
            if (expectedType == FIELD_TYPE_OBJ){
                setObjectValue(f, fieldValue, response);
            }else{
                f->set(response, fieldValue.data());
            }

        }
    }
}

void JsonDeserializer::setObjectValue(Field* f, const std::string& fieldValue, Reflect* response){
    std::string className;
    bool isPtr = false;
    Reflect* tempObj;

    if (f->typeStr[f->typeStr.size() - 1] == '*') {
        isPtr = true;
        className = f->typeStr.substr(0, f->typeStr.size() - 1);
        tempObj = deserialize(fieldValue, Reflect::getClassInstanceByName(className));
    }else{
        deserialize(fieldValue, static_cast<Reflect *>(f->getAddress(response)));
        //tempObj = deserialize(fieldValue, Reflect::getClassInstanceByName(f->typeStr));
    }


    if (isPtr){
        f->setPtr(response, tempObj);
    }else{
        //This is not needed since deserialize sets data in-place
        //f->setValue(response, tempObj, tempObj->getClassSize());
        //tempObj->shallowFree();
    }
}

void JsonDeserializer::setFieldValueArray(const std::string& fieldValue, const FieldType fieldType, Reflect* obj, const Field* f){

    bool isElemPtr = false;
    std::string subTypeStr;
    getArraySubType(f->typeStr, subTypeStr, &isElemPtr);

    FieldType subType = convertToFieldType(subTypeStr);

    switch(subType){
        case FIELD_TYPE_INT:
            if (fieldType == FIELD_TYPE_VECTOR){
                handleVecData<int>(f, obj, isElemPtr, fieldValue, [](std::string &val){ return stoi(val);});
            }/*else {
                int *array = nullptr;
                unsigned int size;
                parseArrayData<int>(fieldValue, [](string &val){ return stoi(val);}, array, &size);
                f->setValue(obj, array, size * sizeof(int));
            }*/
            break;
        case FIELD_TYPE_SHORT:
            if (fieldType == FIELD_TYPE_VECTOR){
                handleVecData<short>(f, obj, isElemPtr, fieldValue, [](std::string &val){ return (short)stoi(val); });
            }
            break;
        case FIELD_TYPE_LONG:
            if (fieldType == FIELD_TYPE_VECTOR){
                handleVecData<long>(f, obj, isElemPtr, fieldValue, [](std::string &val){ return stol(val);});
            }
            break;
        case FIELD_TYPE_CHAR:
            if (fieldType == FIELD_TYPE_VECTOR){
                handleVecData<char>(f, obj, isElemPtr, fieldValue, [](std::string &val){ return val[0]; });
            }
            f->setChar(obj, fieldValue[0]);
            break;
        case FIELD_TYPE_FLOAT:
            if (fieldType == FIELD_TYPE_VECTOR){
                handleVecData<float>(f, obj, isElemPtr, fieldValue, [](std::string &val){ return stof(val); });
            }
            break;
        case FIELD_TYPE_DOUBLE:
            if (fieldType == FIELD_TYPE_VECTOR){
                handleVecData<double>(f, obj, isElemPtr, fieldValue, [](std::string &val){ return stod(val); });
            }
            break;
        case FIELD_TYPE_BOOL:
            if (fieldType == FIELD_TYPE_VECTOR){
                handleVecData<bool>(f, obj, isElemPtr, fieldValue, [](std::string &val){ return StringUtils::parseBoolean(val.data()); });
            }
            break;
        case FIELD_TYPE_STRING: //TODO try optimize
            if (fieldType == FIELD_TYPE_VECTOR){
                handleVecData<std::string>(f, obj, isElemPtr, fieldValue, [](std::string &val){ return val.substr(1, val.size() - 2); });
            }
            break;
        case FIELD_TYPE_OBJ:
            if (fieldType == FIELD_TYPE_VECTOR){
                if (!f->isPtr) {
                    if (!isElemPtr){
                        auto *data = (std::vector<std::byte>*)(f->getAddress(obj));
                        insertVectorData(fieldValue, data, subTypeStr);
                    }else {
                        auto *data = static_cast<std::vector<Reflect *> *>(f->getAddress(obj));
                        insertVectorPtrData(fieldValue, data, subTypeStr);
                    }
                }else{
                    if (!isElemPtr){
                        auto *data = new std::vector<std::byte>(); //static_cast<vector<Reflect *> *> (*(f->getPtr(obj)));
                        insertVectorData(fieldValue, data, subTypeStr);
                        f->setPtr(obj, data);
                    }else {
                        auto *data = new std::vector<Reflect *>(); //static_cast<vector<Reflect *> *> (*(f->getPtr(obj)));
                        insertVectorPtrData(fieldValue, data, subTypeStr);
                        f->setPtr(obj, data);
                    }
                }
            }
            break;
        default:
            wtLogError("Unknown FieldType Type %d in vec", subType);
            f->setInt(obj, 0);
            break;
    }
}

void JsonDeserializer::insertVectorData(const std::string& source, std::vector<std::byte> *dest, const std::string &typeStr){
    /* TLDR we are doing a memcpy into dest vector*/
    std::vector<std::string>* vec = StringUtils::splitObjectArray(source);
    *dest = {};
    for(std::string& s : *vec){
        Reflect* r = deserialize(s, Reflect::getClassInstanceByName(typeStr));
        /*dest->resize(currentSize + r->getClassSize());
        memcpy((void*)(&dest[currentSize]), (const void*)(r), r->getClassSize());*/
        std::byte buffer[r->getClassSize()];
        memcpy(buffer, (void*)r, r->getClassSize());
        dest->insert(dest->end(), &buffer[0], &buffer[r->getClassSize()]);
    }
}


void JsonDeserializer::insertVectorPtrData(const std::string& source, std::vector<Reflect*> *dest, const std::string &typeStr){
    std::vector<std::string>* vec = StringUtils::splitObjectArray(source);
    for(std::string& s : *vec){
        Reflect* r = deserialize(s, Reflect::getClassInstanceByName(typeStr));
        dest->push_back(r);
    }
}
