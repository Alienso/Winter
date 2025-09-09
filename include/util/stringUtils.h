//
// Created by Alienson on 31.1.2024.
//

#ifndef WINTER_STRINGUTILS_H
#define WINTER_STRINGUTILS_H

#include <vector>
#include <string>
#include <string_view>
#include <cstring>
#include <iostream>

class StringUtils{
public:

    [[nodiscard]] static std::string trim(const std::string& s){
        size_t start = s.size(), end = s.size();
        for(size_t i=0; i < s.size(); i++){
            if (!isspace(s[i])){
                start = i;
                break;
            }
        }
        for(long long i=s.size() - 1; i > 0; i--){
            if (!isspace(s[i])){
                end = i;
                break;
            }
        }
        return s.substr(start, end - start + 1);
    }

    [[nodiscard]] static std::string stripBlankCharacters(const std::string& s){
        std::string res;
        res.resize(s.length());
        size_t i,j;
        for (i=0,j=0; i<s.size(); i++)
            if (!isspace(s[i]))
                res[j++] = s[i];
        res.resize(j);
        return res;
    }

    [[nodiscard]] static std::string stripSpecialCharacters(const std::string& s) {
        std::string res;
        res.resize(s.length());
        size_t i,j;
        for (i=0,j=0; i<s.size(); i++)
            if (isalnum(s[i]))
                res[j++] = s[i];
        res.resize(j);
        return res;
    }

    [[nodiscard]] static bool endsWith(const std::string& s, const char* suffix) {
        size_t suffixSize = 0;
        for (;suffix[suffixSize] != '\0'; suffixSize++);
        suffixSize--;
        size_t offset = s.length() - suffixSize - 1;

        long long i=suffixSize;
        while (i > 0){
            if (s[offset + i] != suffix[i])
                return false;
            i--;
        }
        return true;
    }

    [[nodiscard]] static bool startsWith(const std::string& s, const char* prefix) {
        return startsWith(s.data(), prefix);
    }

    [[nodiscard]] static bool startsWith(const char*s, const char* prefix) {
        for (size_t i=0; true; i++) {
            if (prefix[i] == '\0' || s[i] == '\0')
                return true;
            else if (s[i] != prefix[i])
                return false;
        }
    }

    [[nodiscard]] static std::vector<std::string>* split(const std::string& s, const char c){
        auto* res = new std::vector<std::string>();
        size_t startIndex = 0, endIndex;
        while (startIndex < s.size()){
            endIndex = s.find(c, startIndex);
            if (endIndex == std::string::npos)
                endIndex = s.size();

            res->push_back(s.substr(startIndex, endIndex - startIndex));
            startIndex = endIndex + 1;
        }

        return res;
    }

    [[nodiscard]] static std::vector<std::string>* splitArray(const std::string& s, const char c = ','){
        auto* res = new std::vector<std::string>();
        size_t startIndex = 0, endIndex;
        bool hasBrackets = false;

        if (s[0] == '[' || s[0] == '{'){
            startIndex = 1;
            hasBrackets = true;
        }

        size_t i;
        unsigned int bracketCounter = 0;
        bool inStr = false;
        while (startIndex < s.size()){
            endIndex = std::string::npos;
            for (i=startIndex; i<s.size(); i++){ //TODO !inStr should be check for brackets too
                if (s[i] == '[' || s[i] == '{')
                    bracketCounter+=s[i];
                if (s[i] == ']' || s[i] == '}')
                    bracketCounter-=s[i]+2;
                if (s[i] == '"' && s[i-1] != '\\')
                    inStr = !inStr;
                if (bracketCounter == 0 && !inStr && s[i] == c) {
                    endIndex = i;
                    break;
                }
            }

            if (endIndex == std::string::npos || endIndex >= s.size())
                endIndex = hasBrackets ? s.size() -1 : s.size();

            res->push_back(s.substr(startIndex, endIndex - startIndex));
            startIndex = endIndex + 1; //TODO startIndex is first char this is not blank space
        }

        return res;
    }

