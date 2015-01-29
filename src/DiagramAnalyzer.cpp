#include "DiagramAnalyzer.h"

#include <iostream>

namespace taets {

#define DEFAULT_COLOR Qt::white

DiagramAnalyzer::DiagramAnalyzer(AnalysisToolController *_parent, QObject *parent) :
    Analyzer(_parent, parent) {
    //_unused_colors.push_back(QColor(255, 0, 0)); //used for frame indicator
    _unused_colors.push_back(QColor(0, 255, 0));
    _unused_colors.push_back(QColor(0, 0, 255));
    _unused_colors.push_back(QColor(255, 255, 0));
    _unused_colors.push_back(QColor(0, 255, 255));
    _unused_colors.push_back(QColor(0, 255, 255));
}

void DiagramAnalyzer::onPreviewFrameChanged(unsigned int frame) {
    emit frameChanged(frame);
}

void DiagramAnalyzer::setDataSeriesEnabled(DataSeries *_series, bool enabled) {
    if(typeid(*_series) != typeid(FloatSeries))
        throw std::invalid_argument("DiagramAnalyzer only supports FloatSeries");

    FloatSeries *series = static_cast<FloatSeries*>(_series);

    if(enabled) {
        if(std::count(_enabled_series.begin(), _enabled_series.end(), series) != 0)
            throw std::logic_error("data not consistent");

        QColor c;
        if(_unused_colors.size() > 0) {
            c = _unused_colors.front();
            _unused_colors.pop_front();
            _used_colors.push_back(c);
        } else {
            c = DEFAULT_COLOR;
        }

        emit seriesAdded(series, c);

        SeriesEntry e;
        e.series = series;
        e.color = c;
        _enabled_series.push_back(e);
    } else {
        if(std::count(_enabled_series.begin(), _enabled_series.end(), series) == 0)
            throw std::logic_error("data not consistent");

        std::vector<SeriesEntry>::iterator element = std::find(_enabled_series.begin(), _enabled_series.end(), series);

        if(element->color != DEFAULT_COLOR) {
            _used_colors.remove(element->color);
            _unused_colors.push_front(element->color);
        }

        emit seriesRemoved(element->series);

        _enabled_series.erase(element);
    }
}

} //end namespace taets
