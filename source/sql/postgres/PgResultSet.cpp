//
// Created by Alienson on 14.3.2024.
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
std::shared_ptr<Reflect> PgResultSet::getResult(Reflect* (*allocator)()) const{

    if (resultSize == 0){
        return nullptr;
    }

    std::unordered_map<std::string, int> tupleMapping{};

    for (size_t i = 0; i < fieldCount; i++)
        tupleMapping[StringUtils::toCamelCase(std::string{PQfname(pgResult, i)})] = i;

    auto it = tupleMapping.begin();
    std::shared_ptr<Reflect> entity = std::make_shared<Reflect>(*allocator());
    for (auto& f : entity->getDeclaredFields()) {
        it = tupleMapping.find(StringUtils::toCamelCase(f.name));
        if (it != tupleMapping.end()) {
            char *s = PQgetvalue(pgResult, 0, it->second);
            f.set(entity.get(), s);
        }
    }

    PQclear(pgResult);
    return entity;
}

std::shared_ptr<std::vector<std::shared_ptr<Reflect>>> PgResultSet::getResultList(Reflect* (*allocator)()) const{

    auto response = std::make_shared<std::vector<std::shared_ptr<Reflect>>>();
    if (resultSize == 0){
        return response;
    }

    response->resize(resultSize);

    std::unordered_map<std::string, int> tupleMapping{};

    for (size_t i = 0; i < fieldCount; i++)
        tupleMapping[StringUtils::toCamelCase(std::string{PQfname(pgResult, i)})] = i;

    auto it = tupleMapping.begin();

    int j = 0;
    for (size_t i = 0; i < resultSize; i++){
        std::shared_ptr<Reflect> entity = std::make_shared<Reflect>(*allocator());
        for (auto& f : entity->getDeclaredFields()) {
            it = tupleMapping.find(StringUtils::toCamelCase(f.name)); //TODO add field match type to method signature
            if (it != tupleMapping.end()) {
                char *s = PQgetvalue(pgResult, i, it->second);
                f.set(entity.get(), s);
            }
        }
        (*response)[j++] = entity;
    }

    PQclear(pgResult);
    return response;
}

int PgResultSet::getInt(size_t columnIndex) const {
    return std::stoi(PQgetvalue(pgResult, cursorIndex, columnIndex));
}

long PgResultSet::getLong(size_t columnIndex) const {
    return std::stol(PQgetvalue(pgResult, cursorIndex, columnIndex));
}

float PgResultSet::getFloat(size_t columnIndex) const {
    return std::stof(PQgetvalue(pgResult, cursorIndex, columnIndex));
}

double PgResultSet::getDouble(size_t columnIndex) const {
    return std::stod(PQgetvalue(pgResult, cursorIndex, columnIndex));
}

std::string PgResultSet::getString(size_t columnIndex) const {
    return PQgetvalue(pgResult, cursorIndex, columnIndex);
}

bool PgResultSet::getBool(size_t columnIndex) const {
    return StringUtils::parseBoolean(PQgetvalue(pgResult, cursorIndex, columnIndex));
}

std::byte PgResultSet::getByte(size_t columnIndex) const {
    return (std::byte)PQgetvalue(pgResult, cursorIndex, columnIndex)[0];
}

short PgResultSet::getShort(size_t columnIndex) const {
    return (short)std::stoi(PQgetvalue(pgResult, cursorIndex, columnIndex));
}

std::string PgResultSet::getDate(size_t columnIndex) const {
    return PQgetvalue(pgResult, cursorIndex, columnIndex);
}

std::string PgResultSet::getTime(size_t columnIndex) const {
    return PQgetvalue(pgResult, cursorIndex, columnIndex);
}

std::string PgResultSet::getDateTime(size_t columnIndex) const {
    return PQgetvalue(pgResult, cursorIndex, columnIndex);
}

std::string PgResultSet::getBlob(size_t columnIndex) const {
    return PQgetvalue(pgResult, cursorIndex, columnIndex);
}
