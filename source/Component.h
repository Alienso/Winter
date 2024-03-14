//
// Created by Alienson on 5.3.2024..
//

#ifndef WINTER_COMPONENT_H
#define WINTER_COMPONENT_H

#include <vector>
#include <iostream>
#include "Logger.h"

using namespace std;

class Component{
public:
    Component() = default;
    ~Component(){
        for (auto x : components)
            delete x;
    }

    //this method is generated
    static void initializeComponents();

    static void addComponent(Component* component){
        components.push_back(component);
    }

    [[nodiscard]] static Component* getById(int id){
        wtLogTrace("Get Called for ID %d", id);
        return components[id];
    }

    //since this should be singleton, ids can be non-static
    int _componentId_ = 0;

private:
    static vector<Component*> components;
};
#endif //WINTER_COMPONENT_H
