#include <unittest++/UnitTest++.h>
#include <Graph.h>
#include <GradientInputNode.h>
#include <ImageSeries.h>
#include <opencv2/core/core.hpp>
#include <InputConnector.h>
#include <OutputConnector.h>
#include <TestHelper.h>
#include "Color.h"

using namespace taets;

SUITE(GradientInputNodeTests) {

TEST(GradientInputNodeBasic) {
    std::cout << "GradientInputNodeBasic" << std::endl;

    Graph *graph = new Graph(2);
    GradientInputNode *node = new GradientInputNode(graph, 0);

    NumberSetting* height = static_cast<NumberSetting*>(node->getSettings().front());
    NumberSetting* width = static_cast<NumberSetting*>(node->getSettings().at(1));
    ColorSetting* start_color = static_cast<ColorSetting*>(node->getSettings().at(2));
    ColorSetting* end_color = static_cast<ColorSetting*>(node->getSettings().at(3));
    width->setIntValue(100);
    height->setIntValue(100);
    Color red(255, 0, 0);
    Color blue(0, 0, 255);
    start_color->setColor(red);
    end_color->setColor(blue);
    node->compute();

    ImageSeries *result = static_cast<OutputConnector<ImageSeries>*>(node->outputConnectors()[0])->data();
    CHECK(result->length() == 2);

    cv::Mat test = result->itemAsCVMatrix(0);
    //start color should be red
    CHECK(test.at<cv::Vec3b>(0,0)[0] == 0);
    CHECK(test.at<cv::Vec3b>(0,0)[1] == 0);
    CHECK(test.at<cv::Vec3b>(0,0)[2] == 255);
    //end color should be blue (r and b channels are switched)
    CHECK(test.at<cv::Vec3b>(100, 100)[0] == 0);
    CHECK(test.at<cv::Vec3b>(100, 100)[1] == 0);
    CHECK(test.at<cv::Vec3b>(100, 100)[2] == 255);
}

TEST(GradientInputNodeAdvanced) {
    std::cout << "GradientInputNodeAdvanced" << std::endl;

    Graph *graph = new Graph(1);
    GradientInputNode *node = new GradientInputNode(graph, 0);

    NumberSetting* height = static_cast<NumberSetting*>(node->getSettings().front());
    NumberSetting* width = static_cast<NumberSetting*>(node->getSettings().at(1));
    ColorSetting* start_color = static_cast<ColorSetting*>(node->getSettings().at(2));
    ColorSetting* end_color = static_cast<ColorSetting*>(node->getSettings().at(3));

    width->setIntValue(100);
    height->setIntValue(100);
    Color red(255, 0, 0);
    Color blue(0, 0, 255);
    start_color->setColor(red);
    end_color->setColor(blue);
    node->compute();
    ImageSeries *result = static_cast<OutputConnector<ImageSeries>*>(node->outputConnectors()[0])->data();
    cv::Mat test1 = result->itemAsCVMatrix(0);

    Color green(0, 255, 0);
    Color gray(127, 127, 127);
    start_color->setColor(green);
    end_color->setColor(gray);
    node->compute();
    result = static_cast<OutputConnector<ImageSeries>*>(node->outputConnectors()[0])->data();
    cv::Mat test2 = result->itemAsCVMatrix(0);

    CHECK(!MatIsEqual(test1, test2));
}

}
