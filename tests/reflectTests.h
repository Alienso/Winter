//
// Created by alienson on 28.3.24..
//

#ifndef WINTER_REFLECTTESTS_H
#define WINTER_REFLECTTESTS_H

#endif //WINTER_REFLECTTESTS_H

TEST_CASE("Field copy value", "Field::copyValue"){

    auto* source = new AllFieldsDTO();
    source->i = 1;
    source->f = 2.5f;

    auto* dest = new AllFieldsDTO();

    SECTION("copy value int int") {
        Field::copyValue(source, *source->getField("i"), dest, *dest->getField("i"));
        REQUIRE(dest->i == 1);
    }
}