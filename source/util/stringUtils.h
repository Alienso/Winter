//
// Created by Alienson on 31.1.2024..
//

#ifndef WINTER_STRINGUTILS_H
#define WINTER_STRINGUTILS_H

#include <vector>
#include <string>
#include <string_view>
#include <cstring>
#include <iostream>

using namespace std;

class StringUtils{
public:

    [[nodiscard]] static string trim(const string& s){
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

    [[nodiscard]] static string stripBlankCharacters(const string& s){
        string res;
        res.resize(s.length());
        size_t i,j;
        for (i=0,j=0; i<s.size(); i++)
            if (!isspace(s[i]))
                res[j++] = s[i];
        res.resize(j);
        return res;
    }

    [[nodiscard]] static string stripSpecialCharacters(const string& s) {
        string res;
        res.resize(s.length());
        size_t i,j;
        for (i=0,j=0; i<s.size(); i++)
            if (isalnum(s[i]))
                res[j++] = s[i];
        res.resize(j);
        return res;
    }

    [[nodiscard]] static bool endsWith(const string& s, const char* suffix) {
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

    [[nodiscard]] static bool startsWith(const string& s, const char* prefix) {
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

    [[nodiscard]] static vector<string>* split(const string& s, const char c){
        auto* res = new vector<string>();
        size_t startIndex = 0, endIndex;
        while (startIndex < s.size()){
            endIndex = s.find(c, startIndex);
            if (endIndex == string::npos)
                endIndex = s.size();

            res->push_back(s.substr(startIndex, endIndex - startIndex));
            startIndex = endIndex + 1;
        }

        return res;
    }

    [[nodiscard]] static vector<string>* splitArray(const string& s, const char c = ','){
        auto* res = new vector<string>();
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
            endIndex = string::npos;
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

            if (endIndex == string::npos || endIndex >= s.size())
                endIndex = hasBrackets ? s.size() -1 : s.size();

            res->push_back(s.substr(startIndex, endIndex - startIndex));
            startIndex = endIndex + 1; //TODO startIndex is first char this is not blank space
        }

        return res;
    }

    [[nodiscard]] static vector<string>* splitObjectArray(const string& s, const char c = ','){
        auto* res = new vector<string>();
        size_t startIndex = 0, endIndex;
        size_t i;
        unsigned int bracketCounter = 0;
        bool inStr = false;

        while(s[startIndex] != '{' && startIndex < s.size()) startIndex++;

        while (startIndex < s.size()){
            endIndex = string::npos;
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

            if (endIndex == string::npos || endIndex >= s.size())
                endIndex = s.size();

            res->push_back(s.substr(startIndex, endIndex - startIndex));
            startIndex = endIndex + 1;
        }

        return res;
    }

    [[nodiscard]] static string uncapitalize(const string& s){
        string res(s);
        for (size_t i=0; i<s.size(); i++){
            if(isalpha(res[i]) && isupper(res[i]))
                res[i] = (char)tolower(res[i]);
        }
        return res;
    }

    [[nodiscard]] static string_view rtrim(const string_view s){
        long long size = s.size();
        for(long long i=size;i>=0; i--){
            if (isspace(s[i]))
                size--;
            else break;
        }

        return string_view(s.data(), size + 1);
    }

    [[nodiscard]] static string replace(const string& s, const char pattern, const char replacement) {

        if (s.empty())
            return "";

        string res(s);
        for (size_t i=0; i<s.size(); i++){
            if (res[i] == pattern)
                res[i] = replacement;
        }
        return res;
    }

    [[nodiscard]] static string replace(const string& s, const char *pattern, const char replacement) {

        if (s.empty())
            return "";

        string res;
        size_t start=0,end;
        for (size_t i = 0; i<s.size(); i++){
            end = s.find(pattern, start);
            for (size_t j=start; j<end && j<s.size(); j++)
                res.push_back(s[j]);
            if (end == string::npos) {
                break;
            }
            res.push_back(replacement);
            start = end + 1;
        }
        return res;
    }

    [[nodiscard]] static string toUpperCase(const string& s){
        string res = {s};
        for (char &c : res){
            if (islower(c))
                c = (char)toupper(c);
        }
        return res;
    }

    [[nodiscard]] static string toLowerCase(const string& s){
        string res = {s};
        for (char &c : res){
            if (isupper(c))
                c = (char)tolower(c);
        }
        return res;
    }

    [[nodiscard]] static bool parseBoolean(const char *value) {
        string s{value};
        s = toLowerCase(s);
        if (s == "true")
            return true;
        if (s == "false")
            return false;
        std::cerr << "Received invalid bool value: " << value;
        return false;
    }

    [[nodiscard]] static string parseBoolean(bool value) {
        return value ? "true" : "false";
    }

    [[nodiscard]] static string toCamelCase(const string& value){
        string s;
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

    [[nodiscard]] static inline string to_string(short x){
        return std::to_string((int)x);
    }
    [[nodiscard]] static inline string to_string(bool x){
        return x ? "true" : "false";
    }
    [[nodiscard]] static inline string to_string(std::byte x){
        return std::to_string((char)x);
    }
    [[nodiscard]] static inline string to_string(char x){
        return std::to_string(x);
    }

};

#endif //WINTER_STRINGUTILS_H
