#ifndef CONNECTORITEM_H
#define CONNECTORITEM_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include "Connector.h"

namespace taets {

const int CONNECTOR_ITEM_RADIUS = 10;

class ConnectorItem : public QGraphicsItem
{
protected:
    Connector * _connector;

public:
    explicit ConnectorItem(Connector * connector, QGraphicsItem *parent = 0);
    virtual ~ConnectorItem();
    virtual QRectF boundingRect() const;

    virtual QColor getColor() = 0;

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    /**
     * @brief connector gibt den zu diesem Item gehörenden Connector zurück
     * @return
     */
    Connector * connector() {
        return _connector;
    }
};

} // namespace taets

#endif // CONNECTORITEM_H
