#ifndef FRAMEANALYZERWIDGET_H
#define FRAMEANALYZERWIDGET_H

#include <FrameAnalyzer.h>

#include <QWidget>

namespace taets { class AnalysisToolController; }

namespace Ui {
class FrameAnalyzerWidget;
}

class FrameAnalyzerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FrameAnalyzerWidget(taets::AnalysisToolController *_controller, QWidget *parent = 0);
    ~FrameAnalyzerWidget();

    void changeEvent(QEvent *event);

private slots:
    void compareModeChanged(bool _checked);

private:
    void makeConnections();

    Ui::FrameAnalyzerWidget *ui;

    taets::FrameAnalyzer *m_analyzer;
};

#endif // FRAMEANALYZERWIDGET_H
