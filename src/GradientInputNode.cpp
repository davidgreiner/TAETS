#include "GradientInputNode.h"
#include "Color.h"
#include "ImageSeries.h"
#include "ColorSetting.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>


namespace taets {

GradientInputNode::GradientInputNode(Graph *graph, unsigned int id) :
   InputNode(graph, id, tr("Gradient Input Node").toStdString()) {
    _outputConnectors.push_back(new OutputConnector<ImageSeries>(this, tr("generated gradient").toStdString()));
    _settings.push_back(new NumberSetting(tr("width").toStdString(), 0, 1920, 800, true,true));
    _settings.push_back(new NumberSetting(tr("height").toStdString(), 0, 1080, 600, true,true));
    _settings.push_back(new ColorSetting(tr("start color").toStdString(), Color(0, 0, 0)));
    _settings.push_back(new ColorSetting(tr("end color").toStdString(), Color(255, 255, 255)));
}
/**
 * Erzeuge einen linearen Farbverlauf
 */
void GradientInputNode::compute() {
    //Einstellungen parsen
    NumberSetting* width_c = static_cast<NumberSetting*>(_settings.front());
    NumberSetting* height_c = static_cast<NumberSetting*>(_settings.at(1));
    ColorSetting* color_c1 = static_cast<ColorSetting*>(_settings.at(2));
    ColorSetting* color_c2 = static_cast<ColorSetting*>(_settings.at(3));
    unsigned int height = height_c->intValue();
    unsigned int width = width_c->intValue();
    int r1 = color_c1->red();
    int g1 = color_c1->green();
    int b1 = color_c1->blue();
    int r2 = color_c2->red();
    int g2 = color_c2->green();
    int b2 = color_c2->blue();

    //Einstellungen überprüfen
    if(r1 < 0 || r1 > 255 || g1 < 0 || g1 > 255 ||
           b1 < 0 || b1 > 255) throw std::invalid_argument(tr("First color has wrong values.").toStdString());
    if(r2 < 0 || r2 > 255 || g2 < 0 || g2 > 255 ||
           b2 < 0 || b2 > 255) throw std::invalid_argument(tr("Second color has wrong values.").toStdString());

    //Benötigte Objekte initialisieren
    cv::Mat gradient = cv::Mat(cvSize(width, height), CV_8UC3);
    float diffRed, diffGreen, diffBlue;
    float percentFade;
    ImageSeries *out = new ImageSeries(_graph->length(), width, height);


    for(unsigned int x = 0; x < width; x++) {
        for(unsigned int y = 0; y < height; y++) {
            diffRed = r2 - r1;
            diffGreen = g2 - g1;
            diffBlue = b2 - b1;

            percentFade = (float) x / width;
            diffRed = (diffRed * percentFade) + r1;
            diffGreen = (diffGreen * percentFade) + g1;
            diffBlue = (diffBlue * percentFade) + b1;

            gradient.at<cv::Vec3b>(y,x)[0] = cv::saturate_cast<uchar>(diffBlue);
            gradient.at<cv::Vec3b>(y,x)[1] = cv::saturate_cast<uchar>(diffGreen);
            gradient.at<cv::Vec3b>(y,x)[2] = cv::saturate_cast<uchar>(diffRed);

        }

    }

    for(unsigned int i = 0; i < _graph->length(); i++) {
        cv::Mat m = out->itemAsCVMatrix(i);
        gradient.copyTo(m);
    }

    static_cast<OutputConnector<ImageSeries>*>(_outputConnectors[0])->setData(out);
}
}
