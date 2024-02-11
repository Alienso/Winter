//
// Created by Alienson on 6.2.2024..
//

#include "Field.h"

void *Field::getAddress(void *object) const {
    return (int*)(object) + offset;
}
