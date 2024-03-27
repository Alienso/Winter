//
// Created by alienson on 27.3.24..
//

#include "Mapper.h"
#include "Logger.h"

void Mapper::map(Reflect *source, Reflect *dest) const {
    for(auto& sourceField : source->getDeclaredFields()){
        Field* destField = dest->getField(destField->name.data());
        if (destField != &Field::INVALID ) {
            Field::copyValue(source, sourceField, dest, *destField);
        }else{
            if (failOnUnknownProperty){
                wtLogError("Unknown property %s found!", sourceField.name.data());
                return;
            }
        }
    }
}

void Mapper::failOnUnknownProperties(bool val) {
    this->failOnUnknownProperty = val;
}
