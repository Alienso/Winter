//
// Created by Alienson on 3.2.2024..
//

#ifndef WINTER_MYSERVICE_H
#define WINTER_MYSERVICE_H

#include "../core/Component.h"

class MyService : public Component {

public:
    float getSquare(float x){
        return x*x;
    }

};


#endif //WINTER_MYSERVICE_H
