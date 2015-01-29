#include "TransformNode.h"

namespace taets {

TransformNode::TransformNode(Graph* graph, unsigned int id) :
    ModifierNode(graph, id, tr("Transform Node").toStdString()) {
    _inputConnectors.push_back(new InputConnector<ImageSeries>(this, tr("image").toStdString()));
    _outputConnectors.push_back(new OutputConnector<ImageSeries>(this, tr("transformed image").toStdString()));
    _settings.push_back(new NumberSetting(tr("height").toStdString(), 0, 1920, 800, true, true));
    _settings.push_back(new NumberSetting(tr("width").toStdString(), 0, 1024, 600, true, true));
    std::vector<std::string> choices = {"area", "linear", "cubic"};
    _settings.push_back(new ChoiceSetting(tr("transform methods").toStdString(), choices, 1));
}

void TransformNode::compute() {
    //Einstellungen parsen
    if(static_cast<OutputConnector<ImageSeries>* >(this->inputConnectors().front()->source())->data() == NULL) {
        throw std::runtime_error(tr("No data input.").toStdString());
    }

    int length = static_cast<OutputConnector<ImageSeries>* >(this->inputConnectors().front()->source())->data()->length();
    NumberSetting* height_c = static_cast<NumberSetting*>(_settings.front());
    int height_n = height_c->intValue();
    NumberSetting* width_c = static_cast<NumberSetting*>(_settings.at(1));
    int width_n = width_c->intValue();
    ChoiceSetting* method_c = static_cast<ChoiceSetting*>(_settings.at(2));
    std::string method = method_c->selectedString();

    //Einstellungen überprüfen
    if(width_n < 0 || height_n < 0) throw std::invalid_argument(tr("Width or height are negative.").toStdString());

    //Benötigte Objekte initialisieren
    ImageSeries *out = new ImageSeries(length, width_n, height_n);
    cv::Mat img, transformed, output;
    for(int i = 0; i < length; i++) {
        img = static_cast<OutputConnector<ImageSeries>* >(this->inputConnectors().front()->source())->data()->itemAsCVMatrix(i);
        transformed = cv::Mat::zeros(img.size(), img.type());
        output = out->itemAsCVMatrix(i);

        if(method == "area") {
            resize(img, transformed, cv::Size(height_n, width_n), 0, 0, cv::INTER_AREA);

        } else if(method == "linear") {
            resize(img, transformed, cv::Size(height_n, width_n), 0, 0, cv::INTER_LINEAR);

        } else if(method == "cubic") {
            resize(img, transformed, cv::Size(height_n, width_n), 0, 0, cv::INTER_CUBIC);
        }

        transformed.copyTo(output);
    }

    static_cast<OutputConnector<ImageSeries>*>(_outputConnectors[0])->setData(out);
}
}
