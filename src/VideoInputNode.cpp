#include "VideoInputNode.h"
#include "ImageSeries.h"
#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <boost/algorithm/string/predicate.hpp>



namespace taets {

VideoInputNode::VideoInputNode(Graph *graph, unsigned int id) :
   InputNode(graph, id, tr("Video Input Node").toStdString()) {
    _outputConnectors.push_back(new OutputConnector<ImageSeries>(this, tr("read video").toStdString()));
    _settings.push_back(new NumberSetting(tr("width").toStdString(), 0, 1920, 352, true,true));
    _settings.push_back(new NumberSetting(tr("height").toStdString(), 0, 1080, 288, true,true));
    _settings.push_back(new FileSetting(tr("input video").toStdString(), "", "YUV 4:2:0 (*.yuv)"));
}

/**
 * Lese Eingabevideo (YUV 4:2:0)
 */
void VideoInputNode::compute() {
    //Einstellungen parsen
    NumberSetting* width_c = static_cast<NumberSetting*>(_settings.front());
    NumberSetting* height_c = static_cast<NumberSetting*>(_settings.at(1));
    FileSetting* file_c = static_cast<FileSetting*>(_settings.at(2));
    width = width_c->intValue();
    height = height_c->intValue();
    unsigned int frames = _graph->length();
    std::string video_name = file_c->filename();

    //Benötigte Objekte initialisieren
    ImageSeries *out = new ImageSeries(frames, width, height);
    int ret = 0;
    cv::Mat m;
    input_file = fopen(video_name.c_str(), "rb");

    //Einstellungen überprüfen
    if (!input_file) throw std::invalid_argument(tr("Couldn't open file.").toStdString());
    if(!boost::algorithm::ends_with(video_name, ".yuv")) throw std::invalid_argument(tr("Only YUV 4:2:0 is supported.").toStdString());

    for(unsigned int i = 0; i < frames; i++) {
        y = cv::Mat(cvSize(width, height), CV_8U, 1);
        cb = cv::Mat(cvSize(width, height), CV_8U, 1);
        cr = cv::Mat(cvSize(width, height), CV_8U, 1);
        cb_half = cv::Mat(cvSize(width / 2, height / 2), CV_8U, 1);
        cr_half = cv::Mat(cvSize(width / 2, height / 2), CV_8U, 1);
        ycrcb = cv::Mat(cvSize(width, height), CV_8U, 3);
        rgb = cv::Mat(cvSize(width, height), CV_8U, 3);

        if (y.empty() || cb.empty() || cr.empty() ||
            cb_half.empty() || cr_half.empty() || ycrcb.empty() || rgb.empty()) {
            YUV_cleanUp();
            throw std::runtime_error(tr("Internal failure (couldn't create matrices)").toStdString());
        }

        ret = YUV_read();
        switch(ret) {
            //everything is okay
            case 1:
                cvtColor(ycrcb, rgb, cv::COLOR_YCrCb2RGB, 3);
                m = out->itemAsCVMatrix(i);
                rgb.copyTo(m);
                break;

            //EOF
            case 2:
                if(i != frames - 1) tr("Reached end of file before the given count of frames. Check the length of the project.").toStdString();
                break;

            //I/O-Error
            case -1:
                throw std::runtime_error(tr("I/O Error. Check that you have access rights to the file.").toStdString());
                break;

            default: break;
        }

    }

    static_cast<OutputConnector<ImageSeries>*>(_outputConnectors[0])->setData(out);
    fclose(input_file);
}

/*
 * return values:
 *  1 - everything is okay
 *  2 - EOF
 * -1 - I/0 Error
 */
int VideoInputNode::YUV_read() {
    size_t bytes_read;
    unsigned int pixels = width * height;

    bytes_read = fread(y.data, sizeof(uint8_t), pixels, input_file);
    if(bytes_read == 0) return 2;
    if(bytes_read != pixels) return -1;

    bytes_read = fread(cb_half.data, sizeof(uint8_t), pixels / 4, input_file);
    if(bytes_read != pixels / 4) return -1;

    bytes_read = fread(cr_half.data, sizeof(uint8_t), pixels / 4, input_file);
    if(bytes_read != pixels / 4) return -1;

    cv::resize(cb_half, cb, cv::Size(), 2, 2, cv::INTER_CUBIC);
    cv::resize(cr_half, cr, cv::Size(), 2, 2, cv::INTER_CUBIC);
    std::vector<cv::Mat> temp = {y, cr, cb};
    merge(temp, ycrcb);

    return 1;
}

void VideoInputNode::YUV_cleanUp() {
    y.deallocate();
    cb.deallocate();
    cr.deallocate();
    cb_half.deallocate();
    cr_half.deallocate();
    ycrcb.deallocate();
    rgb.deallocate();
}
}

