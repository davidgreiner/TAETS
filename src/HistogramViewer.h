#ifndef HISTOGRAMVIEWER_H
#define HISTOGRAMVIEWER_H

#include "Histogram.h"
#include <list>

#include <QWidget>

class HistogramViewer : public QWidget
{
    Q_OBJECT
public:
    explicit HistogramViewer(QWidget *parent = 0);

    void paintEvent(QPaintEvent *event);

signals:
    void redEnabledChanged(bool enabled);
    void greenEnabledChanged(bool enabled);
    void blueEnabledChanged(bool enabled);
    void luminanceEnabledChanged(bool enabled);
    void logarithmicChanged(bool logarithmic);

public slots:
    void setRedEnabled(bool enabled) {
        if(enabled != _red_enabled) {
            _red_enabled = enabled;
            emit redEnabledChanged(_red_enabled);

            if(enabled) {
                setLuminanceEnabled(false);
            }

            update();
        }
    }

    void setGreenEnabled(bool enabled) {
        if(enabled != _green_enabled) {
            _green_enabled = enabled;
            emit greenEnabledChanged(_green_enabled);

            if(enabled) {
                setLuminanceEnabled(false);
            }

            update();
        }
    }

    void setBlueEnabled(bool enabled) {
        if(enabled != _blue_enabled) {
            _blue_enabled = enabled;
            emit blueEnabledChanged(_blue_enabled);

            if(enabled) {
                setLuminanceEnabled(false);
            }

            update();
        }
    }

    void setLuminanceEnabled(bool enabled) {
        if(enabled != _luminance_enabled) {
            _luminance_enabled = enabled;
            emit luminanceEnabledChanged(_luminance_enabled);

            if(enabled) {
                setRedEnabled(false);
                setGreenEnabled(false);
                setBlueEnabled(false);
            }

            update();
        }
    }

    void setLogarithmic(bool logarithmic) {
        if(_logarithmic != logarithmic) {
            _logarithmic = logarithmic;
            emit logarithmicChanged(_logarithmic);
            update();
        }
    }

    /**
     * Sets the histograms to display. All Histograms must have the same resolution. The pointers must stay valid until the next call to setHistograms.
     */
    void setHistograms(std::vector<taets::Histogram*> histograms) {
        _histograms = histograms;
        update();
    }

private:
    bool _red_enabled;
    bool _green_enabled;
    bool _blue_enabled;
    bool _luminance_enabled;
    bool _logarithmic;
    std::vector<taets::Histogram*> _histograms;
};

#endif // HISTOGRAMVIEWER_H
