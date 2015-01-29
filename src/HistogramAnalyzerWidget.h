#ifndef HISTOGRAMANALYZERWIDGET_H
#define HISTOGRAMANALYZERWIDGET_H

#include <QWidget>

#include "HistogramAnalyzer.h"

namespace Ui {
class HistogramAnalyzerWidget;
}

class HistogramAnalyzerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HistogramAnalyzerWidget(taets::AnalysisToolController * _controller, QWidget *parent = 0);
    ~HistogramAnalyzerWidget();

    void changeEvent(QEvent *event);

private:
    void makeConnections();

    Ui::HistogramAnalyzerWidget *ui;

    taets::HistogramAnalyzer *_analyzer;
};

#endif // HISTOGRAMANALYZERWIDGET_H
