#ifndef IMAGEDATAHELPER_H
#define IMAGEDATAHELPER_H

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <QPixmap>
#include <string.h>

namespace taets {

/**
 * Statische Klasse, die Bilddaten konvertieren und neu interpretieren.
 */
class ImageDataHelper
{
public:
    static cv::Mat createCVMatrix(unsigned int width, unsigned int height, Color *pixels) {
        return cv::Mat(cv::Size(width, height), CV_8UC3, pixels);
    }

    static QPixmap createQPixmap(unsigned int width, unsigned int height, Color *pixels) {
        return QPixmap::fromImage(QImage(reinterpret_cast<uchar*>(pixels), width, height, QImage::Format_RGB888));
    }

    static QPixmap createQPixmap(cv::Mat mat) {
        return QPixmap::fromImage(QImage(mat.data, mat.cols, mat.rows, QImage::Format_RGB888));
    }

    static void YUV420ptoRGB(unsigned int width, unsigned int height, cv::Mat rgb, uchar * yuv) {
        cv::Mat y = cv::Mat(cvSize(width, height), CV_8U, 1);
        cv::Mat cb = cv::Mat(cvSize(width, height), CV_8U, 1);
        cv::Mat cr = cv::Mat(cvSize(width, height), CV_8U, 1);
        cv::Mat cb_half = cv::Mat(cvSize(width / 2, height / 2), CV_8U, 1);
        cv::Mat cr_half = cv::Mat(cvSize(width / 2, height / 2), CV_8U, 1);
        cv::Mat ycrcb = cv::Mat(cvSize(width, height), CV_8U, 3);

        unsigned int pixels = width * height;
        memcpy(y.data, yuv, pixels);
        memcpy(cb_half.data, yuv + pixels, pixels / 4);
        memcpy(cr_half.data, yuv + pixels + pixels / 4, pixels / 4);
        cv::resize(cb_half, cb, cv::Size(), 2, 2, cv::INTER_NEAREST);
        cv::resize(cr_half, cr, cv::Size(), 2, 2, cv::INTER_NEAREST);
        std::vector<cv::Mat> temp = {y, cr, cb};
        merge(temp, ycrcb);

        cvtColor(ycrcb, rgb, cv::COLOR_YCrCb2RGB);
    }

    static void RGBtoYUV420p(unsigned int width, unsigned int height, cv::Mat rgb, uchar * yuv) {
        cv::Mat y = cv::Mat(cvSize(width, height), CV_8U, 1);
        cv::Mat cb = cv::Mat(cvSize(width, height), CV_8U, 1);
        cv::Mat cr = cv::Mat(cvSize(width, height), CV_8U, 1);
        cv::Mat cb_half = cv::Mat(cvSize(width / 2, height / 2), CV_8U, 1);
        cv::Mat cr_half = cv::Mat(cvSize(width / 2, height / 2), CV_8U, 1);
        cv::Mat ycrcb = cv::Mat(cvSize(width, height), CV_8U, 3);
        cvtColor(rgb, ycrcb, CV_RGB2YCrCb);
        unsigned int pixels = width * height;

        std::vector<cv::Mat> temp = { y, cr, cb };
        split(ycrcb, temp);

        cv::resize(cb, cb_half, cv::Size(), 0.5, 0.5, cv::INTER_CUBIC);
        cv::resize(cr, cr_half, cv::Size(), 0.5, 0.5, cv::INTER_CUBIC);

        memcpy(yuv, y.data, pixels);
        memcpy(yuv + pixels, cb_half.data, pixels / 4);
        memcpy(yuv + pixels + pixels / 4, cr_half.data, pixels / 4);

    }
};
}
#endif // IMAGEDATAHELPER_H
