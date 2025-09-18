//
// Created by alienson on 3.4.24.
//

#ifndef WINTER_ENTITY_H
#define WINTER_ENTITY_H

/**
 * Base class for all database entities. ColumnMappings are filled at runtime based on $Column annotation
 */

#include "reflect/Reflect.h"

#define $Column(x)

class Entity : public Reflect {
public:
    Entity() = default;
    virtual ~Entity() = default;

    [[nodiscard]] virtual std::string getTableName() const = 0;
    [[nodiscard]] virtual std::string getPrimaryKeyName() const = 0;

    virtual std::unordered_map<std::string, std::string>& getColumnMappings() const{
        return columnMappings;
    }

private:
    //static std::unordered_map<std::string, std::string> generateMappings();
    static inline std::unordered_map<std::string, std::string> columnMappings;

};


#endif //WINTER_ENTITY_H
