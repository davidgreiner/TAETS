#include <unittest++/UnitTest++.h>
#include <Graph.h>
#include <BrightnessContrastNode.h>
#include <ImageSeries.h>
#include <FloatSeries.h>
#include <opencv2/core/core.hpp>
#include <InputConnector.h>
#include <OutputConnector.h>
#include <TestHelper.h>

using namespace taets;

SUITE(BrightnessContrastNodeTests) {

TEST(BrightnessContrastNodeBasic) {
    std::cout << "BrightnessContrastNodeBasic" << std::endl;

    cv::Mat img_src1 = cv::imread("tests/PSNR-example-base.png");
    ImageSeries *image1 = new ImageSeries(2, img_src1.cols, img_src1.rows);
    Graph *graph = new Graph(2);
    BrightnessContrastNode *node = new BrightnessContrastNode(graph, 0);
    cv::Mat m;
    for(int i = 0; i < 2; i++) {
        m = image1->itemAsCVMatrix(i);
        img_src1.copyTo(m);
    }

    OutputConnector<ImageSeries>* src1 = new OutputConnector<ImageSeries>(nullptr, "");
    src1->setData(image1);
    static_cast<InputConnector<ImageSeries>*>(node->inputConnectors()[0])->connect(src1);

    NumberSetting* brightness = static_cast<NumberSetting*>(node->getSettings().front());
    NumberSetting* contrast = static_cast<NumberSetting*>(node->getSettings().at(1));
    brightness->setIntValue(50);
    contrast->setFloatValue(50);
    node->compute();
    ImageSeries *result = static_cast<OutputConnector<ImageSeries>*>(node->outputConnectors()[0])->data();
    CHECK(result->length() == 2);

    cv::Mat test = result->itemAsCVMatrix(0);

    //check that image has been modified
    CHECK(!(MatIsEqual(img_src1, test)));

    //as we read a image, the resulting ImageSeries should have the same output image
    CHECK(MatIsEqual(test, result->itemAsCVMatrix(1)));

}

TEST(BrightnessContrastNodeAdvanced) {
    std::cout << "BrightnessContrastNodeAdvanced" << std::endl;

    cv::Mat img_src1 = cv::imread("tests/PSNR-example-base.png");
    ImageSeries *image1 = new ImageSeries(1, img_src1.cols, img_src1.rows);
    Graph *graph = new Graph(1);
    BrightnessContrastNode *node = new BrightnessContrastNode(graph, 0);
    cv::Mat m = image1->itemAsCVMatrix(0);
    img_src1.copyTo(m);

    OutputConnector<ImageSeries>* src1 = new OutputConnector<ImageSeries>(nullptr, "");
    src1->setData(image1);
    static_cast<InputConnector<ImageSeries>*>(node->inputConnectors()[0])->connect(src1);

    NumberSetting* brightness = static_cast<NumberSetting*>(node->getSettings().front());
    NumberSetting* contrast = static_cast<NumberSetting*>(node->getSettings().at(1));
    brightness->setIntValue(10);
    contrast->setFloatValue(60);
    node->compute();
    ImageSeries *result = static_cast<OutputConnector<ImageSeries>*>(node->outputConnectors()[0])->data();
    cv::Mat test1 = result->itemAsCVMatrix(0);

    brightness->setIntValue(70);
    contrast->setFloatValue(20);
    node->compute();
    result = static_cast<OutputConnector<ImageSeries>*>(node->outputConnectors()[0])->data();
    cv::Mat test2 = result->itemAsCVMatrix(0);

    brightness->setIntValue(100);
    contrast->setFloatValue(0);
    node->compute();
    result = static_cast<OutputConnector<ImageSeries>*>(node->outputConnectors()[0])->data();
    cv::Mat test3 = result->itemAsCVMatrix(0);

    CHECK(!MatIsEqual(test1, test2));
    CHECK(!MatIsEqual(test2, test3));
}

}
