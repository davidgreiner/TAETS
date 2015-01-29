#ifndef GRAPHCANVAS_H
#define GRAPHCANVAS_H

#include <QGraphicsView>
#include "InspectorNodeItem.h"

using namespace taets;
class GraphScene;

class GraphCanvas : public QGraphicsView
{
    Q_OBJECT

    QList<QAction*> _input_nodes_act;
    QList<QAction*> _modifier_nodes_act;

    GraphScene * _scene;

    /**
     * @brief createNodeActions erstellt das Kontextmenu fürs Hinzufügen neuer Knoten
     */
    void createNodeActions();

    /**
     * @brief makeConnections verbindet Signale und Slots
     */
    void makeConnections();

public:
    explicit GraphCanvas(QWidget * parent = 0);

    QList<QAction*> inputNodeActions() { return _input_nodes_act; }
    QList<QAction*> modifierNodeActions() { return _modifier_nodes_act; }

    Node * selectedNode();

    void newGraph(Graph * graph);

    Graph * currentGraph();

signals:
    void selectedNodeChanged();
public slots:

private slots:
    void selectedNodeItemChanged();
};

#endif // GRAPHCANVAS_H
