#include <QPainter>
#include <QKeyEvent>
#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>
#include <iostream>
#include "InspectorNode.h"
#include "ConnectionItem.h"
#include "ConnectorItem.h"
#include "Connector.h"
#include "OutputConnectorItem.h"

namespace taets {

ConnectorItem::ConnectorItem(Connector * connector, QGraphicsItem * parent) :
    QGraphicsItem(parent), _connector(connector)
{
    setFlag(QGraphicsItem::ItemIsSelectable, true);
}

ConnectorItem::~ConnectorItem() {
}

QRectF ConnectorItem::boundingRect() const {
    qreal strokeWidth = 1;
    qreal r = CONNECTOR_ITEM_RADIUS; + strokeWidth / 2;
    return QRectF(-r, -r,
                  2 * r, 2 * r);
}

void ConnectorItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    //painter->setPen(hasFocus() ? Qt::white : Qt::black);
    painter->setBrush(getColor());
    painter->drawEllipse(QPoint(0, 0), CONNECTOR_ITEM_RADIUS, CONNECTOR_ITEM_RADIUS);
}

} //namespace taets
