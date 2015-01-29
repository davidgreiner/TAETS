#include "MixNode.h"

namespace taets {

MixNode::MixNode(Graph* graph, unsigned int id) :
    ModifierNode(graph, id, tr("Mix Node").toStdString()) {
    _inputConnectors.push_back(new InputConnector<ImageSeries>(this, tr("image 1").toStdString()));
    _inputConnectors.push_back(new InputConnector<ImageSeries>(this, tr("image 2").toStdString()));
    _outputConnectors.push_back(new OutputConnector<ImageSeries>(this, tr("mixed image").toStdString()));
    std::vector<std::string> choices = {"add", "substract", "screen", "multiply", "alpha", "average"};
    _settings.push_back(new ChoiceSetting(tr("mix methods").toStdString(), choices, 0, true));
    _settings.push_back(new NumberSetting(tr("factor").toStdString(), 0, 100, 50, false, false));
}

void MixNode::compute() {
    //Einstellungen parsen
    if(static_cast<OutputConnector<ImageSeries>* >(this->inputConnectors().front()->source())->data() == NULL) {
        throw std::runtime_error(tr("No data input.").toStdString());
    }

    int length = static_cast<OutputConnector<ImageSeries>* >(this->inputConnectors().front()->source())->data()->length();
    ChoiceSetting* method_c = static_cast<ChoiceSetting*>(_settings.front());
    std::string method = method_c->selectedString();
    NumberSetting* factor_c = static_cast<NumberSetting*>(_settings.at(1));
    float factor = factor_c->floatValue() / 100;
    int width = static_cast<OutputConnector<ImageSeries>* >(this->inputConnectors().front()->source())->data()->width();
    int height = static_cast<OutputConnector<ImageSeries>* >(this->inputConnectors().front()->source())->data()->height();



    //Benötigte Objekte initialisieren
    ImageSeries *out = new ImageSeries(length, width, height);
    cv::Mat img1, img2, mixed;

    //Einstellungen überprüfen
    if(width < 0 || height < 0) throw std::invalid_argument(tr("Width or height are negative.").toStdString());
    if(factor < 0 || factor > 1) throw std::invalid_argument(tr("Factor has to have a value between 0 and 100").toStdString());

    img1 = static_cast<OutputConnector<ImageSeries>* >(this->inputConnectors().front()->source())->data()->itemAsCVMatrix(0);
    img2 = static_cast<OutputConnector<ImageSeries>* >(this->inputConnectors()[1]->source())->data()->itemAsCVMatrix(0);
    if(img1.cols != img2.cols || img1.rows != img2.rows) throw std::runtime_error(tr("The dimension of the images do not match.").toStdString());


    for(int i = 0; i < length; i++) {
        img1 = static_cast<OutputConnector<ImageSeries>* >(this->inputConnectors().front()->source())->data()->itemAsCVMatrix(i);
        img2 = static_cast<OutputConnector<ImageSeries>* >(this->inputConnectors()[1]->source())->data()->itemAsCVMatrix(i);
        mixed =  out->itemAsCVMatrix(i);

        if(method == "add") {
            for(int y = 0; y < img1.rows; y++) {
                for(int x = 0; x < img1.cols; x++) {
                    for(int c = 0; c < 3; c++) {
                        mixed.at<cv::Vec3b>(y,x)[c] = cv::saturate_cast<uchar>(addMixMode(img1.at<cv::Vec3b>(y,x)[c], img2.at<cv::Vec3b>(y,x)[c], factor));
                    }
                }
            }

        } else if(method == "substract") {
            for(int y = 0; y < img1.rows; y++) {
                for(int x = 0; x < img1.cols; x++) {
                    for(int c = 0; c < 3; c++) {
                        mixed.at<cv::Vec3b>(y,x)[c] = cv::saturate_cast<uchar>(subtractMixMode(img1.at<cv::Vec3b>(y,x)[c], img2.at<cv::Vec3b>(y,x)[c], factor));
                    }
                }
            }

        } else if(method == "screen") {
            for(int y = 0; y < img1.rows; y++) {
                for(int x = 0; x < img1.cols; x++) {
                    for(int c = 0; c < 3; c++) {
                        mixed.at<cv::Vec3b>(y,x)[c] = cv::saturate_cast<uchar>(screenMixMode(img1.at<cv::Vec3b>(y,x)[c], img2.at<cv::Vec3b>(y,x)[c], factor));
                    }
                }
            }

        } else if(method == "multiply") {
            for(int y = 0; y < img1.rows; y++) {
                for(int x = 0; x < img1.cols; x++) {
                    for(int c = 0; c < 3; c++) {
                        mixed.at<cv::Vec3b>(y,x)[c] = cv::saturate_cast<uchar>(multiplyMixMode(img1.at<cv::Vec3b>(y,x)[c], img2.at<cv::Vec3b>(y,x)[c], factor));
                    }
                }
            }

        } else if(method == "alpha") {
            for(int y = 0; y < img1.rows; y++) {
                for(int x = 0; x < img1.cols; x++) {
                    for(int c = 0; c < 3; c++) {
                        mixed.at<cv::Vec3b>(y,x)[c] = cv::saturate_cast<uchar>(alphaMixMode(img1.at<cv::Vec3b>(y,x)[c], img2.at<cv::Vec3b>(y,x)[c], factor));
                    }
                }
            }

        } else if(method == "average") {
            for(int y = 0; y < img1.rows; y++) {
                for(int x = 0; x < img1.cols; x++) {
                    for(int c = 0; c < 3; c++) {
                        mixed.at<cv::Vec3b>(y,x)[c] = cv::saturate_cast<uchar>(averageMixMode(img1.at<cv::Vec3b>(y,x)[c], img2.at<cv::Vec3b>(y,x)[c], factor));
                    }
                }
            }
        }

    }

    static_cast<OutputConnector<ImageSeries>*>(_outputConnectors[0])->setData(out);
}

float MixNode::addMixMode(float a, float b, float factor) {
    return factor * (a + b);
}

float MixNode::subtractMixMode(float a, float b, float factor) {
    return a + b < 255 ? 0 : factor * (a + b - 255);
}

float MixNode::screenMixMode(float a, float b, float factor) {
    return factor * (255 - (((255 - a) * (255 - b)) / 256));
}

float MixNode::multiplyMixMode(float a, float b, float factor) {
    return factor * ((a * b) / 255);
}

float MixNode::alphaMixMode(float a, float b, float factor) {
    return (1 - factor) * a + factor * b;
}

float MixNode::averageMixMode(float a, float b, float factor) {
    return factor*((a + b) / 2);
}

}

