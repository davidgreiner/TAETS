#include "PSNRNode.h"

#include "Graph.h"
#include "InputConnector.h"
#include "OutputConnector.h"
#include "ImageSeries.h"
#include "FloatSeries.h"

namespace taets {

PSNRNode::PSNRNode(Graph *graph, unsigned int id) :
    ModifierNode(graph, id, tr("PSNR Node").toStdString()) {
    _inputConnectors.push_back(new InputConnector<ImageSeries>(this, tr("image 1").toStdString()));
    _inputConnectors.push_back(new InputConnector<ImageSeries>(this, tr("image 2").toStdString()));
    _outputConnectors.push_back(new OutputConnector<FloatSeries>(this, tr("PSNR").toStdString()));
}

void PSNRNode::compute() {
    if(static_cast<OutputConnector<ImageSeries>* >(this->inputConnectors().front()->source())->data() == NULL) {
        throw std::runtime_error(tr("No data input.").toStdString());
    }

    OutputConnector<ImageSeries> *src1 = static_cast<OutputConnector<ImageSeries>*>(_inputConnectors[0]->source());
    OutputConnector<ImageSeries> *src2 = static_cast<OutputConnector<ImageSeries>*>(_inputConnectors[1]->source());

    if(src1 && src2) {
        if(src1->data()->width() != src2->data()->width() || src1->data()->height() != src2->data()->height())
            throw std::invalid_argument(tr("Resolutions do not match").toStdString());
    }

    FloatSeries *result = new FloatSeries(_graph->length());

    int width, height;

    if(src1 || src2) {
        width = src1 ? src1->data()->width() : src2->data()->width();
        height = src1 ? src1->data()->height() : src2->data()->height();
    }

    cv::Mat I, K;

    //fill matrix with default data, if the connector is not connected
    if(!src1)
        I = cv::Mat::zeros(height, width, CV_32SC3);
    if(!src2)
        K = cv::Mat::zeros(height, width, CV_32SC3);

    //calculate PSNR for each frame
    unsigned int i;
    for(i = 0; i < _graph->length(); i++) {
        /* see: https://en.wikipedia.org/wiki/Peak_signal-to-noise_ratio */

        if(src1)
            src1->data()->itemAsCVMatrix(i).convertTo(I, CV_32SC3);
        if(src2)
            src2->data()->itemAsCVMatrix(i).convertTo(K, CV_32SC3);

        cv::Mat summands;
        cv::pow(I - K, 2.0, summands);
        cv::Scalar sum = cv::sum(summands); //sum is calculated per channel, those are added in the next line
        float mse = (sum[0] + sum[1] + sum[2]) / (width * height * 3.0f);
        float psnr = 10.0f * log10((255.0f * 255.0f) / mse);

        result->setFloatItem(i, psnr);
    }

    static_cast<OutputConnector<FloatSeries>*>(_outputConnectors[0])->setData(result);
}

}
