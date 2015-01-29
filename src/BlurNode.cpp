#include "BlurNode.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace taets {

BlurNode::BlurNode(Graph* graph, unsigned int id) :
    ModifierNode(graph, id, tr("Blur Node").toStdString()) {
    _inputConnectors.push_back(new InputConnector<ImageSeries>(this, tr("image").toStdString()));
    _outputConnectors.push_back(new OutputConnector<ImageSeries>(this, tr("blurred image").toStdString()));
    std::vector<std::string> choices = {"homogenous", "gaussian", "median", "bilateral"};
    _settings.push_back(new ChoiceSetting(tr("blur methods").toStdString(), choices, 0));
    _settings.push_back(new NumberSetting(tr("size").toStdString(), 1, 100, 40, true, false));
}

/**
 * Verschwimmt das Eingabebild
 */
void BlurNode::compute() {
    //Einstellungen parsen
    if(static_cast<OutputConnector<ImageSeries>* >(this->inputConnectors().front()->source())->data() == NULL) {
        throw std::runtime_error(tr("No data input.").toStdString());
    }

    int length = static_cast<OutputConnector<ImageSeries>* >(this->inputConnectors().front()->source())->data()->length();
    ChoiceSetting* method_c = static_cast<ChoiceSetting*>(_settings.front());
    std::string method = method_c->selectedString();
    NumberSetting* size_c = static_cast<NumberSetting*>(_settings.at(1));
    int size = size_c->intValue();    
    int width = static_cast<OutputConnector<ImageSeries>* >(this->inputConnectors().front()->source())->data()->width();
    int height = static_cast<OutputConnector<ImageSeries>* >(this->inputConnectors().front()->source())->data()->height();

    //Einstellungen anpassen / überprüfen
    size = size * 0.31;
    if((size % 2) == 0) size += 1;
    if(size < 0 || size > 31) throw std::invalid_argument(tr("Size has to have a value between 1 and 100").toStdString());

    //benötigte Objekte initialisieren
    ImageSeries *out = new ImageSeries(length, width, height);
    cv::Mat img, blurred;
    int s;


    for(int i = 0; i < length; i++) {
        img = static_cast<OutputConnector<ImageSeries>* >(this->inputConnectors().front()->source())->data()->itemAsCVMatrix(i);
        blurred = out->itemAsCVMatrix(i);
        s = 1;

        if(method == "homogenous") {
            do {
                blur(img, blurred, cv::Size(s, s), cv::Point(-1, -1));
                s += 2;
            } while(s < size);

        } else if(method == "gaussian") {
            do {
                GaussianBlur(img, blurred, cv::Size(s, s), 0, 0);
                s += 2;
            } while(s < size);

        } else if(method == "median") {
            do {
                medianBlur(img, blurred, s);
                s += 2;
            } while(s < size);

        } else if (method == "bilateral") {
            do {
                bilateralFilter(img, blurred, s, s * 2, s / 2);
                s += 2;
            } while(s < size);
        }
    }

    static_cast<OutputConnector<ImageSeries>*>(_outputConnectors[0])->setData(out);
}
}
