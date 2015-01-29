#include "InspectorInputConnectorItem.h"
#include "InspectorNodeItem.h"
#include "ConnectionItem.h"
#include <QKeyEvent>

namespace taets {

InspectorInputConnectorItem::InspectorInputConnectorItem(InputConnectorBase *connector, QGraphicsItem *parent) :
    InputConnectorItem(connector, parent)
{
    setFlag(QGraphicsItem::ItemIsFocusable, true);
}

void InspectorInputConnectorItem::keyPressEvent(QKeyEvent * event) {
    if (event->key() == Qt::Key_Delete) {
        remove();
    }
}

void InspectorInputConnectorItem::remove() {
    if(connection()) {
        delete connection();
    }
    static_cast<NodeItem*>(parentItem())->removeInputConnector(this);

    scene()->removeItem(this);
    static_cast<InspectorNode*>(connector()->node())->removeConnector(connector());
    delete connector();
    delete this;

    // Irgendwie finde ich diese Methode sehr traurig.
    // Nein, ich weine nicht. Ich schneide Zwiebeln.
}

} // namespace taets
