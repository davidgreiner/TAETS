#include <QGraphicsSimpleTextItem>
#include <QGraphicsSceneMouseEvent>
#include <iostream>
#include <QDebug>
#include "ConnectionItem.h"
#include "InputConnectorItem.h"
#include "InputConnector.h"
#include "OutputConnectorItem.h"
#include "ImageSeries.h"
#include "FloatSeries.h"
#include "Connector.h"

namespace taets {

InputConnectorItem::InputConnectorItem(InputConnectorBase *connector, QGraphicsItem *parent) :
    ConnectorItem(connector, parent), _connection(nullptr)
{
    QGraphicsSimpleTextItem * item = new QGraphicsSimpleTextItem(QString::fromStdString(connector->name()), this);
    item->setPos(CONNECTOR_ITEM_RADIUS + 5,  0);
}

InputConnectorItem::~InputConnectorItem() {
    if(connection()) {
        qDebug() << "An InputConnector whicht was still connected was deleted.";
    }
}

void InputConnectorItem::mousePressEvent(QGraphicsSceneMouseEvent * event) {
    QGraphicsItem::mousePressEvent(event);
    if(event->isAccepted()) {
        ConnectionItem * item = new ConnectionItem(this, this);
        // scene()->addItem(item); Ist implizit in der parent-child Beziehung dabei
        // Mit diesem Aufruf erhält das neue item die weiteren mouseMove events
        // ist aber nicht ausgewählt
        // wichtig ist nur, dass beim MouseReleaseEvent das neue Item auch wieder loslässt

        item->grabMouse();
        //TODO? Cursor
    }
}

void InputConnectorItem::mouseReleaseEvent(QGraphicsSceneMouseEvent * event) {
    QGraphicsItem::mouseReleaseEvent(event);
    ungrabMouse();
}

void InputConnectorItem::setConnection(ConnectionItem * connection) {
    if(_connection) {
        connector()->disconnect();
    }
    _connection = connection;
    if(_connection) {
        connector()->connect(connection->source()->connector());
    }
}

QColor InputConnectorItem::getColor() {
    //TODO: schöner
    if(typeid(*_connector) == typeid(InputConnector<ImageSeries>)) {
        return Qt::yellow;
    } else {
        return Qt::gray;
    }
}
} //namespace taets
