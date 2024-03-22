//
// Created by Alienson on 11.2.2024..
//

#include "JsonDeserializer.h"

Reflect* JsonDeserializer::deserialize(const string& s, Reflect* response){
    string fieldName, fieldValue;
    JsonFieldType fieldType;

    size_t offset;
    size_t semicolonIndex;
    size_t commaIndex;

    int bracketCounter = 0, largeBracketCounter = 0;
    bool inStr = false;

    offset = s.find('{');
    if (offset == string::npos){
        wtLogWarn("Error reading json. Could not find {");
        return response;
    }
    offset++;

    while(true){

        semicolonIndex = s.find(':', offset);
        if (semicolonIndex == string::npos)
            return response;

        commaIndex = string::npos;
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

        if (commaIndex == string::npos || i >= s.size()) {
            commaIndex = s.rfind('}', s.size());
            if (commaIndex == string::npos){
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

        fieldType = convertToJsonFieldType(fieldValue);
        Field* f = response->getField(fieldName.data());
        if (f == &Field::INVALID){
            continue;
        }
        FieldType expectedType = f->type;

        if (areTypesCompatible(fieldType, expectedType)) {

            if(fieldType == JSON_FILED_TYPE_STRING)
                fieldValue = fieldValue.substr(1, fieldValue.size() - 2);

            if (fieldType == JSON_FIELD_TYPE_ARRAY) {
                FieldType subType = getArraySubFieldType(f->typeStr);
                string subTypeStr = getArraySubType(f->typeStr);
                string typeStr = subTypeStr.find('*') != string::npos ? subTypeStr.substr(0,subTypeStr.size() - 1) : subTypeStr;
                setFieldValueArray(fieldValue, expectedType, subType, response, f, typeStr);
            }
            else {
                string typeStr = expectedType == FIELD_TYPE_PTR ? f->typeStr.substr(0,f->typeStr.size() - 1) : f->typeStr;
                setFieldValue(fieldValue, expectedType, response, f, typeStr);
            }
        }else{
            wtLogError("Incompatible types: %d and %d for field %s", fieldType, expectedType, fieldName.data());
        }
    }
}

void JsonDeserializer::setFieldValue(const string& fieldValue, const FieldType fieldType, Reflect* obj, const Field* f, const string& typeStr){
    switch(fieldType){
        case FIELD_TYPE_INT:
            f->setInt(obj, stoi(fieldValue));
            break;
        case FIELD_TYPE_SHORT:
            f->setShort(obj, (short)stoi(fieldValue));
            break;
        case FIELD_TYPE_LONG:
            f->setLong(obj, stol(fieldValue));
            break;
        case FIELD_TYPE_CHAR:
            f->setChar(obj, fieldValue[0]);
            break;
        case FIELD_TYPE_FLOAT:
            f->setFloat(obj, stof(fieldValue));
            break;
        case FIELD_TYPE_DOUBLE:
            f->setDouble(obj, stod(fieldValue));
            break;
        case FIELD_TYPE_STRING:
            f->setString(obj, fieldValue);
            break;
        case FIELD_TYPE_OBJ:
            tempObj = deserialize(fieldValue, (Reflect*) Reflect::getClassInstanceByName(typeStr));
            f->setValue(obj, tempObj, tempObj->getClassSize());
            delete tempObj;
            break;
        case FIELD_TYPE_PTR:
            tempObj = deserialize(fieldValue, (Reflect*) Reflect::getClassInstanceByName(typeStr));
            f->setPtr(obj, tempObj);
            tempObj = nullptr;
            break;
        default:
            wtLogError("Unknown FieldType Type %d", fieldType);
            f->setInt(obj, 0);
    }
}

void JsonDeserializer::setFieldValueArray(const string& fieldValue, const FieldType fieldType, const FieldType subType, Reflect* obj, const Field* f, const string& typeStr){
    switch(subType){
        case FIELD_TYPE_INT:
            if (fieldType == FIELD_TYPE_VECTOR){
                auto* data = static_cast<vector<int> *>(f->getAddress(obj));
                insertVectorData<int>(fieldValue, [](string &val){ return stoi(val);}, data);
            }else {
                int *array = nullptr;
                unsigned int size;
                parseArrayData<int>(fieldValue, [](string &val){ return stoi(val);}, array, &size);
                f->setValue(obj, array, size * sizeof(int)); //TODO set ptr
            }
            break;
        case FIELD_TYPE_SHORT:
            if (fieldType == FIELD_TYPE_VECTOR){
                auto* data = static_cast<vector<short> *>(f->getAddress(obj));
                insertVectorData<short>(fieldValue, [](string &val){ return (short)stoi(val);}, data);
            }
            break;
        case FIELD_TYPE_LONG:
            if (fieldType == FIELD_TYPE_VECTOR){
                auto* data = static_cast<vector<long> *>(f->getAddress(obj));
                insertVectorData<long>(fieldValue, [](string &val){ return stol(val);}, data);
            }
            break;
        case FIELD_TYPE_CHAR:
            if (fieldType == FIELD_TYPE_VECTOR){
                auto* data = static_cast<vector<char> *>(f->getAddress(obj));
                insertVectorData<char>(fieldValue, [](string &val){ return val[0]; }, data);
            }
            f->setChar(obj, fieldValue[0]);
            break;
        case FIELD_TYPE_FLOAT:
            if (fieldType == FIELD_TYPE_VECTOR){
                auto* data = static_cast<vector<float> *>(f->getAddress(obj));
                insertVectorData<float>(fieldValue, [](string &val){ return stof(val);}, data);
            }
            break;
        case FIELD_TYPE_DOUBLE:
            if (fieldType == FIELD_TYPE_VECTOR){
                auto* data = static_cast<vector<double> *>(f->getAddress(obj));
                insertVectorData<double>(fieldValue, [](string &val){ return stod(val);}, data);
            }
            break;
        case FIELD_TYPE_STRING: //TODO optimize
            if (fieldType == FIELD_TYPE_VECTOR){
                auto* data = static_cast<vector<string> *>(f->getAddress(obj));
                insertVectorData<string>(fieldValue, [](string &val){ return val.substr(1, val.size() - 2); }, data);
            }
            break;
        case FIELD_TYPE_OBJ:
            if (fieldType == FIELD_TYPE_VECTOR){
                auto* data = static_cast<vector<Reflect*> *>(f->getAddress(obj));
                insertVectorData(fieldValue, data, typeStr);
            }
            break;
        case FIELD_TYPE_PTR:
            if (fieldType == FIELD_TYPE_VECTOR){
                auto* data = static_cast<vector<Reflect*> *>(f->getAddress(obj));
                insertVectorData(fieldValue, data, typeStr);
            }
            break;
        default:
            wtLogError("Unknown FieldType Type %d in vec", subType);
            f->setInt(obj, 0);
            break;
    }
}

void JsonDeserializer::insertVectorData(const string& source,  vector<Reflect*> *dest, const string &typeStr){
    vector<string>* vec = StringUtils::splitObjectArray(source);
    for(string& s : *vec){
        Reflect* r = deserialize(s, (Reflect*) Reflect::getClassInstanceByName(typeStr));
        dest->push_back(r);
    }
}
