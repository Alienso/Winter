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

    static string trim(string& s){
        size_t start = 0, end = s.size();
        for(int i=0; i < s.size(); i++){
            if (!isspace(s[i])){
                start = i;
                break;
            }
        }
        for(int i=s.size(); i > 0; i--){
            if (!isspace(s[i])){
                end = i;
                break;
            }
        }
        return s.substr(start, end - start);
    }

    static string stripBlankCharacters(string& s){
        char res[s.length()];
        int i,j;
        for (i=0,j=0; i<s.size(); i++)
            if (!isspace(s[i]))
                res[j++] = s[i];
        res[j] = '\0';
        return {res};
    }

    static string stripSpecialCharacters(string& s) {
        char res[s.length()];
        int i,j;
        for (i=0,j=0; i<s.size(); i++)
            if (isalnum(s[i]))
                res[j++] = s[i];
        res[j] = '\0';
        return {res};
    }

    static bool endsWith(string& s, const char* suffix) {
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

    static bool startsWith(string& s, const char* prefix) {
        return startsWith(s.data(), prefix);
    }

    static bool startsWith(const char*s, const char* prefix) {
        for (int i=0; true; i++) {
            if (prefix[i] == '\0' || s[i] == '\0')
                return true;
            else if (s[i] != prefix[i])
                return false;
        }
    }

    static vector<string>* split(string& s, char c){
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

    static vector<string>* splitArray(string& s, char c = ','){
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

    static string_view rtrim(string_view s){
        int size = s.size();
        for(int i=size;i>=0; i--){
            if (isspace(s[i]))
                size--;
            else break;
        }

        return string_view(s.data(), size + 1);
    }
};

#endif //WINTER_STRINGUTILS_H