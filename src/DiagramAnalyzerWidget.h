#ifndef DIAGRAMANALYZERWIDGET_H
#define DIAGRAMANALYZERWIDGET_H

#include <QWidget>
#include "DiagramAnalyzer.h"

namespace Ui {
class DiagramAnalyzerWidget;
}

class DiagramAnalyzerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DiagramAnalyzerWidget(taets::AnalysisToolController *_parent, QWidget *parent = 0);
    ~DiagramAnalyzerWidget();

    void changeEvent(QEvent *event);

private:
    void makeConnections();

    Ui::DiagramAnalyzerWidget *ui;

    taets::DiagramAnalyzer *_analyzer;
};

#endif // DIAGRAMANALYZERWIDGET_H
