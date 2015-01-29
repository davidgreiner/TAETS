#include "BrightnessContrastNode.h"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

namespace taets {
BrightnessContrastNode::BrightnessContrastNode(Graph* graph, unsigned int id) :
    ModifierNode(graph, id, tr("Brightness/Contrast Node").toStdString()) {
    _inputConnectors.push_back(new InputConnector<ImageSeries>(this, tr("image").toStdString()));
    _outputConnectors.push_back(new OutputConnector<ImageSeries>(this, tr("modified image").toStdString()));
    _settings.push_back(new NumberSetting(tr("brightness").toStdString(), 0, 100, 50, true, false));
    _settings.push_back(new NumberSetting(tr("contrast").toStdString(), 0, 100, 50, false, false));
}
/**
 * Verändert die Helligkeit / Kontrast des Eingabebilds
 */
void BrightnessContrastNode::compute() {
    //Einstellungen parsen
    if(static_cast<OutputConnector<ImageSeries>* >(this->inputConnectors().front()->source())->data() == NULL) {
        throw std::runtime_error(tr("No data input.").toStdString());
    }

    int length = static_cast<OutputConnector<ImageSeries>* >(this->inputConnectors().front()->source())->data()->length();
    int width = static_cast<OutputConnector<ImageSeries>* >(this->inputConnectors().front()->source())->data()->width();
    int height = static_cast<OutputConnector<ImageSeries>* >(this->inputConnectors().front()->source())->data()->height();
    NumberSetting* brightness_c = static_cast<NumberSetting*>(_settings.front());
    NumberSetting* contrast_c = static_cast<NumberSetting*>(_settings.at(1));

    //Benötigte Werte initialisieren
    float alpha = contrast_c->floatValue();
    alpha = (alpha / 50) + 1;
    int beta = brightness_c->intValue();
    cv::Mat img, modified;
    ImageSeries *out = new ImageSeries(length, width, height);

    //Einstellungen überprüfen
    if(alpha < 1 || alpha > 3) throw std::invalid_argument(tr("Contrast has to have a value between 0 and 100").toStdString());
    if(beta < 0 || beta > 100) throw std::invalid_argument(tr("Brightness has to have a value between 0 and 100").toStdString());


    for(int i = 0; i < length; i++) {
        img = static_cast<OutputConnector<ImageSeries>* >(this->inputConnectors().front()->source())->data()->itemAsCVMatrix(i);
        modified = out->itemAsCVMatrix(i);
        img.convertTo(modified, -1, alpha, beta);
    }

    static_cast<OutputConnector<ImageSeries>*>(_outputConnectors[0])->setData(out);
}
}
