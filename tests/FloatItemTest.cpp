#include <unittest++/UnitTest++.h>
#include <iostream>
#include "FloatItem.h"


using namespace taets;

SUITE(FloatItemTests) {

TEST(FloatItemBasic) {
    std::cout << "FloatItemBasic" << std::endl;
    float value = 3.14f;
    FloatItem* test = new FloatItem(&value);

    //basic test scenarios
    CHECK(value == test->value());

    float newValue = 2.71;
    test->setValue(newValue);
    CHECK(newValue == test->value());
}

}
