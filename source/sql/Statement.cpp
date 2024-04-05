//
// Created by alienson on 24.3.24..
//

#include "Statement.h"

Statement *Statement::setInt(int x, const char *name) {
    string val = to_string(x);
    paramsMap[name] = val; //impl conversion to string
    totalParamsLength += val.length();
    return this;
}

Statement *Statement::setLong(long x, const char *name) {
    string val = to_string(x);
    paramsMap[name] = val; //impl conversion to string
    totalParamsLength += val.length();
    return this;
}

Statement *Statement::setFloat(float x, const char *name) {
    string val = to_string(x);
    paramsMap[name] = val; //impl conversion to string
    totalParamsLength += val.length();
    return this;
}

Statement *Statement::setDouble(double x, const char *name) {
    string val = to_string(x);
    paramsMap[name] = val; //impl conversion to string
    totalParamsLength += val.length();
    return this;
}

Statement *Statement::setString(string &s, const char *name) {
    string val = {};
    val.reserve(s.length() + 2);
    val+="'";
    val+= s;
    val+= "'";
    paramsMap[name] = val; //impl conversion to string
    totalParamsLength += val.length();
    return this;
}

Statement *Statement::setString(const char *s, const char *name) {
    string val = "'" + string(s) + "'";
    paramsMap[name] = val; //impl conversion to string
    totalParamsLength += val.length();
    return this;
}

Statement *Statement::setObject(int o, const char *name) {
    wtLogError("Unimplemented method!");
    return nullptr;
}

Statement *Statement::setNull(const char *name) {
    paramsMap[name] = "NULL"; //impl conversion to string
    totalParamsLength += 4;
    return this;
}

Statement *Statement::setBool(bool b, const char *name) {
    string val = StringUtils::toUpperCase(StringUtils::parseBoolean(b));
    paramsMap[name] = val; //impl conversion to string
    totalParamsLength += val.length();
    return this;
}

Statement *Statement::setByte(std::byte c, const char *name) {
    wtLogError("Unimplemented method!");
    return nullptr;
}

Statement *Statement::setShort(short s, const char *name) {
    string val = to_string((int)s);
    paramsMap[name] = val; //impl conversion to string
    totalParamsLength += val.length();
    return this;
}

Statement *Statement::setDate(string &s, const char *name) {
    wtLogError("Unimplemented method!");
    return nullptr;
}

Statement *Statement::setDate(const char *s, const char *name) {
    wtLogError("Unimplemented method!");
    return nullptr;
}

Statement *Statement::setTime(string &s, const char *name) {
    wtLogError("Unimplemented method!");
    return nullptr;
}

Statement *Statement::setTime(const char *s, const char *name) {
    wtLogError("Unimplemented method!");
    return nullptr;
}

Statement *Statement::setDateTime(string &s, const char *name) {
    wtLogError("Unimplemented method!");
    return nullptr;
}

Statement *Statement::setDateTime(const char *s, const char *name) {
    wtLogError("Unimplemented method!");
    return nullptr;
}

Statement *Statement::setBlob(string &s, const char *name) {
    wtLogError("Unimplemented method!");
    return nullptr;
}

Statement *Statement::setBlob(const char *s, const char *name) {
    wtLogError("Unimplemented method!");
    return nullptr;
}

void Statement::generateParameterMap() {
    size_t beginIndex, endIndex;
    for(size_t i=0; i<query.size(); i++){
        if (query[i] == ':'){
            beginIndex = i+1;
            endIndex = query.find(' ', i+1);
            if (endIndex == string::npos)
                endIndex = query.size();
            string paramName = query.substr(beginIndex, endIndex - beginIndex);
            paramsMap.try_emplace(paramName, "");
            i = endIndex;
        }
    }
}

string Statement::buildQuery() {
    string response;
    response.reserve(totalParamsLength + query.size()); //A little over since param names are included

    size_t beginIndex, endIndex;
    auto it = paramsMap.begin();
    for (size_t i=0; i<query.size(); i++){
        if (query[i] == ':'){
            beginIndex = i+1;
            endIndex = query.find(' ', i+1);
            if (endIndex == string::npos)
                endIndex = query.size();
            string paramName = query.substr(beginIndex, endIndex - beginIndex);
            it = paramsMap.find(paramName);
            if (it == paramsMap.end()){
                wtLogError("Parameter %s is unbound!", paramName.data());
                return "";
            }
            i = endIndex;
            response+=it->second;
        }
        response+=query[i];
    }

    return response;
}

shared_ptr<ResultSet> Statement::execute() {
    return executeQuery(buildQuery().data());
}
