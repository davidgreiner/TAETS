#ifndef INSPECTORINPUTCONNECTORITEM_H
#define INSPECTORINPUTCONNECTORITEM_H

#include "InputConnectorItem.h"

namespace taets {

class InspectorInputConnectorItem : public InputConnectorItem
{
protected:
    void keyPressEvent(QKeyEvent *event);

public:
    InspectorInputConnectorItem(InputConnectorBase *connector, QGraphicsItem *parent = 0);

    virtual ~InspectorInputConnectorItem() {}

    void remove();
};

} // namespace taets

#endif // INSPECTORINPUTCONNECTORITEM_H
