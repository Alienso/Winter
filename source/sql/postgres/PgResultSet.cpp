//
// Created by Alienson on 14.3.2024..
//

#include "PgResultSet.h"

PgResultSet::~PgResultSet() {
    pgResult = PQexec(pgConn, "END");
    PQclear(pgResult);
}

bool PgResultSet::next() {
    return false;
}

Reflect* PgResultSet::getResult(){
    return new Reflect();
}

vector<Reflect*>* PgResultSet::getResultList(Reflect* (*allocator)()){

    int fieldsSize = PQnfields(pgResult);
    int resultSize = PQntuples(pgResult);
    int j = 0;

    auto* response = new vector<Reflect*>();

    if (resultSize == 0){
        return response;
    }

    response->resize(resultSize);

    unordered_map<string, int> tupleMapping{};

    for (int i = 0; i < fieldsSize; i++)
        tupleMapping[StringUtils::toCamelCase(string{PQfname(pgResult, i)})] = i;

    auto it = tupleMapping.begin();

    for (int i = 0; i < resultSize; i++){
        Reflect* entity = allocator();
        for (auto& f : entity->getDeclaredFields()) {
            it = tupleMapping.find(StringUtils::toCamelCase(f.name)); //TODO add field match type to method signature
            if (it != tupleMapping.end()) {
                char *s = PQgetvalue(pgResult, i, it->second);
                f.set(entity, s);
            }
        }
        (*response)[j++] = entity;
    }

    PQclear(pgResult);
    return response;
}

int PgResultSet::getInt(int columnIndex) {
    return 0;
}

long PgResultSet::getLong(int columnIndex) {
    return 0;
}

float PgResultSet::getFloat(int columnIndex) {
    return 0;
}

double PgResultSet::getDouble(int columnIndex) {
    return 0;
}

string PgResultSet::getString(int columnIndex) {
    return std::string();
}

bool PgResultSet::getBool(int columnIndex) {
    return false;
}

byte PgResultSet::getByte(int columnIndex) {
    byte result = static_cast<byte>(0);
    return result;
}

short PgResultSet::getShort(int columnIndex) {
    return 0;
}

string PgResultSet::getDate(int columnIndex) {
    return std::string();
}

string PgResultSet::getTime(int columnIndex) {
    return std::string();
}

string PgResultSet::getDateTime(int columnIndex) {
    return std::string();
}

string PgResultSet::getBlob(int columnIndex) {
    return std::string();
}
