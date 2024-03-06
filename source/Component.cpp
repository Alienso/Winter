//
// Created by Alienson on 5.3.2024..
//

#include "Component.h"

#include "app/controller/MyController.h"

int Component::id = 0;
vector<Component*> Component::components = {};

//TEMP
void Component::initializeComponents() {
    wtLogTrace("Initializing components");
    auto* myController = new MyController();
    components.push_back((Component*)myController);
    MyController::id = 0;
}