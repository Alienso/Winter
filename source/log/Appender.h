//
// Created by Alienson on 27.1.2024..
//

#ifndef WINTER_APPENDER_H
#define WINTER_APPENDER_H

#include <string>
#include <iostream>

using namespace std;

class Appender {
public:

    Appender(string _format, ostream* _stream);
    //TODO close ofstreams?

    void write(const string &s) const;
    void write(const char* s) const;
    void write(const char *s, va_list args) const;
    void writeFormatString(const char* logLevelStr, const char* filePath, int line) const;

    [[nodiscard]] ostream* getStream() const;

private:
    const string format;
    ostream* stream;
};


#endif //WINTER_APPENDER_H
