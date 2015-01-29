#include "ConnectionItem.h"
#include "OutputConnectorItem.h"
#include "InputConnectorItem.h"
#include "QGraphicsSceneMouseEvent"
#include "GraphCanvas.h"
#include <algorithm>
#include <QPainter>

namespace taets {

ConnectionItem::ConnectionItem(OutputConnectorItem * source, InputConnectorItem * destination, QGraphicsItem *parent) :
    QGraphicsItem(parent), _source(source), _destination(destination)
{
    state = ConnectionState::FINALIZED;
    setFlag(ItemIsSelectable, true);
    setFlag(ItemIsFocusable, true);
    _source->addConnection(this);
    _destination->setConnection(this);
    if(parentItem() != _destination)
        setParentItem(_destination);
}

ConnectionItem::ConnectionItem(OutputConnectorItem *source, QGraphicsItem *parent) :
    QGraphicsItem(parent), _source(source), _destination(nullptr) {
    state = SOURCE_KNOWN;
    tempDest = _source->scenePos();
    setPos(tempDest);
    setFlag(ItemIsSelectable, true);
    setFlag(ItemIsFocusable, true);
}

ConnectionItem::ConnectionItem(InputConnectorItem *destination, QGraphicsItem *parent) :
    QGraphicsItem(parent), _source(nullptr), _destination(destination) {
    state = DESTINATION_KNOWN;
    if(parentItem() != _destination) {
        setParentItem(_destination);
    }
    tempDest = QPointF(_destination->scenePos());
    setFlag(ItemIsSelectable, true);
    setFlag(ItemIsFocusable, true);
}

ConnectionItem::~ConnectionItem() {
    // setParentItem(nullptr)?
    if(state == FINALIZED) {
        _destination->setConnection(nullptr);
        _source->removeConnection(this);
    }
}

void ConnectionItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsItem::mouseMoveEvent(event);
    if(state == FINALIZED) {
        return;
    }
    prepareGeometryChange();
    tempDest = event->scenePos();
    QList<QGraphicsItem *> items = scene()->items(tempDest, Qt::IntersectsItemBoundingRect);
    if(!items.empty()) {
        if(state == SOURCE_KNOWN) {
            InputConnectorItem * inCon = nullptr;
            for(QGraphicsItem * item : items) {
                inCon = dynamic_cast<InputConnectorItem*>(item);
                if(inCon) break;
            }
            if(inCon) {
                if(inCon->connector()->isConnectionPossible(_source->connector())) {
                    _destination = inCon;
                    setPos(_destination->scenePos());
                    return;
                } else {
                    setPos(tempDest);
                    _destination = nullptr;
                    return;
                }
            }
        } else {
            OutputConnectorItem * outCon = nullptr;
            for(QGraphicsItem * item : items) {
                outCon = dynamic_cast<OutputConnectorItem*>(item);
                if(outCon) break;
            }
            if(outCon) {
                if(_destination->connector()->isConnectionPossible(outCon->connector())) {
                    _source = outCon;
                    return;
                } else {
                    _source = nullptr;
                    return;
                }
            }
        }
    }
    if(state == SOURCE_KNOWN) {
        setPos(tempDest);
        _destination = nullptr;
    } else {
        _source = nullptr;
    }
}

void ConnectionItem::mouseReleaseEvent(QGraphicsSceneMouseEvent * event) {
    QGraphicsItem::mouseReleaseEvent(event);
    if(state == FINALIZED) return;
    ungrabMouse();
    if(source() && destination()) {
        state = FINALIZED;
        _source->addConnection(this);
        if(_destination->connection())
            delete _destination->connection();
        _destination->setConnection(this);
        setParentItem(_destination);
        setPos(QPointF());
    } else {
        delete this;
    }
}

void ConnectionItem::keyPressEvent(QKeyEvent * event) {
    if (state == FINALIZED && event->key() == Qt::Key_Delete) {
        delete this;
    }
}

void ConnectionItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    painter->setPen(QPen(isSelected() ? Qt::red : Qt::black, 2));
    painter->drawPath(createPath());
}

QPainterPath ConnectionItem::createPath() const {
    QPointF end;
    if(state == FINALIZED || state == SOURCE_KNOWN || _source ) {
        end = mapFromScene(_source->scenePos());
    } else {
        end = mapFromScene(tempDest);
    }
    QPointF p1(end.rx() / 2, 0);
    QPointF p2(end.rx() / 2, end.ry());
    QPainterPath path;
    path.cubicTo(p1, p2, end);
    return path;
}

QPainterPath ConnectionItem::shape() const {
    return createPath();
}

QRectF ConnectionItem::boundingRect() const {
    qreal lineWidth = 2;
    QRectF rect = createPath().boundingRect();
    rect.setWidth(rect.width() + lineWidth);
    rect.setHeight(rect.height() + lineWidth);
    rect.moveLeft(rect.left() - lineWidth / 2);
    rect.moveTop(rect.top() - lineWidth / 2);
    return rect;
}

} //namespace taets
