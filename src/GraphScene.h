#ifndef GRAPHSCENE_H
#define GRAPHSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QDebug>
#include "GraphCanvas.h"
#include "Graph.h"
#include "NodeItem.h"
#include "BinaryTemplate.h"

class GraphScene : public QGraphicsScene
{
    Q_OBJECT

    // Das ist nur für das Kontextmenü, das finde ich nicht gut so.
    GraphCanvas * canvas;

    taets::Graph * _graph;

    taets::NodeItem * lastSelectedNode;

protected:
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent * event);

public:
    explicit GraphScene(taets::Graph * graph = nullptr, GraphCanvas * canv = 0, QObject *parent = 0);

    virtual ~GraphScene();

    taets::NodeItem * selectedNode() {
        return lastSelectedNode;
    }

    taets::Graph * graph() { return _graph; }

    Node* addNode(taets::Node * node);
    Node* addBinaryNode(taets::BinaryTemplate * tmpl);

    template<typename T>
    Node* addNode() {
        return addNode(_graph->createNode<T>());
    }

signals:
    void selectedNodeChanged();

public slots:

private slots:
    void checkNodeSelection();

};

#endif // GRAPHSCENE_H
