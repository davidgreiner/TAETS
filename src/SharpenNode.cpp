#include "SharpenNode.h"

namespace taets {

SharpenNode::SharpenNode(Graph* graph, unsigned int id) :
    ModifierNode(graph, id, tr("Sharpen Node").toStdString()) {
    _inputConnectors.push_back(new InputConnector<ImageSeries>(this, tr("image").toStdString()));
    _outputConnectors.push_back(new OutputConnector<ImageSeries>(this, tr("sharpened image").toStdString()));
    std::vector<std::string> choices = {"unsharp masking", "laplace"};
    _settings.push_back(new ChoiceSetting(tr("sharpening methods").toStdString(), choices, 0));
}


/**
 * Schärft das Eingabebild
 */
void SharpenNode::compute() {
    //Einstellungen parsen
    if(static_cast<OutputConnector<ImageSeries>* >(this->inputConnectors().front()->source())->data() == NULL) {
        throw std::runtime_error(tr("No data input.").toStdString());
    }

    int length = static_cast<OutputConnector<ImageSeries>* >(this->inputConnectors().front()->source())->data()->length();
    ChoiceSetting* method_c = static_cast<ChoiceSetting*>(_settings.front());
    std::string method = method_c->selectedString();
    int width = static_cast<OutputConnector<ImageSeries>* >(this->inputConnectors().front()->source())->data()->width();
    int height = static_cast<OutputConnector<ImageSeries>* >(this->inputConnectors().front()->source())->data()->height();

    //Benötigte Objekte initialisieren
    ImageSeries *out = new ImageSeries(length, width, height);
    cv::Mat img, sharpened, m;

    for(int i = 0; i < length; i++) {
        img = static_cast<OutputConnector<ImageSeries>* >(this->inputConnectors().front()->source())->data()->itemAsCVMatrix(i);
        sharpened = out->itemAsCVMatrix(i);

        if(method == "unsharp masking") {
            cv::GaussianBlur(img, sharpened, cv::Size(5, 5), 5);
            cv::addWeighted(img, 1.5, sharpened, -0.5, 0, sharpened);

        } else if(method == "laplace") {
            cv::GaussianBlur(img, img, cv::Size(3, 3), 0, 0);
            cv::Mat kernel = (cv::Mat_<float>(3,3) <<
                1,  1, 1,
                1, -8, 1,
                1,  1, 1);
            cv::Mat laplace;
            cv::filter2D(img, laplace, CV_32F, kernel, cv::Point( -1, -1 ), 0, 0);
            img.convertTo(img, CV_32F);
            m = img - laplace;
            m.convertTo(m, CV_8U);
            m.copyTo(sharpened);
        }
    }

    static_cast<OutputConnector<ImageSeries>*>(_outputConnectors[0])->setData(out);
}


}
