//
// Created by alienson on 27.3.24.
//

#include "Mapper.h"
#include "Logger.h"

void Mapper::map(Reflect *source, Reflect *dest, FieldMatchType fieldMatchType) const {
    for(auto& sourceField : source->getDeclaredFields()){
        Field* destField;
        if (fieldMatchType == FIELD_MATCH_TYPE_RELAXED)
            destField = dest->getField(StringUtils::toLowerCase(sourceField.name).data());
        else destField = dest->getField(sourceField.name.data());
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
