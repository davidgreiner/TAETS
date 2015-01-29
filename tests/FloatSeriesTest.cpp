#include <unittest++/UnitTest++.h>
#include "FloatSeries.h"

using namespace taets;

SUITE(FloatSeriesTests) {

TEST(FloatSeriesBasic) {
    std::cout << "FloatSeriesBasic" << std::endl;
    FloatSeries* test = new FloatSeries(4);
    test->setFloatItem(0, 0);
    test->setFloatItem(1, 1);
    test->setFloatItem(2, 2.71);
    test->setFloatItem(3, 3.14);

    //basic test scenarios
    CHECK_THROW(test->setFloatItem(4, 5), std::out_of_range);
    CHECK_THROW(test->item(5), std::out_of_range);
    CHECK_THROW(test->itemAsFloat(1337), std::out_of_range);

}

}
