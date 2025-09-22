//
// Created by alienson on 24.3.24.
//

#include "sql/Statement.h"

Statement *Statement::setInt(int x, const char *name) {
    std::string val = std::to_string(x);
    paramsMap[name] = val; //impl conversion to string
    totalParamsLength += val.length();
    return this;
}

Statement *Statement::setLong(long x, const char *name) {
    std::string val = std::to_string(x);
    paramsMap[name] = val; //impl conversion to string
    totalParamsLength += val.length();
    return this;
}

Statement *Statement::setFloat(float x, const char *name) {
    std::string val = std::to_string(x);
    paramsMap[name] = val; //impl conversion to string
    totalParamsLength += val.length();
    return this;
}

Statement *Statement::setDouble(double x, const char *name) {
    std::string val = std::to_string(x);
    paramsMap[name] = val; //impl conversion to string
    totalParamsLength += val.length();
    return this;
}

Statement *Statement::setString(std::string &s, const char *name) {
    std::string val = {};
    val.reserve(s.length() + 2);
    val+="'";
    val+= s;
    val+= "'";
    paramsMap[name] = val; //impl conversion to string
    totalParamsLength += val.length();
    return this;
}

Statement *Statement::setString(const char *s, const char *name) {
    std::string val = "'" + std::string(s) + "'";
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
    std::string val = StringUtils::toUpperCase(StringUtils::parseBoolean(b));
    paramsMap[name] = val; //impl conversion to string
    totalParamsLength += val.length();
    return this;
}

Statement *Statement::setByte(std::byte c, const char *name) {
    wtLogError("Unimplemented method!");
    return nullptr;
}

Statement *Statement::setShort(short s, const char *name) {
    std::string val = std::to_string((int)s);
    paramsMap[name] = val; //impl conversion to string
    totalParamsLength += val.length();
    return this;
}

Statement *Statement::setDate(std::string &s, const char *name) {
    wtLogError("Unimplemented method!");
    return nullptr;
}

Statement *Statement::setDate(const char *s, const char *name) {
    wtLogError("Unimplemented method!");
    return nullptr;
}

Statement *Statement::setTime(std::string &s, const char *name) {
    wtLogError("Unimplemented method!");
    return nullptr;
}

Statement *Statement::setTime(const char *s, const char *name) {
    wtLogError("Unimplemented method!");
    return nullptr;
}

Statement *Statement::setDateTime(std::string &s, const char *name) {
    wtLogError("Unimplemented method!");
    return nullptr;
}

Statement *Statement::setDateTime(const char *s, const char *name) {
    wtLogError("Unimplemented method!");
    return nullptr;
}

Statement *Statement::setBlob(std::string &s, const char *name) {
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
            if (endIndex == std::string::npos)
                endIndex = query.size();
            std::string paramName = query.substr(beginIndex, endIndex - beginIndex);
            paramsMap.try_emplace(paramName, "");
            i = endIndex;
        }
    }
}

std::string Statement::buildQuery() {
    std::string response;
    response.reserve(totalParamsLength + query.size()); //A little over since param names are included

    size_t beginIndex, endIndex;
    auto it = paramsMap.begin();
    for (size_t i=0; i<query.size(); i++){
        if (query[i] == ':'){
            beginIndex = i+1;
            endIndex = query.find(' ', i+1);
            if (endIndex == std::string::npos)
                endIndex = query.size();
            std::string paramName = query.substr(beginIndex, endIndex - beginIndex);
            it = paramsMap.find(paramName);
            if (it == paramsMap.end()){
                wtLogError("Parameter {} is unbound!", paramName.data());
                return "";
            }
            i = endIndex;
            response+=it->second;
        }
        response+=query[i];
    }

    return response;
}

std::shared_ptr<ResultSet> Statement::execute() {
    return executeQuery(buildQuery().data());
}
