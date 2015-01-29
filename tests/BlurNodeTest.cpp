#include <unittest++/UnitTest++.h>
#include <Graph.h>
#include <BlurNode.h>
#include <ImageSeries.h>
#include <FloatSeries.h>
#include <opencv2/core/core.hpp>
#include <InputConnector.h>
#include <OutputConnector.h>
#include <TestHelper.h>

using namespace taets;

SUITE(BlurNodeTests) {

TEST(BlurNodeBasic) {
    std::cout << "BlurNodeBasic" << std::endl;

    cv::Mat img_src1 = cv::imread("tests/PSNR-example-base.png");
    ImageSeries *image1 = new ImageSeries(2, img_src1.cols, img_src1.rows);
    Graph *graph = new Graph(2);
    BlurNode *node = new BlurNode(graph, 0);
    cv::Mat m;
    for(int i = 0; i < 2; i++) {
        m = image1->itemAsCVMatrix(i);
        img_src1.copyTo(m);
    }

    OutputConnector<ImageSeries>* src1 = new OutputConnector<ImageSeries>(nullptr, "");
    src1->setData(image1);
    static_cast<InputConnector<ImageSeries>*>(node->inputConnectors()[0])->connect(src1);

    ChoiceSetting* method_c = static_cast<ChoiceSetting*>(node->getSettings().front());
    NumberSetting* size_c = static_cast<NumberSetting*>(node->getSettings().at(1));
    method_c->setSelection(1);
    size_c->setIntValue(40);
    node->compute();

    ImageSeries *result = static_cast<OutputConnector<ImageSeries>*>(node->outputConnectors()[0])->data();
    CHECK(result->length() == 2);

    cv::Mat test = result->itemAsCVMatrix(0);

    //check that image has been modified
    CHECK(!(MatIsEqual(img_src1, test)));

    //as we read a image, the resulting ImageSeries should have the same output image
    CHECK(MatIsEqual(test, result->itemAsCVMatrix(1)));

}

TEST(BlurNodeAdvanced) {
    std::cout << "BlurNodeAdvanced" << std::endl;

    cv::Mat img_src1 = cv::imread("tests/PSNR-example-base.png");
    ImageSeries *image1 = new ImageSeries(1, img_src1.cols, img_src1.rows);
    Graph *graph = new Graph(1);
    BlurNode *node = new BlurNode(graph, 0);
    cv::Mat m = image1->itemAsCVMatrix(0);;
    img_src1.copyTo(m);

    OutputConnector<ImageSeries>* src1 = new OutputConnector<ImageSeries>(nullptr, "");
    src1->setData(image1);
    static_cast<InputConnector<ImageSeries>*>(node->inputConnectors()[0])->connect(src1);
    ChoiceSetting* method_c = static_cast<ChoiceSetting*>(node->getSettings().front());
    NumberSetting* size_c = static_cast<NumberSetting*>(node->getSettings().at(1));
    method_c->setSelection(0);
    size_c->setIntValue(18);
    node->compute();
    ImageSeries *result = static_cast<OutputConnector<ImageSeries>*>(node->outputConnectors()[0])->data();
    cv::Mat test1 = result->itemAsCVMatrix(0);

    method_c->setSelection(1);
    node->compute();
    result = static_cast<OutputConnector<ImageSeries>*>(node->outputConnectors()[0])->data();
    cv::Mat test2 = result->itemAsCVMatrix(0);

    method_c->setSelection(2);
    node->compute();
    result = static_cast<OutputConnector<ImageSeries>*>(node->outputConnectors()[0])->data();
    cv::Mat test3 = result->itemAsCVMatrix(0);

    method_c->setSelection(3);
    node->compute();
    result = static_cast<OutputConnector<ImageSeries>*>(node->outputConnectors()[0])->data();
    cv::Mat test4 = result->itemAsCVMatrix(0);

    CHECK(!MatIsEqual(test1, test2));
    CHECK(!MatIsEqual(test2, test3));
    CHECK(!MatIsEqual(test3, test4));
    }

}
