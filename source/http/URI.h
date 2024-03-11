//
// Created by Alienson on 25.1.2024..
//

#ifndef WINTER_URI_H
#define WINTER_URI_H

#include <string>

using namespace std;

class URI {
public:
    URI() = default;
    explicit URI(const char* s) : value(s){}
    explicit URI(string_view s) : value(s.substr(0, s.size())){}
    string &getPath();
    string &getFullPath();

    bool operator==(const URI& other){
        return value == other.value;
    }
private:
    string value;
};


#endif //WINTER_URI_H
