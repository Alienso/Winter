//
// Created by Alienson on 14.2.2024..
//

#include "JsonSerializer.h"

string* JsonSerializer::serialize(Reflect* obj){
    auto* s = new string("{\n");
    size_t n = obj->getDeclaredFields().size();
    for (int i=0; i<n; i++){
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

string JsonSerializer::convertToJsonString(Field &field, Reflect* obj){
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
        case FIELD_TYPE_STRING:
            return "\"" + field.getString(obj) + "\"";
        case FIELD_TYPE_OBJ:
            return *serialize((Reflect*)field.getAddress(obj)); //TODO this is a copy
        case FIELD_TYPE_PTR:
            ptr = *((Reflect**)field.getPtr(obj));
            if (ptr == nullptr) return "\"\"";
            return *serialize(ptr); //TODO this is a copy
        case FIELD_TYPE_ARRAY:
        case FIELD_TYPE_VECTOR:
            //return "[" + convertToJsonString(field, obj) + "]"; TODO
            return "[]";
        default:
            return "\"\"";

    }
}
