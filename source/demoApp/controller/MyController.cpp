//
// Created by Alienson on 3.2.2024..
//

#include "MyController.h"
#include "../dto/BaseRequest.h"
#include "../dto/BaseResponse.h"
#include "client/HttpClient.h"

JsonDeserializer MyController::deserializer = {};

HttpResponse *MyController::home(HttpRequest *httpRequest) {
    auto* request = (BaseRequest*) deserializer.deserialize(httpRequest->getRequestBody(),new BaseRequest());

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

    return new HttpResponse(response, HttpStatus::OK);
}

HttpResponse *MyController::player(HttpRequest *request) {

    string playerIdStr = (request->getQueryParameters()).find("id")->second;
    long playerId = stol(playerIdStr);

    auto* players = playerRepository->getAllPlayers();
    PlayerEntity* player = playerRepository->getSinglePlayer();
    int playerCount = playerRepository->getPlayerCount();
    PlayerEntity* player2 = playerRepository->getPlayerById(playerId);

    int success;

    auto* newPlayer = new PlayerEntity();
    newPlayer->id = 3;
    newPlayer->name = "New Player";
    newPlayer->createdOn = "2024-04-05 12:00:00.000";
    success = PlayerRepository::insert(newPlayer);

    newPlayer->name = "New New Player";
    success = PlayerRepository::update(newPlayer);

    success = playerRepository->deleteEntity(newPlayer);

    wtLogInfo("There are %d players in database", playerCount);

    for (auto* p : *players)
        delete p;
    delete players;
    delete player;
    delete player2;
    delete newPlayer;

    return new HttpResponse(HttpStatus::OK);
}

HttpResponse *MyController::clientTest(HttpRequest *request) {
    shared_ptr<HttpClient> client = HttpClient::builder()
            .baseUrl("/home")
            .build();

    InnerClass body(1.0,0.5,"Val");
    int res = client->get()->uri("/v2").bodyValue(&body).retrieve().block<int>();
    return new HttpResponse(HttpStatus::OK);
}

