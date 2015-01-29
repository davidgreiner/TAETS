#include "NoiseInputNode.h"
#include "Color.h"
#include "ImageSeries.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace taets {

NoiseInputNode::NoiseInputNode(Graph *graph, unsigned int id) :
   InputNode(graph, id, tr("Noise Input Node").toStdString()) {
    _outputConnectors.push_back(new OutputConnector<ImageSeries>(this, tr("generated noise").toStdString()));
    _settings.push_back(new NumberSetting(tr("width").toStdString(), 0, 1920, 800, true,true));
    _settings.push_back(new NumberSetting(tr("height").toStdString(), 0, 1080, 600, true,true));
    std::vector<std::string> choices = {"Gaussian", "Salt and Pepper"};
    _settings.push_back(new ChoiceSetting(tr("noise generation methods").toStdString(), choices, 0));
}

/**
 * Erzeuge eine statische Farbe
 */
void NoiseInputNode::compute() {
    //Einstellungen parsen
    NumberSetting* width_c = static_cast<NumberSetting*>(_settings.front());
    NumberSetting* height_c = static_cast<NumberSetting*>(_settings.at(1));
    ChoiceSetting* method_c = static_cast<ChoiceSetting*>(_settings.at(2));
    unsigned int width = width_c->intValue();
    unsigned int height = height_c->intValue();
    std::string method = method_c->selectedString();

    //Benötigte Objekte initialisieren
    ImageSeries *out = new ImageSeries(_graph->length(), width, height);
    cv::Mat noise;

    for(int i = 0; i < _graph->length(); i++) {
        noise = out->itemAsCVMatrix(i);
        if(method == "Gaussian") {
                randn(noise, 128, 30);

        } else if (method == "Salt and Pepper") {
            cv::Mat saltpepper_noise = cv::Mat::zeros(cv::Size(width, height),CV_8U);
            randu(saltpepper_noise,0,255);
            cv::Mat black = saltpepper_noise < 30;
            cv::Mat white = saltpepper_noise > 225;
            noise.setTo(255,white);
            noise.setTo(0,black);
        }
    }

    static_cast<OutputConnector<ImageSeries>*>(_outputConnectors[0])->setData(out);
}
}

