//
// Created by alienson on 4.4.24.
//

#ifndef WINTER_UTIL_H
#define WINTER_UTIL_H

#endif //WINTER_UTIL_H

#include <string>
#include <regex>

class Util{
public:
    [[nodiscard]] static std::string getFieldName(std::string& line){
        std::smatch match;
        if (regex_search(line, match, fieldReg)) {
            return match.str(4);
        }

        return "";
    }

private:
    //TODO will catch syntactically inaccurate declarations (int & const * const & x;)
    static inline std::regex fieldReg{R"(([a-zA-Z]+::)?([a-zA-Z_][a-zA-Z0-9_]*)\s*(<.+?>)?\s*[\*&\s]+?\s*([a-zA-Z_][a-zA-Z0-9_]*)\s*(=\s*.+)?\s*;)"};
};

