#include <unittest++/UnitTest++.h>
#include <Graph.h>
#include <ColorNode.h>
#include <ImageSeries.h>
#include <opencv2/core/core.hpp>
#include <InputConnector.h>
#include <OutputConnector.h>
#include <TestHelper.h>
#include <ColorSetting.h>
#include "Color.h"

using namespace taets;

SUITE(ColorNodeTests) {

TEST(ColorNodeBasic) {
    std::cout << "ColorNodeBasic" << std::endl;

    Graph *graph = new Graph(2);
    ColorNode *node = new ColorNode(graph, 0);

    NumberSetting* height = static_cast<NumberSetting*>(node->getSettings().front());
    NumberSetting* width = static_cast<NumberSetting*>(node->getSettings().at(1));
    ColorSetting* color = static_cast<ColorSetting*>(node->getSettings().at(2));
    width->setIntValue(100);
    height->setIntValue(100);
    Color green(0, 255, 0);
    color->setColor(green);
    node->compute();

    ImageSeries *result = static_cast<OutputConnector<ImageSeries>*>(node->outputConnectors()[0])->data();
    CHECK(result->length() == 2);

    cv::Mat test = result->itemAsCVMatrix(0);
    //mat should be green only
    cv::Mat greenOnly = cv::Mat(cvSize(100, 100), CV_8UC3, cv::Scalar(0, 255, 0));
    CHECK(MatIsEqual(test, greenOnly));
}

TEST(ColorNodeAdvanced) {
    std::cout << "ColorNodeAdvanced" << std::endl;

    Graph *graph = new Graph(1);
    ColorNode *node = new ColorNode(graph, 0);

    NumberSetting* height = static_cast<NumberSetting*>(node->getSettings().front());
    NumberSetting* width = static_cast<NumberSetting*>(node->getSettings().at(1));
    ColorSetting* color = static_cast<ColorSetting*>(node->getSettings().at(2));

    width->setIntValue(100);
    height->setIntValue(100);
    Color color1(127, 127, 127);
    color->setColor(color1);
    node->compute();
    ImageSeries *result = static_cast<OutputConnector<ImageSeries>*>(node->outputConnectors()[0])->data();
    cv::Mat test1 = result->itemAsCVMatrix(0);

    Color color2(0, 0, 0);
    color->setColor(color2);
    node->compute();
    result = static_cast<OutputConnector<ImageSeries>*>(node->outputConnectors()[0])->data();
    cv::Mat test2 = result->itemAsCVMatrix(0);

    Color color3(255, 255, 255);
    color->setColor(color3);
    node->compute();
    result = static_cast<OutputConnector<ImageSeries>*>(node->outputConnectors()[0])->data();
    cv::Mat test3 = result->itemAsCVMatrix(0);

    CHECK(!MatIsEqual(test1, test2));
    CHECK(!MatIsEqual(test2, test3));
}
}
