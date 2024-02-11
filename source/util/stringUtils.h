//
// Created by Alienson on 31.1.2024..
//

#ifndef WINTER_STRINGUTILS_H
#define WINTER_STRINGUTILS_H

#include <string>
#include <string_view>

using namespace std;

class StringUtils{
public:
    static bool isBlankChar(char c){
        return c == ' ' || c == '\n' || c == '\r' || c == '\t';
    }

    static string trim(string& s){
        unsigned int start = 0, end = s.size();
        for(int i=0; i < s.size(); i++){
            if (!isBlankChar(s[i])){
                start = i;
                break;
            }
        }
        for(int i=s.size(); i > 0; i--){
            if (!isBlankChar(s[i])){
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
            if (!isBlankChar(s[i]))
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

    static string_view rtrim(string_view s){
        int size = s.size();
        for(int i=size;i>=0; i--){
            if (isBlankChar(s[i]))
                size--;
            else break;
        }

        return string_view(s.data(), size + 1);
    }
};

#endif //WINTER_STRINGUTILS_H