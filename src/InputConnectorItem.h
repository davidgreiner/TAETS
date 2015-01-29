#ifndef INPUTCONNECTORITEM_H
#define INPUTCONNECTORITEM_H

#include "ConnectorItem.h"
#include "InputConnectorBase.h"

namespace taets {
    class ConnectionItem;

class InputConnectorItem : public ConnectorItem
{
    ConnectionItem * _connection;
protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent * event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);
public:
    explicit InputConnectorItem(InputConnectorBase * connector, QGraphicsItem * parent = 0);
    virtual ~InputConnectorItem();

    void setConnection(ConnectionItem * connection);
    ConnectionItem * connection() { return _connection; }

    virtual QColor getColor();

    /**
     * @brief connector gibt den Connector zurück, zu dem das Item gehört
     * @return
     */
    InputConnectorBase * connector() {
        return static_cast<InputConnectorBase*>(_connector);
    }
};

} //namespace taets

#endif // INPUTCONNECTORITEM_H
