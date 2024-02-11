//
// Created by Alienson on 27.1.2024..
//

#include "Appender.h"

#include <iostream>

using namespace std;

Appender::Appender(string _format, ostream* _stream) : format(std::move(_format)), stream(_stream) {

}

void Appender::write(const string &s) {
    write(s.data());
}

void Appender::write(const char *s) {
    *(this->stream) << s;
}

void Appender::write(const char *s, va_list args) {
    vprintf(s, args);
}
