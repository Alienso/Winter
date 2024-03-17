//
// Created by Alienson on 27.1.2024..
//

#include "Appender.h"

#include <iostream>
#include <cstdarg>

using namespace std;

Appender::Appender(string _format, ostream* _stream) : format(std::move(_format)), stream(_stream) {

}

void Appender::writeFormatString(const char* logLevelStr, const char* filePath, const int line) const{
    *stream << logLevelStr << filePath << ':' << line << '\t';
}

void Appender::write(const string &s) const {
    write(s.data());
}

void Appender::write(const char *s) const {
    *(this->stream) << s;
}

/*void Appender::writeOld(const char *s, va_list args) {
    vprintf(s, args);
}*/

void Appender::write(const char *s, va_list args) const {
    for (int i=0; s[i]!='\0'; i++){
        if (s[i] == '%'){

            switch (s[i+1]) {
                case 'd':
                case 'i':
                    *stream << va_arg(args, int);
                    break;
                case 'f':
                case 'F':
                    *stream << va_arg(args, double);
                    break;
                case 'c':
                    *stream << (char) va_arg(args, int); //char
                    break;
                case 's':
                    *stream << (const char*) va_arg(args, const char*); //string
                    break;
                case 'u':
                case 'o':
                case 'x':
                case 'X':
                    *stream << va_arg(args, unsigned int);
                    break;
                case 'e':
                case 'E':
                    *stream << va_arg(args, double);
                    break;
                case 'l':
                    switch (s[i+2]) {
                        case 'd':
                            *stream << va_arg(args, long);
                            break;
                        case 'l':
                            *stream << va_arg(args, long long);
                            break;
                        default:
                            *stream << s[i+1] << s[i+2];
                    }
                    if (s[i+2] != '\0') i++;
                    break;
                case 'h':
                    switch (s[i+2]) {
                        case 'd':
                            *stream << va_arg(args, int); //short
                            break;
                        case 'u':
                            *stream << va_arg(args, unsigned int); //unsigned short
                            break;
                        default:
                            *stream << s[i+1] << s[i+2];
                    }
                    if (s[i+2] != '\0') i++;
                    break;
                case '%':
                    *stream<<'%';
                    break;
                case '.':
                    //precision TODO use stream::setPrecision
                    break;
                default:
                    *stream<<s[i+1];
            }
            if (s[i+1] != '\0') i++;
            continue;
        }

        *stream << s[i];
    }
}

ostream* Appender::getStream() const{
    return stream;
}
