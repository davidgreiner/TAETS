#ifndef GRAPHEDITORWINDOW_H
#define GRAPHEDITORWINDOW_H

#include <QMainWindow>
#include <QEvent>
#include <QString>
#include "GraphCanvas.h"

namespace Ui {
class GraphEditorWindow;
}

class GraphEditorWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GraphEditorWindow(QWidget *parent = 0);
    ~GraphEditorWindow();

    void changeEvent(QEvent *event);

signals:
    void calculationFinished();

public slots:
    void newProject();
    void loadProject(const QString& filename);
    void saveProject(const QString& filename);
    void queryOutputFile();
    void compute();
    void selectedNodeChanged();

private:
    void makeConnections();
    void queryGraphLength();

    Ui::GraphEditorWindow *ui;

    GraphCanvas * graphCanvas;
};

#endif // GRAPHEDITORWINDOW_H
