//
// Created by Alienson on 31.1.2024..
//

#ifndef WINTER_STRINGUTILS_H
#define WINTER_STRINGUTILS_H

#include <vector>
#include <string>
#include <string_view>

using namespace std;

class StringUtils{
public:

    [[nodiscard]] static string trim(const string& s){
        size_t start = 0, end = s.size();
        for(size_t i=0; i < s.size(); i++){
            if (!isspace(s[i])){
                start = i;
                break;
            }
        }
        for(int i=s.size() - 1; i > 0; i--){
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
        int suffixSize = 0;
        for (;suffix[suffixSize] != '\0'; suffixSize++);
        suffixSize--;
        unsigned int offset = s.length() - suffixSize - 1;

        int i=suffixSize;
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
        for (int i=0; true; i++) {
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
        int bracketCounter = 0;
        bool inStr = false;
        while (startIndex < s.size()){
            endIndex = string::npos;
            for (i=startIndex; i<s.size(); i++){
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
        int size = s.size();
        for(int i=size;i>=0; i--){
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
};

#endif //WINTER_STRINGUTILS_H
