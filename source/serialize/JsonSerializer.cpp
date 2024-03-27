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

        string fieldValue = convertToJsonString(field, obj);
        s->append(fieldValue);
        if (i != n - 1)
            s->push_back(',');
        s->push_back('\n');
    }

    s->push_back('}');
    return s;
}

string JsonSerializer::convertToJsonString(const Field &field, Reflect* obj){
    switch (field.type) {
        case FIELD_TYPE_INT:
            return to_string(field.getInt(obj));
        case FIELD_TYPE_SHORT:
            return to_string(field.getShort(obj));
        case FIELD_TYPE_LONG:
            return to_string(field.getLong(obj));
        case FIELD_TYPE_FLOAT:
            return to_string(field.getFloat(obj));
        case FIELD_TYPE_DOUBLE:
            return to_string(field.getDouble(obj));
        case FIELD_TYPE_CHAR:
            return "\"" + to_string(field.getChar(obj)) + "\"";
        case FIELD_TYPE_BOOL:
            return to_string(field.getBool(obj));
        case FIELD_TYPE_STRING:
            return "\"" + field.getString(obj) + "\"";
        case FIELD_TYPE_OBJ:
            return *serialize((Reflect*)field.getAddress(obj)); //TODO this is a copy
        case FIELD_TYPE_PTR:
            ptr = *((Reflect**)field.getPtr(obj));
            if (ptr == nullptr) return "null";
            return *serialize(ptr); //TODO this is a copy
        //case FIELD_TYPE_ARRAY:
        case FIELD_TYPE_VECTOR:
            return convertVectorToJsonString(field, obj);
        default:
            wtLogError("Unknown fieldType: %d", field.type);
            return "\"\"";
    }
}

string to_string(short x){
    return to_string((int)x);
}
string to_string(bool x){
    return std::to_string(x);
}

string JsonSerializer::convertVectorToJsonString(const Field &f, Reflect *obj) {
    FieldType subType = getArraySubFieldType(f.typeStr);
    vector<Reflect*>* vecObj = nullptr;
    switch (subType) {
        case FIELD_TYPE_INT:
            vecInt = static_cast<vector<int> *>(f.getAddress(obj));
            return vectorToString(*vecInt, to_string);
        case FIELD_TYPE_SHORT:
            vecShort = static_cast<vector<short> *>(f.getAddress(obj));
            return vectorToString(*vecShort, to_string);
        case FIELD_TYPE_LONG:
            vecLong = static_cast<vector<long> *>(f.getAddress(obj));
            return vectorToString(*vecLong, to_string);
        case FIELD_TYPE_CHAR:
            vecChar = static_cast<vector<char> *>(f.getAddress(obj));
            return vectorToString(*vecChar);
        case FIELD_TYPE_FLOAT:
            vecFloat = static_cast<vector<float> *>(f.getAddress(obj));
            return vectorToString(*vecFloat, to_string);
        case FIELD_TYPE_DOUBLE:
            vecDouble = static_cast<vector<double> *>(f.getAddress(obj));
            return vectorToString(*vecDouble, to_string);
        case FIELD_TYPE_BOOL:
            vecBool = static_cast<vector<bool> *>(f.getAddress(obj));
            return vectorToString(*vecBool, to_string);
        case FIELD_TYPE_STRING:
            vecString = static_cast<vector<string> *>(f.getAddress(obj));
            return vectorToString(*vecString);
        case FIELD_TYPE_PTR:
            vecObj = static_cast<vector<Reflect*> *>(f.getAddress(obj));
            return vectorToString(*vecObj);
        case FIELD_TYPE_OBJ:
            //TODO
        default:
            wtLogError("Unknown FieldType Type %d in vec", subType);
            return "\"\"";
    }

}

string JsonSerializer::vectorToString(const vector<Reflect*>& source) {

    if (source.empty())
        return "[]";

    string res = "[";
    for(Reflect* x : source){
        res+=*serialize(x);
        res+=',';
    }
    res[res.size()-1] = ']';
    return res;
}

string JsonSerializer::vectorToString(const vector<char>& source) {

    if (source.empty())
        return "[]";

    string res = "[";
    for(char c : source){
        res += "\"";
        res.push_back(c);
        res += "\",";
    }
    res[res.size()-1] = ']';
    return res;
}

string JsonSerializer::vectorToString(const vector<string>& source) {

    if (source.empty())
        return "[]";

    string res = "[";
    for(const string& s : source){
        res += "\"" + s + "\",";
    }
    res[res.size()-1] = ']';
    return res;
}
