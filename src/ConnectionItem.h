#ifndef CONNECTIONITEM_H
#define CONNECTIONITEM_H

#include <QKeyEvent>
#include <vector>
#include <QGraphicsItem>
#include <QPainterPath>
#include "InputConnectorItem.h"
#include "OutputConnectorItem.h"

namespace taets {

class ConnectionItem : public QGraphicsItem
{

    InputConnectorItem * _destination;
    OutputConnectorItem * _source;

    /*
     * Wenn eine Connection gezogen wird, beginnt man entweder bei
     * der Quelle oder dem Ziel an, zu ziehen. Diese Seite ist dann bekannt
     * Wenn die Maus auf einem Zugelassenen Ziel losgelassen wird und eine
     * connection erlaubt ist, wird die Connection finalized, also ins Modell
     * übernommen.
     */
    enum ConnectionState {
        SOURCE_KNOWN,
        DESTINATION_KNOWN,
        FINALIZED
    };

    // Der nicht an einem Knoten hängende Endpunkt der Linie, in Szenenkoordinaten
    QPointF tempDest;

    ConnectionState state;

    QPainterPath createPath() const;

protected:
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);
public:
    explicit ConnectionItem(OutputConnectorItem * source, InputConnectorItem * destination, QGraphicsItem *parent = 0);
    explicit ConnectionItem(OutputConnectorItem * source, QGraphicsItem *parent = 0);
    explicit ConnectionItem(InputConnectorItem * destination, QGraphicsItem *parent = 0);

    virtual ~ConnectionItem();

    void updateBoundaries() { prepareGeometryChange(); }

    void keyPressEvent(QKeyEvent *event);

    virtual QPainterPath shape() const;

    QRectF boundingRect() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    /**
      * Gibt die Source zurück
      */
    OutputConnectorItem * source() const {
        return _source;
    }

    /**
      * Gibt die destination zurück
      */
    InputConnectorItem * destination() const {
        return _destination;
    }
};

} //namespace taets

#endif // CONNECTIONITEM_H
