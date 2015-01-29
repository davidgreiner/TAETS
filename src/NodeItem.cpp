#include "NodeItem.h"
#include "GraphCanvas.h"
#include "InputConnectorItem.h"
#include "OutputConnectorItem.h"
#include "ConnectionItem.h"
#include "Graph.h"
#include <vector>
#include <QPainter>
#include <QKeyEvent>
#include <QString>
#include <iostream>

namespace taets {

NodeItem::NodeItem(Node *node, QGraphicsItem *parent) :
    QGraphicsItem(parent), _node(node)
{
    setup();
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsFocusable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

void NodeItem::setup() {
    for (InputConnectorBase * inCon : _node->inputConnectors()) {
        _inputConnectorItems.push_back(new InputConnectorItem(inCon, this));

    }
    for (OutputConnectorBase * outCon : _node->outputConnectors()) {
        _outputConnectorItems.push_back(new OutputConnectorItem(outCon, this));
    }
    setPos(node()->layoutX(), node()->layoutY());
    arrange();
}

void NodeItem::arrange() {
    prepareGeometryChange();
    int maxCons;
    if(_inputConnectorItems.size() > _outputConnectorItems.size())
        maxCons = _inputConnectorItems.size();
    else
        maxCons = _outputConnectorItems.size();
    body = QRect(QPoint(),
                 QSize(NODE_ITEM_WIDTH, NODE_ITEM_TITLE_HEIGHT +
                       NODE_ITEM_CONNECTOR_DISTANCE *  maxCons));
    for (int i= 0; i < _outputConnectorItems.size(); i++) {
        OutputConnectorItem * item = _outputConnectorItems[i];
        item->setPos(NODE_ITEM_WIDTH, NODE_ITEM_TITLE_HEIGHT + CONNECTOR_ITEM_RADIUS +
                     NODE_ITEM_CONNECTOR_DISTANCE * i);
    }
    for (int i = 0; i < _inputConnectorItems.size(); i++) {
        InputConnectorItem * item = _inputConnectorItems[i];
        item->setPos(0, NODE_ITEM_TITLE_HEIGHT + CONNECTOR_ITEM_RADIUS +
                     NODE_ITEM_CONNECTOR_DISTANCE * i);
    }
}

NodeItem::~NodeItem() {

    for(InputConnectorItem * inCon : _inputConnectorItems) {
        if(inCon->connection()) {
            delete inCon->connection();
        }
    }
    for(OutputConnectorItem * outCon : _outputConnectorItems) {
        for(ConnectionItem * con : outCon->connections()) {
            delete con;
        }
    }
    if(_node) _node->graph()->removeNode(_node);

    // Die ConnectorItems selbst werden durch den Destruktor der Oberklasse
    // QGraphicsItem gelöscht, da sie Kinder des Knotens sind
}

void NodeItem::addInputConnector(InputConnectorItem *item) {
    _inputConnectorItems.push_back(item);
    item->setParentItem(this);
    arrange();
}

void NodeItem::removeInputConnector(InputConnectorItem *item) {
    _inputConnectorItems.erase(std::find(_inputConnectorItems.begin(), _inputConnectorItems.end(), item));
    item->setParentItem(nullptr);
    arrange();
}

//void NodeItem::setPreviewRect(const QRect & rect) {

//}

void NodeItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event) {
    QGraphicsItem::mouseMoveEvent(event);
    for(InputConnectorItem * inCon : _inputConnectorItems) {
        if(inCon->connection()) {
            inCon->connection()->updateBoundaries();
        }
    }
    for(OutputConnectorItem * outCon : _outputConnectorItems) {
        for(ConnectionItem * con : outCon->connections()) {
            con->updateBoundaries();
        }
    }
}

QRectF NodeItem::boundingRect() const {
    qreal penWidth = 1;
    return QRectF(body.left() - penWidth / 2, body.top() - penWidth / 2,
                  body.width() + penWidth, body.height() + penWidth);
}

void NodeItem::keyPressEvent(QKeyEvent * event) {
    if (event->key() == Qt::Key_Delete) {
        delete this;
    }
}

QVariant NodeItem::itemChange(GraphicsItemChange change, const QVariant &value) {
    if(change == QGraphicsItem::ItemPositionHasChanged) {
        node()->setLayoutX(x());
        node()->setLayoutY(y());
    }
    return QGraphicsItem::itemChange(change, value);
}

void NodeItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget) {
//    foreach (QGraphicsItem * i, collidingItems()) {
//        i->update();
//    }
    (void) option;
    (void) widget;
    QString qName = QString::fromStdString(_node->name());
    QRect titleBar = QRect(0, 0, NODE_ITEM_WIDTH, NODE_ITEM_TITLE_HEIGHT);
    painter->setPen(isSelected() ? Qt::white : Qt::black);
    painter->setBrush(QBrush(Qt::gray));
    painter->drawRect(body);

    painter->setPen(Qt::red);
    painter->setBrush(QBrush(Qt::darkGray));
    painter->drawRect(titleBar);

    painter->setPen(Qt::white);
    painter->drawText(titleBar, Qt::AlignCenter | Qt::AlignVCenter, qName);
}

} // namespace taets
