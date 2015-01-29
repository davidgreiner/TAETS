#include "TableAnalyzer.h"

namespace taets {

TableAnalyzer::TableAnalyzer(AnalysisToolController *_parent, QObject *parent) :
    Analyzer(_parent, parent) {
    _frame = 0;
}

void TableAnalyzer::onPreviewFrameChanged(unsigned int frame) {
    if(_frame == frame) return;

    _frame = frame;

    std::list<FloatSeries*>::iterator it;
    for(it = _enabled_series.begin(); it != _enabled_series.end(); it++) {
        emit seriesValueChanged(*it, (*it)->itemAsFloat(frame));
    }
}

void TableAnalyzer::setDataSeriesEnabled(DataSeries *_series, bool enabled) {
    if(typeid(*_series) != typeid(FloatSeries))
        throw std::invalid_argument("TableAnalyzer only supports FloatSeries");

    FloatSeries *series = static_cast<FloatSeries*>(_series);

    if(enabled) {
        if(std::count(_enabled_series.begin(), _enabled_series.end(), series) != 0)
            throw std::logic_error("data not consistent");

        _enabled_series.push_back(series);
        emit seriesAdded(series, parent()->testData()->dataSeriesName(series));
        emit seriesValueChanged(series, series->itemAsFloat(_frame));
    } else {
        if(std::count(_enabled_series.begin(), _enabled_series.end(), series) != 1)
            throw std::logic_error("data not consistent");

        _enabled_series.remove(series);
        emit seriesDeleted(series);
    }
}

} //end namespace taets
