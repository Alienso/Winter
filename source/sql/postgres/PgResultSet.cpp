//
// Created by Alienson on 14.3.2024..
//

#include "PgResultSet.h"

PgResultSet::PgResultSet(PGresult *pgResult, PGconn *pgConn) : pgResult(pgResult), pgConn(pgConn) {
    fieldCount = PQnfields(pgResult);
    resultSize = PQntuples(pgResult);
    cursorIndex = 0;
}

PgResultSet::~PgResultSet() {
}

bool PgResultSet::next() {
    if (cursorIndex == resultSize - 1)
        return false;
    cursorIndex++;
    return true;
}

//TODO matching? Entity is available now. but Reflect* are also viable
Reflect* PgResultSet::getResult(Reflect* (*allocator)()) const{

    if (resultSize == 0){
        return nullptr;
    }

    unordered_map<string, int> tupleMapping{};

    for (size_t i = 0; i < fieldCount; i++)
        tupleMapping[StringUtils::toCamelCase(string{PQfname(pgResult, i)})] = i;

    auto it = tupleMapping.begin();
    Reflect* entity = allocator();
    for (auto& f : entity->getDeclaredFields()) {
        it = tupleMapping.find(StringUtils::toCamelCase(f.name));
        if (it != tupleMapping.end()) {
            char *s = PQgetvalue(pgResult, 0, it->second);
            f.set(entity, s);
        }
    }

    PQclear(pgResult);
    return entity;
}

vector<Reflect*>* PgResultSet::getResultList(Reflect* (*allocator)()) const{

    auto* response = new vector<Reflect*>();
    if (resultSize == 0){
        return response;
    }

    response->resize(resultSize);

    unordered_map<string, int> tupleMapping{};

    for (size_t i = 0; i < fieldCount; i++)
        tupleMapping[StringUtils::toCamelCase(string{PQfname(pgResult, i)})] = i;

    auto it = tupleMapping.begin();

    int j = 0;
    for (size_t i = 0; i < resultSize; i++){
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

int PgResultSet::getInt(size_t columnIndex) const {
    return stoi(PQgetvalue(pgResult, cursorIndex, columnIndex));
}

long PgResultSet::getLong(size_t columnIndex) const {
    return stol(PQgetvalue(pgResult, cursorIndex, columnIndex));
}

float PgResultSet::getFloat(size_t columnIndex) const {
    return stof(PQgetvalue(pgResult, cursorIndex, columnIndex));
}

double PgResultSet::getDouble(size_t columnIndex) const {
    return stod(PQgetvalue(pgResult, cursorIndex, columnIndex));
}

string PgResultSet::getString(size_t columnIndex) const {
    return PQgetvalue(pgResult, cursorIndex, columnIndex);
}

bool PgResultSet::getBool(size_t columnIndex) const {
    return StringUtils::parseBoolean(PQgetvalue(pgResult, cursorIndex, columnIndex));
}

std::byte PgResultSet::getByte(size_t columnIndex) const {
    return (std::byte)PQgetvalue(pgResult, cursorIndex, columnIndex)[0];
}

short PgResultSet::getShort(size_t columnIndex) const {
    return (short)stoi(PQgetvalue(pgResult, cursorIndex, columnIndex));
}

string PgResultSet::getDate(size_t columnIndex) const {
    return PQgetvalue(pgResult, cursorIndex, columnIndex);
}

string PgResultSet::getTime(size_t columnIndex) const {
    return PQgetvalue(pgResult, cursorIndex, columnIndex);
}

string PgResultSet::getDateTime(size_t columnIndex) const {
    return PQgetvalue(pgResult, cursorIndex, columnIndex);
}

string PgResultSet::getBlob(size_t columnIndex) const {
    return PQgetvalue(pgResult, cursorIndex, columnIndex);
}
