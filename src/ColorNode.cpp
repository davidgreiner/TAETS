#include "ColorNode.h"
#include "Color.h"
#include "ImageSeries.h"
#include "ColorSetting.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>




namespace taets {

ColorNode::ColorNode(Graph *graph, unsigned int id) :
   InputNode(graph, id, tr("Color Node").toStdString()) {
    _outputConnectors.push_back(new OutputConnector<ImageSeries>(this, tr("color").toStdString()));
    _settings.push_back(new NumberSetting(tr("width").toStdString(), 0, 1920, 800, true,true));
    _settings.push_back(new NumberSetting(tr("height").toStdString(), 0, 1080, 600, true,true));
    _settings.push_back(new ColorSetting(tr("color").toStdString(), Color(0, 0, 0)));
}

/**
 * Erzeuge eine statische Farbe
 */
void ColorNode::compute() {
    //Einstellungen parsen
    NumberSetting* width_c = static_cast<NumberSetting*>(_settings.front());
    NumberSetting* height_c = static_cast<NumberSetting*>(_settings.at(1));
    ColorSetting* color_c = static_cast<ColorSetting*>(_settings.at(2));
    unsigned int width = width_c->intValue();
    unsigned int height = height_c->intValue();
    unsigned int r = color_c->red();
    unsigned int g = color_c->green();
    unsigned int b = color_c->blue();

    //Einstellungen überprüfen
    if(r < 0 || r > 255 || g < 0 || g > 255 ||
       b < 0 || b > 255) throw std::invalid_argument(tr("Color has wrong values.").toStdString());

    //Benötigte Objekte erstellen
    cv::Mat output = cv::Mat(cvSize(width, height), CV_8UC3, cv::Scalar(r, g, b));
    ImageSeries *out = new ImageSeries(_graph->length(), width, height);


    for(int i = 0; i < _graph->length(); i++) {
        cv::Mat m = out->itemAsCVMatrix(i);
        output.copyTo(m);
    }

    static_cast<OutputConnector<ImageSeries>*>(_outputConnectors[0])->setData(out);
}
}
