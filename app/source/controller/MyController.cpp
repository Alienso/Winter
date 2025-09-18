//
// Created by Alienson on 3.2.2024.
//

#include "MyController.h"
#include "../dto/BaseRequest.h"
#include "../dto/BaseResponse.h"

JsonDeserializer MyController::deserializer = {};

HttpResponse *MyController::home(HttpRequest *httpRequest) {
    auto* request = (BaseRequest*) deserializer.deserialize(httpRequest->getRequestBody(), new BaseRequest());

    //Do some logic
    wtLogInfo("Received data: number= %d, type=%s", request->number, request->type.data());
    wtLogInfo("InnerClass: x=%f, y=%f, c=%s", request->innerClass->x, request->innerClass->y, request->innerClass->c.data());
    wtLogInfo("List: ", request->values);

    InnerClass* inner = new InnerClass();
    inner->x = myService->getSquare(request->innerClass->x);
    inner->y = myService->getSquare(request->innerClass->y);
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

HttpResponse *MyController::player(HttpRequest *request) {

    std::string playerIdStr = (request->getQueryParameters()).find("id")->second;
    long playerId = stol(playerIdStr);

    auto players = playerRepository->getAllPlayers();
    std::shared_ptr<PlayerEntity> player = playerRepository->getSinglePlayer();
    int playerCount = playerRepository->getPlayerCount();
    std::shared_ptr<PlayerEntity> player2 = playerRepository->getPlayerById(playerId);

    int success; //For debug

    auto* newPlayer = new PlayerEntity();
    newPlayer->id = 3;
    newPlayer->name = "New Player";
    newPlayer->createdOn = "2024-04-05 12:00:00.000";
    success = PlayerRepository::insert(newPlayer);

    newPlayer->name = "New New Player";
    success = PlayerRepository::update(newPlayer);

    success = playerRepository->deleteEntity(newPlayer);

    wtLogInfo("There are %d players in database", playerCount);

    return new HttpResponse(HttpCode::OK);
}
