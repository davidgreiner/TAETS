#include "HistogramAnalyzer.h"

namespace taets {

HistogramAnalyzer::HistogramAnalyzer(AnalysisToolController * _parent, QObject *parent) :
    Analyzer(_parent, parent) {
    _frame = _parent->frame();
}

HistogramAnalyzer::~HistogramAnalyzer() {
    std::map<ImageSeries*, Histogram*>::iterator it;
    for(it = _enabled_series.begin(); it != _enabled_series.end(); it++) {
        Histogram *h = it->second;
        delete h;
    }
}

void HistogramAnalyzer::onPreviewFrameChanged(unsigned int frame) {
    _frame = frame;
    recalculateHistograms(true);
}

void HistogramAnalyzer::setDataSeriesEnabled(DataSeries *_series, bool enabled) {
    if(typeid(*_series) != typeid(ImageSeries))
        throw std::invalid_argument("HistogramAnalyzer only supports ImageSeries");

    ImageSeries *series = static_cast<ImageSeries*>(_series);

    if(enabled) {
        if(_enabled_series.count(series) != 0)
            throw std::logic_error("data not consistent");

        _enabled_series.insert(std::pair<ImageSeries*, Histogram*>(series, new Histogram));
    } else {
        if(_enabled_series.count(series) != 1)
            throw std::logic_error("data not consistent");

        Histogram *h = _enabled_series[series];
        delete h;
        _enabled_series.erase(series);
    }

    recalculateHistograms(true);
}

void HistogramAnalyzer::recalculateHistograms(bool frame_changed) {
    std::vector<Histogram*> histograms;

    std::map<ImageSeries*, Histogram*>::iterator it;
    for(it = _enabled_series.begin(); it != _enabled_series.end(); it++) {
        ImageSeries *s = it->first;
        Histogram *h = it->second;

        if(frame_changed) {
            h->setImage(s->item(_frame));
        }

        histograms.push_back(h);
    }

    emit histogramsChanged(histograms);
}

} //end namespace taets
