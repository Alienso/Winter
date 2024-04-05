//
// Created by alienson on 3.4.24..
//

#ifndef WINTER_ENTITY_H
#define WINTER_ENTITY_H

/**
 * Base class for all database entities. ColumnMappings are filled at runtime based on $Column annotation
 */

#include "Reflect.h"

#define $Column(x)

using namespace std;

class Entity : public Reflect {
public:
    Entity() = default;
    virtual ~Entity() = default;

    [[nodiscard]] virtual string getTableName() const = 0;
    [[nodiscard]] virtual string getPrimaryKeyName() const = 0;

    virtual unordered_map<string, string>& getColumnMappings() const{
        return columnMappings;
    }

private:
    //static unordered_map<string,string> generateMappings();
    static inline unordered_map<string,string> columnMappings;

};


#endif //WINTER_ENTITY_H
