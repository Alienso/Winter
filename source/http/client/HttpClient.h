//
// Created by alienson on 5.4.24..
//

#ifndef WINTER_HTTPCLIENT_H
#define WINTER_HTTPCLIENT_H

#include "httpConstants.h"
#include "Reflect.h"
#include "HttpRequest.h"

#include <unordered_map>
#include <string>
#include <utility>
#include <memory>


using namespace std;

class HttpClient;

class HttpClientBuilder{
public:
    [[nodiscard]] shared_ptr<HttpClient> build() const;

    HttpClientBuilder& baseUrl(const string& uri);
    HttpClientBuilder& defaultCookie(const string& cookie, const string& value);
    HttpClientBuilder& defaultCookies(void (*f)(unordered_map<string,string>& cookies));
    HttpClientBuilder& defaultHeader(const string& header, const string& value);
    HttpClientBuilder& defaultHeaders(void (*f)(unordered_map<string,string>& headers));

private:
    string baseUrlStr = "";
    unordered_map<string,string> cookies = {};
    unordered_map<string,string> headers = {};
};

class ResponseSpec{
public:
    ResponseSpec(wt::web::Connection* connection) : connection(connection) {}

    template<typename T>
    [[nodiscard]] T block(){
        std::cout << connection->getDataFromSocket();
    }
    template<typename T>
    [[nodiscard]] T subscribe(/*predicate*/){
        std::cout << connection->getDataFromSocket();
    }
    void onStatus(/*predicate*/){
        return;
    }

private:
    wt::web::Connection* connection;
};

class RequestSpec{
public:

    RequestSpec(HttpClient* client, HttpMethod* method);

    [[nodiscard]] ResponseSpec retrieve();

    RequestSpec& uri(const char* uri);
    RequestSpec& cookie(const string& cookie, const string& value);
    RequestSpec& cookies(void (*f)(unordered_map<string,string>& cookies));
    RequestSpec& header(const string& header, const string& value);
    RequestSpec& headers(void (*f)(unordered_map<string,string>& headers));

    RequestSpec& body(const string& value);
    RequestSpec& bodyValue(Reflect* bodyObj);
    RequestSpec& contentType(MediaType* mediaType);

private:
    HttpRequest httpRequest;
    string url = "";
};

class HttpClient {
public:

    HttpClient(string  baseUri, unordered_map<string,string>  cookies, unordered_map<string,string> headers) :
    baseUrl(std::move(baseUri)), cookies(std::move(cookies)), headers(std::move(headers)) {}

    [[nodiscard]] static HttpClientBuilder builder();

    [[nodiscard]] shared_ptr<RequestSpec> get();
    [[nodiscard]] shared_ptr<RequestSpec> post();
    [[nodiscard]] shared_ptr<RequestSpec> put();
    [[nodiscard]] shared_ptr<RequestSpec> Delete();
    [[nodiscard]] shared_ptr<RequestSpec> options();
    [[nodiscard]] shared_ptr<RequestSpec> patch();
    [[nodiscard]] shared_ptr<RequestSpec> connect();
    [[nodiscard]] shared_ptr<RequestSpec> trace();

    friend RequestSpec;

private:
    string baseUrl = "";
    unordered_map<string,string> cookies = {};
    unordered_map<string,string> headers = {};
};


#endif //WINTER_HTTPCLIENT_H
