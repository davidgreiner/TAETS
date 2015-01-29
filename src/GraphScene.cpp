#include <QMenu>
#include <QAction>
#include "GraphScene.h"
#include "Node.h"
#include "ConnectionItem.h"
#include "InspectorNodeItem.h"
#include "TemplateManager.h"
#include <map>
#include <QDebug>

using namespace taets;

GraphScene::GraphScene(Graph * graph, GraphCanvas * canv, QObject *parent) :
    QGraphicsScene(parent), _graph(graph), lastSelectedNode(nullptr), canvas(canv)
{
    std::map<InputConnectorBase*, InputConnectorItem*> in;
    std::map<OutputConnectorBase*, OutputConnectorItem*> out;
    for(Node * node : _graph->nodes()) {
        NodeItem * item = new NodeItem(node);
        for(InputConnectorItem * inCon : item->inputConnectorItems()) {
            in[inCon->connector()] = inCon;
        }
        for(OutputConnectorItem * outCon : item->outputConnectorItems()) {
            out[outCon->connector()] = outCon;
        }
        addItem(item);
        // this does not make connections yet
    }
    for(Node * node : _graph->nodes()) {
        for(InputConnectorBase * inCon : node->inputConnectors()) {
            if(inCon->source()) {
                new ConnectionItem(out[inCon->source()], in[inCon]); //registers itself
            }
        }
    }
    InspectorNodeItem * inspector = new InspectorNodeItem(_graph->inspector());
    addItem(inspector);
    for(InputConnectorItem *inCon : inspector->inputConnectorItems()) {
        in[inCon->connector()] = inCon;
    }
    for(InputConnectorBase * inCon : _graph->inspector()->inputConnectors()) {
        if(inCon->source()) {
            new ConnectionItem(out[inCon->source()], in[inCon]); //registers itself
        }
    }

    connect(this, &GraphScene::selectionChanged, this, &GraphScene::checkNodeSelection);
}

GraphScene::~GraphScene() {
    /* Wir müssen die Items in der richtigen Reihenfolge löschen, damit nichts knallt.
     * Das Model wird dabei zwar verändert, aber sowieso nicht mehr gespeichert. */

    for(auto item : items()) {
        if(typeid(*item) == typeid(ConnectionItem)) delete item;
    }

    for(auto item : items()) {
        if(typeid(*item) == typeid(NodeItem)) delete item;
    }

    for(auto item : items()) {
        if(typeid(*item) == typeid(InspectorNodeItem)) delete item;
    }

    assert(items().count() == 0); //mehr Items sollte es nicht geben, sonst ist was falsch.

    delete _graph;
}

void GraphScene::checkNodeSelection() {
    QList<QGraphicsItem *> selected = selectedItems();
    NodeItem * item = nullptr;
    for(QGraphicsItem * gitem : selected) {
        item = dynamic_cast<NodeItem*>(gitem);
        if(item) break;
    }

    if(item != lastSelectedNode) {

        lastSelectedNode = item;
        emit selectedNodeChanged();
    }
}

Node* GraphScene::addNode(Node *node) {
    addItem(new NodeItem(node));
    return node;
}

Node* GraphScene::addBinaryNode(BinaryTemplate *tmpl) {
    return addNode(_graph->createBinaryNode(tmpl));
}

void GraphScene::contextMenuEvent(QGraphicsSceneContextMenuEvent * event) {
    QGraphicsScene::contextMenuEvent(event);
    if(!event->isAccepted()) {
        event->accept();
        QMenu menu;
        QMenu *addInputMenu = menu.addMenu(tr("New Input Node"));
        QMenu *addModificatorMenu = menu.addMenu(tr("New Modifier Node"));
        QMenu *addBinaryNode = menu.addMenu(tr("New Binary Node"));
        addInputMenu->addActions(canvas->inputNodeActions());
        addModificatorMenu->addActions(canvas->modifierNodeActions());
        TemplateManager& manager = TemplateManager::instance();
        for(BinaryTemplate * tmpl : manager.templates) {
            QAction * action = new QAction(QString::fromStdString(tmpl->name()), this);
            connect(action, &QAction::triggered, [=](bool checked) {
               this->addBinaryNode(tmpl);
            });
            addBinaryNode->addAction(action);
        }
        menu.exec(event->screenPos());
    }
}
