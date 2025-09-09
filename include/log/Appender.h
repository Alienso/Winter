//
// Created by Alienson on 27.1.2024.
//

#ifndef WINTER_APPENDER_H
#define WINTER_APPENDER_H

#include <string>
#include <iostream>

class Appender {
public:

    Appender(std::string _format, std::ostream* _stream);
    //TODO close ofstreams?

    void write(const std::string &s) const;
    void write(const char* s) const;
    void write(const char *s, va_list args) const;
    void writeFormatString(const char* logLevelStr, const char* filePath, int line) const;

    [[nodiscard]] std::ostream* getStream() const;

private:
    const std::string format;
    std::ostream* stream;
};


#endif //WINTER_APPENDER_H
