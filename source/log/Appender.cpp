//
// Created by Alienson on 27.1.2024..
//

#include "Appender.h"

#include <iostream>
#include <cstdarg>

using namespace std;

Appender::Appender(string _format, ostream* _stream) : format(std::move(_format)), stream(_stream) {

}

void Appender::write(const string &s) {
    write(s.data());
}

void Appender::write(const char *s) {
    *(this->stream) << s;
}

/*void Appender::writeOld(const char *s, va_list args) {
    vprintf(s, args);
}*/

void Appender::write(const char *s, va_list args) {
    for (int i=0; s[i]!='\0'; i++){
        if (s[i] == '%'){

            switch (s[i+1]) {
                case 'd':
                case 'i':
                    cout << va_arg(args, int);
                    break;
                case 'f':
                case 'F':
                    cout << va_arg(args, double);
                    break;
                case 'c':
                    cout << (char) va_arg(args, int); //char
                    break;
                case 's':
                    cout << (const char*) va_arg(args, const char*); //string
                    break;
                case 'u':
                case 'o':
                case 'x':
                case 'X':
                    cout << va_arg(args, unsigned int);
                    break;
                case 'e':
                case 'E':
                    cout << va_arg(args, double);
                    break;
                case 'l':
                    switch (s[i+2]) {
                        case 'd':
                            cout << va_arg(args, long);
                            break;
                        case 'l':
                            cout << va_arg(args, long long);
                            break;
                        default:
                            cout << s[i+1] << s[i+2];
                    }
                    if (s[i+2] != '\0') i++;
                    break;
                case 'h':
                    switch (s[i+2]) {
                        case 'd':
                            cout << va_arg(args, int); //short
                            break;
                        case 'u':
                            cout << va_arg(args, unsigned int); //unsigned short
                            break;
                        default:
                            cout << s[i+1] << s[i+2];
                    }
                    if (s[i+2] != '\0') i++;
                    break;
                case '%':
                    cout<<'%';
                    break;
                case '.':
                    //precision TODO use stream::setPrecision
                    break;
                default:
                    cout<<s[i+1];
            }
            if (s[i+1] != '\0') i++;
            continue;
        }

        cout << s[i];
    }
}
