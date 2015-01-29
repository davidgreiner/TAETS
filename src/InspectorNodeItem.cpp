#include "InspectorNodeItem.h"
#include <QMenu>
#include <QAction>
#include <QEvent>
#include <QContextMenuEvent>
#include <QGraphicsSceneContextMenuEvent>
#include <sstream>
#include <QDebug>

namespace taets {

InspectorNodeItem::InspectorNodeItem(InspectorNode * node, QGraphicsItem * parent) :
    NodeItem(node, parent) {
//    setFlag(QGraphicsItem::ItemIsFocusable, false);
    // Achtung hack
    for(QGraphicsItem * item : this->childItems()) {
        removeInputConnector(static_cast<InputConnectorItem*>(item));
        delete item;
    }
    for(InputConnectorBase * inCon : node->inputConnectors()) {
        addInputConnector(new InspectorInputConnectorItem(inCon));
    }
}

void InspectorNodeItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    event->accept();
    QMenu menu;
    QAction * imageAction = new QAction(tr("New image connector"), &menu);
    QAction * floatAction = new QAction(tr("New float connector"), &menu);
    menu.connect(floatAction, &QAction::triggered, [=](bool checked) {
        addFloatConnector();
    });
    menu.connect(imageAction, &QAction::triggered, [=](bool checked) {
        addImageConnector();
    });

    menu.addAction(imageAction);
    menu.addAction(floatAction);
    menu.exec(event->screenPos());
}

void InspectorNodeItem::addFloatConnector() {
    std::stringstream ss;
    std::string trl = tr("float").toStdString();
    ss << trl << node()->inputConnectors().size() - 1;
    InputConnectorBase * connector = new InputConnector<FloatSeries>(node(), ss.str());
    static_cast<InspectorNode*>(node())->addConnector(connector);
    addInputConnector(new InspectorInputConnectorItem(connector));
}

void InspectorNodeItem::addImageConnector() {
    std::stringstream ss;
    std::string trl = tr("video").toStdString();
    ss << trl << node()->inputConnectors().size() - 1;
    InputConnectorBase * connector = new InputConnector<ImageSeries>(node(), ss.str());
    static_cast<InspectorNode*>(node())->addConnector(connector);
    addInputConnector(new InspectorInputConnectorItem(connector));
}

void InspectorNodeItem::keyPressEvent(QKeyEvent *event) {
    QGraphicsItem::keyPressEvent(event);
}

void InspectorNodeItem::removeConnectorItem(InspectorInputConnectorItem *connector) {
    connector->remove();
    arrange();
}

} // namespace taets
