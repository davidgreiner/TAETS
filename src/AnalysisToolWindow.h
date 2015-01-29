#ifndef ANALYSISTOOLWINDOW_H
#define ANALYSISTOOLWINDOW_H

#include <QMainWindow>

#include "AnalysisToolController.h"

namespace Ui {
class AnalysisToolWindow;
}

class AnalysisToolWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AnalysisToolWindow(const std::string & testdata_filename, QWidget *parent = 0);
    ~AnalysisToolWindow();

    void changeEvent(QEvent *event);
    void resizeEvent(QResizeEvent *event);

public slots:
    void createFrameAnalyzer();
    void createHistogramAnalyzer();
    void createDiagramAnalyzer();
    void createTableAnalyzer();
    void setStandardLayout();

private:
    void makeConnections();

    Ui::AnalysisToolWindow *ui;

    /* Später kann der GraphEditorKontroller direkt die TestData übergeben, damit kein Laden von der Platte nötig ist.
     * Da davon der AnalysisToolController nix weiß, soll das Fenster die Daten bei Bedarf freigeben. */
    taets::TestData *m_test_data;

    taets::AnalysisToolController *m_controller;

    /* Da beim erstellen eines maximierten Fensters mehrere resize-Events auftreten, müssen wir das richtige
     * herausfiltern, um das initiale Standartlayout aufzubauen */
    int m_initialization_stage;
};

#endif // ANALYSISTOOLWINDOW_H
