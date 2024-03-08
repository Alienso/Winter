//
// Created by Alienson on 3.2.2024..
//

#include "MyController.h"
#include "../dto/BaseRequest.h"
#include "../dto/BaseResponse.h"

JsonDeserializer MyController::deserializer = {};

HttpResponse *MyController::home(HttpRequest *httpRequest) {
    auto* request = (BaseRequest*) deserializer.deserialize(httpRequest->getRequestBody(),new BaseRequest());

    //Do some logic
    wtLogInfo("Received data: number= %d, type=%s", request->number, request->type.data());
    wtLogInfo("InnerClass: x=%f, y=%f, c=%s", request->innerClass->x, request->innerClass->y, request->innerClass->c.data());
    wtLogInfo("List: ", request->values);

    InnerClass* inner = new InnerClass();
    inner->x = request->innerClass->x * 2;
    inner->y = request->innerClass->y * 2;
    inner->c = request->innerClass->c + "X2";

    int sum = 0;
    for (int value : request->values)
        sum+=value;

    auto* response = new BaseResponse();
    response->code = 0;
    response->message = "OK";
    response->innerClass = inner;
    response->sum = sum;

    return new HttpResponse(response, HttpCode::OK);
}
