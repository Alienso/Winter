//
// Created by Alienson on 3.2.2024..
//

#include "MyController.h"
#include "../dto/BaseRequest.h"
#include "../dto/BaseResponse.h"

JsonDeserializer MyController::deserializer = {};

HttpResponse *MyController::home(HttpRequest *httpRequest) {
    auto* request = (BaseRequest*) deserializer.deserialize(httpRequest->getRequestBody(),new BaseRequest());

    //TODO const char* is not deserialized
    //Do some logic
    wtLogInfo("Received data: number= %d, type=%s", request->number, request->type.data());

    auto* response = new BaseResponse();
    response->code = 0;
    response->message = "OK";

    return new HttpResponse(response, HttpCode::OK);
}
