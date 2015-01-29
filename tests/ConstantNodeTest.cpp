#include <unittest++/UnitTest++.h>
#include <Graph.h>
#include <ConstantNode.h>
#include <ImageSeries.h>
#include <FloatSeries.h>
#include <opencv2/core/core.hpp>
#include <InputConnector.h>
#include <OutputConnector.h>
#include <TestHelper.h>

using namespace taets;

SUITE(ConstantNodeTests) {

TEST(ConstantNodeBasic) {
    std::cout << "ConstantNodeBasic" << std::endl;

    Graph *graph = new Graph(2);
    ConstantNode *node = new ConstantNode(graph, 0);

    NumberSetting* value = static_cast<NumberSetting*>(node->getSettings().front());
    value->setFloatValue(3.14f);
    node->compute();

    FloatSeries *result = static_cast<OutputConnector<FloatSeries>*>(node->outputConnectors()[0])->data();
    CHECK(result->length() == 2);

    CHECK(result->itemAsFloat(0) == 3.14f);
    CHECK(result->itemAsFloat(0) == result->itemAsFloat(1));

}

TEST(ConstantNodeAdvanced) {
    std::cout << "ConstantNodeAdvanced" << std::endl;

    Graph *graph = new Graph(1);
    ConstantNode *node = new ConstantNode(graph, 0);

    NumberSetting* value = static_cast<NumberSetting*>(node->getSettings().front());
    value->setFloatValue(3.14f);
    node->compute();
    FloatSeries *result = static_cast<OutputConnector<FloatSeries>*>(node->outputConnectors()[0])->data();
    float result1 = result->itemAsFloat(0);

    value->setFloatValue(2.56f);
    node->compute();
    result = static_cast<OutputConnector<FloatSeries>*>(node->outputConnectors()[0])->data();
    CHECK(result->itemAsFloat(0) != result1);
}

}
