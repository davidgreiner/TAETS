#include "boost_class_exports.h"
#include <TestData.h>

namespace taets {

void TestData::save(std::string filename) const
{
    std::ofstream fs(filename.c_str());
    boost::archive::binary_oarchive ar(fs);
    ar << *this;
}

TestData* TestData::load(std::string filename)
{
    TestData *data = new TestData();

    std::ifstream fs(filename.c_str());
    boost::archive::binary_iarchive ar(fs);

    ar >> *data;

    return data;
}

} //end namespace taets
