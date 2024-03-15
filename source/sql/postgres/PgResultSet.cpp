//
// Created by Alienson on 14.3.2024..
//

#include "PgResultSet.h"

bool PgResultSet::next() {

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
    byte result;
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

Reflect* PgResultSet::getResult(int columnIndex){
    return nullptr;
}

vector<Reflect*> PgResultSet::getResultList(int columnIndex){
    return {};
}
