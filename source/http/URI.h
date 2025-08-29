//
// Created by Alienson on 25.1.2024.
//

#ifndef WINTER_URI_H
#define WINTER_URI_H

#include <string>

class URI {
public:
    URI() = default;
    explicit URI(const char* s) : value(s){}
    explicit URI(std::string_view s) : value(s.substr(0, s.size())){}
    [[nodiscard]] const std::string &getPath() const;
    [[nodiscard]] const std::string &getFullPath() const;

    bool operator==(const URI& other){
        return value == other.value;
    }
private:
    std::string value;
};


#endif //WINTER_URI_H
