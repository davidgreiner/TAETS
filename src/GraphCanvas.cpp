#include "GraphCanvas.h"
#include "Graph.h"
#include "NodeItem.h"
#include "InspectorNodeItem.h"
#include "Connector.h"
#include "ConnectorItem.h"
#include "ConnectionItem.h"
#include "InputConnectorBase.h"
#include "OutputConnectorBase.h"
#include "Node.h"
#include "TemplateManager.h"
#include <iostream>
#include <QGraphicsItem>
#include <QContextMenuEvent>
#include <QMenu>
#include <algorithm>
#include <QDebug>

#include "GraphScene.h"
#include "BinaryNode.h"
#include "BlurNode.h"
#include "BrightnessContrastNode.h"
#include "ColorNode.h"
#include "ConstantNode.h"
#include "GradientInputNode.h"
#include "ImageInputNode.h"
#include "InputNode.h"
#include "InspectorNode.h"
#include "MixNode.h"
#include "ModifierNode.h"
#include "NoiseInputNode.h"
#include "PSNRNode.h"
#include "SharpenNode.h"
#include "TransformNode.h"
#include "VideoInputNode.h"
#include "NodeItem.h"

// Makros für das Hinzufügen von Kontext-Menü Actions
#define ADD_INPUT_NODE_ACT(type, action_text) \
    { \
        QAction *action = new QAction(action_text, this); \
        connect(action, &QAction::triggered, [=](){ _scene->addNode<type>(); }); \
        _input_nodes_act.append(action); \
    }

#define ADD_MODIFIER_NODE_ACT(type, action_text) \
    { \
        QAction *action = new QAction(action_text, this); \
        connect(action, &QAction::triggered, [=]{ _scene->addNode<type>(); }); \
        _modifier_nodes_act.append(action); \
    }

/* todo
 *
 *      Previews anzeigen
 *      fehlermeldungen beim connecten
 *      drag drop verbessern (direkte checks)?
 *
 * */

GraphCanvas::GraphCanvas(QWidget *parent) :
    QGraphicsView(parent)
{
    _scene = new GraphScene(new Graph(100), this);
    this->setScene(_scene);
    createNodeActions();
    makeConnections();
}

Graph * GraphCanvas::currentGraph() {
    return _scene->graph();
}

void GraphCanvas::createNodeActions() {
    ADD_MODIFIER_NODE_ACT(BlurNode, tr("Add Blur Node"))
    ADD_MODIFIER_NODE_ACT(BrightnessContrastNode, tr("Add Brightness / Contrast Node"))
    ADD_INPUT_NODE_ACT(ColorNode, tr("Add Color Node"))
    ADD_INPUT_NODE_ACT(ConstantNode, tr("Add Constant Node"))
    ADD_INPUT_NODE_ACT(GradientInputNode, tr("Add Gradient Node"))
    ADD_INPUT_NODE_ACT(ImageInputNode, tr("Add Image Node"))
    ADD_MODIFIER_NODE_ACT(MixNode, tr("Add Mix Node"))
    ADD_INPUT_NODE_ACT(NoiseInputNode, tr("Add Noise Node"))
    ADD_MODIFIER_NODE_ACT(PSNRNode, tr("Add PSNR Node"))
    ADD_MODIFIER_NODE_ACT(SharpenNode, tr("Add Sharpen Node"))
    ADD_MODIFIER_NODE_ACT(TransformNode, tr("Add Transform Node"))
    ADD_INPUT_NODE_ACT(VideoInputNode, tr("Add Video Node"))
}

Node * GraphCanvas::selectedNode() {
    NodeItem * sel = _scene->selectedNode();
    if(sel) return sel->node();
    return nullptr;
}

void GraphCanvas::selectedNodeItemChanged() {
    emit selectedNodeChanged();
}

void GraphCanvas::makeConnections() {
    connect(_scene, &GraphScene::selectedNodeChanged, this, &GraphCanvas::selectedNodeItemChanged);
}

void GraphCanvas::newGraph(Graph * graph) {
    setScene(nullptr);
    delete _scene;

    // Durch das letzte this wird die Szene beim löschen des Canvas mitgelöscht
    _scene = new GraphScene(graph, this);
    setScene(_scene);
    makeConnections();
}
