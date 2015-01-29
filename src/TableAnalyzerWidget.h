#ifndef TABLEANALYZERWIDGET_H
#define TABLEANALYZERWIDGET_H

#include <QWidget>
#include <QDialog>
#include <TableAnalyzer.h>
#include "SourcesSelectionWidget.h"
#include <map>

namespace Ui {
class TableAnalyzerWidget;
}

class TableAnalyzerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TableAnalyzerWidget(taets::AnalysisToolController *_parent, QWidget *parent = 0);
    ~TableAnalyzerWidget();

    void changeEvent(QEvent *event);

private slots:
    void addSeries(taets::FloatSeries *series, std::string name);
    void removeSeries(taets::FloatSeries *series);
    void updateSeriesValue(taets::FloatSeries *series, float value);

private:
    void buildDialog();
    void makeConnections();

    Ui::TableAnalyzerWidget *ui;

    taets::TableAnalyzer *_analyzer;

    QDialog *_selection_dialog;
    SourcesSelectionWidget *_selection_widget;
    std::map<taets::FloatSeries*, QLabel*> _output_labels;
};

#endif // TABLEANALYZERWIDGET_H