    [[nodiscard]] static std::vector<std::string>* splitObjectArray(const std::string& s, const char c = ','){
        auto* res = new std::vector<std::string>();
        size_t startIndex = 0, endIndex;
        size_t i;
        unsigned int bracketCounter = 0;
        bool inStr = false;

        while(s[startIndex] != '{' && startIndex < s.size()) startIndex++;

        while (startIndex < s.size()){
            endIndex = std::string::npos;
            for (i=startIndex; i<s.size(); i++){
                if (s[i] == '{')
                    bracketCounter++;
                if (s[i] == '}')
                    bracketCounter--;
                if (s[i] == '"' && s[i-1] != '\\')
                    inStr = !inStr;
                if (bracketCounter == 0 && !inStr && s[i] == c) {
                    endIndex = i;
                    break;
                }
            }

            if (endIndex == std::string::npos || endIndex >= s.size())
                endIndex = s.size();

            res->push_back(s.substr(startIndex, endIndex - startIndex));
            startIndex = endIndex + 1;
        }

        return res;
    }

    [[nodiscard]] static std::string uncapitalize(const std::string& s){
        std::string res(s);
        for (size_t i=0; i<s.size(); i++){
            if(isalpha(res[i]) && isupper(res[i]))
                res[i] = (char)tolower(res[i]);
        }
        return res;
    }

    [[nodiscard]] static std::string_view rtrim(const std::string_view s){
        long long size = s.size();
        for(long long i=size;i>=0; i--){
            if (isspace(s[i]))
                size--;
            else break;
        }

        return std::string_view(s.data(), size + 1);
    }

    [[nodiscard]] static std::string replace(const std::string& s, const char pattern, const char replacement) {

        if (s.empty())
            return "";

        std::string res(s);
        for (size_t i=0; i<s.size(); i++){
            if (res[i] == pattern)
                res[i] = replacement;
        }
        return res;
    }

    [[nodiscard]] static std::string replace(const std::string& s, const char *pattern, const char replacement) {

        if (s.empty())
            return "";

        std::string res;
        size_t start=0,end;
        for (size_t i = 0; i<s.size(); i++){
            end = s.find(pattern, start);
            for (size_t j=start; j<end && j<s.size(); j++)
                res.push_back(s[j]);
            if (end == std::string::npos) {
                break;
            }
            res.push_back(replacement);
            start = end + 1;
        }
        return res;
    }

    [[nodiscard]] static std::string toUpperCase(const std::string& s){
        std::string res = {s};
        for (char &c : res){
            if (islower(c))
                c = (char)toupper(c);
        }
        return res;
    }

    [[nodiscard]] static std::string toLowerCase(const std::string& s){
        std::string res = {s};
        for (char &c : res){
            if (isupper(c))
                c = (char)tolower(c);
        }
        return res;
    }

    [[nodiscard]] static bool parseBoolean(const char *value) {
        std::string s{value};
        s = toLowerCase(s);
        if (s == "true")
            return true;
        if (s == "false")
            return false;
        std::cerr << "Received invalid bool value: " << value;
        return false;
    }

    [[nodiscard]] static std::string parseBoolean(bool value) {
        return value ? "true" : "false";
    }

    [[nodiscard]] static std::string toCamelCase(const std::string& value){
        std::string s;
        s.resize(value.size());

        size_t j = 0;
        for (size_t i=0; i<value.size(); i++){
            if (value[i] == ' ' || value[i] == '_'){
                i++;
                if (i <value.size() && isalpha(value[i])){
                    s[j++] = toupper(value[i]);
                    continue;
                }else{ //We copy '_' or ' '
                    i--;
                    s[j++] = value[i];
                }
            }else s[j++] = value[i];
        }
        s.resize(j);
        return s;
    }

    [[nodiscard]] static inline std::string to_string(short x){
        return std::to_string((int)x);
    }
    [[nodiscard]] static inline std::string to_string(bool x){
        return x ? "true" : "false";
    }
    [[nodiscard]] static inline std::string to_string(std::byte x){
        return std::to_string((char)x);
    }
    [[nodiscard]] static inline std::string to_string(char x){
        return std::to_string(x);
    }

};

#endif //WINTER_STRINGUTILS_H
