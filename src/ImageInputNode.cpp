#include "ImageInputNode.h"
#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "ImageSeries.h"
#include "OutputConnector.h"

namespace taets {

ImageInputNode::ImageInputNode(Graph *graph, unsigned int id) :
   InputNode(graph, id, tr("Image Input Node").toStdString()) {
    _outputConnectors.push_back(new OutputConnector<ImageSeries>(this, tr("read image").toStdString()));
    _settings.push_back(new FileSetting(tr("input image").toStdString(), "", tr("Image files (*.png, *.jpg)").toStdString()));
}


void ImageInputNode::compute() {
    //Einstellungen parsen
    FileSetting* file_c = static_cast<FileSetting*>(_settings.front());
    std::string image_name = file_c->filename();
    cv::Mat img = cv::imread(image_name, CV_LOAD_IMAGE_COLOR);
    ImageSeries *out = new ImageSeries(_graph->length(), img.size().width, img.size().height);

    //Einstellungen überprüfen
    if(img.data == NULL) throw std::runtime_error(tr("Could not read image. Supported are: .jpg, .png, .bmp, .tiff. Or not sufficient rights to access the file.").toStdString());

    //imread liest in BGR ein
    cv::cvtColor(img, img, CV_BGR2RGB);

    for(int i = 0; i < _graph->length(); i++) {
        cv::Mat m = out->itemAsCVMatrix(i);
        img.copyTo(m);
    }
    static_cast<OutputConnector<ImageSeries>*>(_outputConnectors[0])->setData(out);
}
}
