//
// Created by Alienson on 14.3.2024..
//

#include "PgResultSet.h"

bool PgResultSet::next() {
    return false;
}

Reflect* PgResultSet::getResult(){
    return new Reflect();
}

vector<Reflect*>* PgResultSet::getResultList(){

    int nFields, i, j;

    /* first, print out the attribute names */
    nFields = PQnfields(pgResult);
    for (i = 0; i < nFields; i++)
        printf("%-15s", PQfname(pgResult, i));
    printf("\n\n");

    /* next, print out the rows */
    for (i = 0; i < PQntuples(pgResult); i++){
        for (j = 0; j < nFields; j++)
            printf("%-15s", PQgetvalue(pgResult, i, j));
        printf("\n");
    }

    PQclear(pgResult);

    /* end the transaction */
    pgResult = PQexec(pgConn, "END");
    PQclear(pgResult);

    return new vector<Reflect*>();
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
