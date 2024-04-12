//
// Created by alienson on 5.4.24..
//

#include "HttpClient.h"

#include <memory>
#include "JsonSerializer.h"

HttpClientBuilder &HttpClientBuilder::baseUrl(const string &uri) {
    this->baseUrlStr = uri;
    return *this;
}

HttpClientBuilder &HttpClientBuilder::defaultCookie(const string &cookie, const string &value) {
    this->cookies[cookie] = value;
    return *this;
}

HttpClientBuilder &HttpClientBuilder::defaultCookies(void (*f)(unordered_map<string,string>& cookies)) {
    f(this->cookies);
    return *this;
}

HttpClientBuilder &HttpClientBuilder::defaultHeader(const string &header, const string &value) {
    this->headers[header] = value;
    return *this;
}

HttpClientBuilder &HttpClientBuilder::defaultHeaders(void (*f)(unordered_map<string,string>& headers)) {
    f(this->headers);
    return *this;
}

shared_ptr<HttpClient> HttpClientBuilder::build() const {
    return std::make_shared<HttpClient>(baseUrlStr, cookies, headers);
}


//-----------------------------------------------------

HttpClientBuilder HttpClient::builder() {
    return HttpClientBuilder();
}

shared_ptr<RequestSpec> HttpClient::get() {
    return make_shared<RequestSpec>(this, HttpMethod::GET);
}

shared_ptr<RequestSpec> HttpClient::post() {
    return make_shared<RequestSpec>(this, HttpMethod::POST);
}

shared_ptr<RequestSpec> HttpClient::put() {
    return make_shared<RequestSpec>(this, HttpMethod::PUT);
}

shared_ptr<RequestSpec> HttpClient::Delete() {
    return make_shared<RequestSpec>(this, HttpMethod::DELETE_);
}

shared_ptr<RequestSpec> HttpClient::options() {
    return make_shared<RequestSpec>(this, HttpMethod::OPTIONS);
}

shared_ptr<RequestSpec> HttpClient::patch() {
    return make_shared<RequestSpec>(this, HttpMethod::PATCH);
}

shared_ptr<RequestSpec> HttpClient::connect() {
    return make_shared<RequestSpec>(this, HttpMethod::CONNECT);
}

shared_ptr<RequestSpec> HttpClient::trace() {
    return make_shared<RequestSpec>(this, HttpMethod::TRACE);
}


//--------------------------------------------------------------


RequestSpec::RequestSpec(HttpClient *client, HttpMethod *method) {
    this->url = client->baseUrl;
    httpRequest.setMethod(method);
    httpRequest.setHttpHeaders(client->headers);
    //this->cookiesMap = client->cookies;
}

RequestSpec &RequestSpec::uri(const char *uri) {
    url += uri;
    return *this;
}

RequestSpec &RequestSpec::cookie(const string &cookie, const string &value) {
    //cookiesMap[cookie] = value;
    return *this;
}

RequestSpec &RequestSpec::cookies(void (*f)(unordered_map<string, string> &)) {
    //f(cookiesMap);
    return *this;
}

RequestSpec &RequestSpec::header(const string &header, const string &value) {
    httpRequest.getHttpHeaders()[header] = value;
    return *this;
}

RequestSpec &RequestSpec::headers(void (*f)(unordered_map<string, string> &)) {
    f(httpRequest.getHttpHeaders());
    return *this;
}

RequestSpec &RequestSpec::body(const string &value) {
    httpRequest.setBody(value);
    return *this;
}

RequestSpec &RequestSpec::bodyValue(Reflect *bodyObj) {
    string* res = JsonSerializer::serialize(bodyObj);
    httpRequest.setBody(*res);
    delete res;
    return *this;
}

RequestSpec &RequestSpec::contentType(MediaType *mediaType) {
    httpRequest.getHttpHeaders()["Content-Type"] = mediaType->value;
    return *this;
}

ResponseSpec RequestSpec::retrieve() {
    asio::ip::tcp::resolver::query query("echo.free.beeceptor.com", "https");
    asio::io_context io_context;
    asio::ip::tcp::resolver resolver(io_context);
    asio::ip::tcp::socket socket(io_context);

    for (const auto& ep : resolver.resolve(query)) {
        //socket.connect(ep.endpoint());
        error_code ec;
        asio::ip::address ip_address = asio::ip::address::from_string("127.0.0.1", ec);
        asio::ip::tcp::endpoint endpoint(ip_address, 8080);
        socket.connect(endpoint);
        break;
    }

    if (!socket.is_open()){
        wtLogError("Error while sending request to: HOST");
        return ResponseSpec(nullptr);
    }

    /*auto* conn = new wt::web::Connection(std::move(socket));
    httpRequest.setConnection(conn);
    httpRequest.setUri(this->url.data());
    httpRequest.send();

    httpRequest.getConnection()->getDataFromSocket();

    return ResponseSpec(httpRequest.getConnection());*/

    string response = "GET /home/v2 HTTP/1.1\n\n{\n\"x\":1.000000,\n\"y\":0.500000,\n\"c\":\"Val\"\n}\n";
    const char* ptr = &(response[0]);
    socket.write_some(asio::buffer(ptr, response.size()));


    char tempRequestBuffer[2048];
    std::vector<char> requestData;
    int bufferSize = sizeof(tempRequestBuffer);
    while (int read = socket.read_some(asio::buffer(tempRequestBuffer))){
        std::size_t oldSize = requestData.size();
        requestData.resize(requestData.size() + read);
        memcpy(&(requestData[oldSize]), tempRequestBuffer, read);
        if (read != bufferSize){
            //tryParseRequest();
            break;
        }
    }

    httpRequest.getConnection()->getDataFromSocket();

    return ResponseSpec(httpRequest.getConnection());
}
