//
// Created by Alienson on 23.9.2025..
//

#ifndef WINTER_HTTPHEADER_H
#define WINTER_HTTPHEADER_H

#include <string>

class HttpHeader{
private:
    HttpHeader() = delete;
public:
    static inline std::string ACCEPT = "Accept";
    static inline std::string ALLOW = "Allow";
    static inline std::string AUTHORIZATION = "Authorization";
    static inline std::string CACHE_CONTROL = "Cache-Control";
    static inline std::string CONTENT_ENCODING = "Content-Encoding";
    static inline std::string CONTENT_DISPOSITION = "Content-Disposition";
    static inline std::string CONTENT_LENGTH = "Content-Length";
    static inline std::string CONTENT_TYPE = "Content-Type";
    static inline std::string COOKIE = "Cookie";
    static inline std::string DATE = "Date";
    static inline std::string HOST = "Host";
    static inline std::string LOCATION = "Location";
    static inline std::string ORIGIN = "Origin";
    static inline std::string USER_AGENT = "User-Agent";
    //static inline std::string  = "";
};

#endif //WINTER_HTTPHEADER_H
