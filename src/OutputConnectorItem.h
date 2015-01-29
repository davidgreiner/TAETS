#ifndef OUTPUTCONNECTORITEM_H
#define OUTPUTCONNECTORITEM_H

#include "ConnectorItem.h"
#include "OutputConnectorBase.h"
#include <vector>

namespace taets {
    class ConnectionItem;

class OutputConnectorItem : public ConnectorItem
{
    std::vector<ConnectionItem*> _connections;
protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
public:
    explicit OutputConnectorItem(OutputConnectorBase * connector, QGraphicsItem * parent = 0);
    virtual ~OutputConnectorItem();

    virtual QColor getColor();

    void addConnection(ConnectionItem * connection);
    void removeConnection(ConnectionItem * connection);

    std::vector<ConnectionItem*> connections() {
        return _connections;
    }

    /**
     * @brief connector gibt den Connector zurück, zu dem das Item gehört
     * @return
     */
    OutputConnectorBase * connector() {
        return static_cast<OutputConnectorBase*>(_connector);
    }
};

} // namespace taets

#endif // OUTPUTCONNECTORITEM_H
