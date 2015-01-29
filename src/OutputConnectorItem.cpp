#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include "OutputConnectorItem.h"
#include "OutputConnector.h"
#include "InputConnectorItem.h"
#include "InspectorInputConnectorItem.h"
#include "ConnectionItem.h"
#include "ImageSeries.h"
#include "FloatSeries.h"
#include "GraphCanvas.h"

namespace taets {

OutputConnectorItem::OutputConnectorItem(OutputConnectorBase *connector, QGraphicsItem *parent) :
    ConnectorItem(connector, parent)
{
    QGraphicsSimpleTextItem * item = new QGraphicsSimpleTextItem(QString::fromStdString(connector->name()), this);
    qreal width = item->boundingRect().width();
    item->setPos(CONNECTOR_ITEM_RADIUS - 5 - width,  0);
}

OutputConnectorItem::~OutputConnectorItem() {
    if(!connections().empty())
        qDebug() << "An output connector item was deleted while it still hat active connections";
}

void OutputConnectorItem::mousePressEvent(QGraphicsSceneMouseEvent * event) {
    QGraphicsItem::mousePressEvent(event);
    if(event->isAccepted()) {
        ConnectionItem * item = new ConnectionItem(this);
        scene()->addItem(item);
        // Mit diesem Aufruf erhält das neue item die weiteren mouseMove events
        // ist aber nicht ausgewählt
        // wichtig ist nur, dass beim MouseReleaseEvent das neue Item auch wieder loslässt

        item->grabMouse();
        //TODO? Cursor
    }
}

void OutputConnectorItem::mouseReleaseEvent(QGraphicsSceneMouseEvent * event) {
    QGraphicsItem::mouseReleaseEvent(event);
    ungrabMouse();
}

void OutputConnectorItem::addConnection(ConnectionItem * connection) {
    _connections.push_back(connection);
    connector()->connect(connection->destination()->connector());
}

void OutputConnectorItem::removeConnection(ConnectionItem * connection) {
    _connections.erase(std::find(_connections.begin(), _connections.end(), connection));
    connector()->disconnect(connection->destination()->connector());
}

QColor OutputConnectorItem::getColor() {
    //TODO: schöner
    if(typeid(*_connector) == typeid(OutputConnector<ImageSeries>)) {
        return Qt::yellow;
    } else {
        return Qt::gray;
    }
}
} // namespace taets
