#include "HistogramViewer.h"

#include <QPainter>

HistogramViewer::HistogramViewer(QWidget *parent) :
    QWidget(parent) {
    _red_enabled = _blue_enabled = _green_enabled = false;
    _luminance_enabled = true;
    _logarithmic = false;
}

void HistogramViewer::paintEvent(QPaintEvent *event) {
    (void)event;

    QPainter painter(this);

    painter.fillRect(rect(), Qt::black);

    //nothing to draw
    if(_histograms.size() == 0) return;

    painter.setCompositionMode(QPainter::CompositionMode_Plus);
    painter.setOpacity(1.0 / (float)_histograms.size());

    float max_value = 0;
    unsigned int i;
    for(i = 0; i < _histograms.size(); i++) {
        if(_red_enabled && _histograms[i]->maxRedValue() > max_value) max_value = _histograms[i]->maxRedValue();
        if(_green_enabled && _histograms[i]->maxGreenValue() > max_value) max_value = _histograms[i]->maxGreenValue();
        if(_blue_enabled && _histograms[i]->maxBlueValue() > max_value) max_value = _histograms[i]->maxBlueValue();
        if(_luminance_enabled && _histograms[i]->maxLuminanceValue() > max_value) max_value = _histograms[i]->maxLuminanceValue();
    }
    if(_logarithmic) max_value = log(max_value);
    painter.scale(width() / 256.0, height() / max_value);

    unsigned int j;
    for(i = 0; i < 256; i++) {
        for(j = 0; j < _histograms.size(); j++) {
            if(_red_enabled) {
                float h = _histograms[j]->redValue(i);
                 if(_logarithmic) h = log(h);
                painter.fillRect(QRectF(i, max_value - h, 1, h), Qt::red);
            }
            if(_green_enabled) {
                float h = _histograms[j]->greenValue(i);
                if(_logarithmic) h = log(h);
                painter.fillRect(QRectF(i, max_value - h, 1, h), Qt::green);
            }
            if(_blue_enabled) {
                float h = _histograms[j]->blueValue(i);
                if(_logarithmic) h = log(h);
                painter.fillRect(QRectF(i, max_value - h, 1, h), Qt::blue);
            }
            if(_luminance_enabled) {
                float h = _histograms[j]->luminanceValue(i);
                if(_logarithmic) h = log(h);
                painter.fillRect(QRectF(i, max_value - h, 1, h), Qt::white);
            }
        }
    }
}
