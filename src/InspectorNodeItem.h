#ifndef INSPECTORNODEITEM_H
#define INSPECTORNODEITEM_H

#include "NodeItem.h"
#include "FloatSeries.h"
#include "ImageSeries.h"
#include "InputConnector.h"
#include "InspectorInputConnectorItem.h"
#include "InspectorNode.h"
#include <QCoreApplication>

namespace taets {

class InspectorNodeItem : public NodeItem
{
    Q_DECLARE_TR_FUNCTIONS(InspectorNodeItem)

protected:
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    virtual void keyPressEvent(QKeyEvent * event);

public:
    InspectorNodeItem(InspectorNode *node, QGraphicsItem *parent = 0);

    void removeConnectorItem(InspectorInputConnectorItem *connector);

    void addFloatConnector();
    void addImageConnector();
};

} // namespace taets

#endif // INSPECTORNODEITEM_H
