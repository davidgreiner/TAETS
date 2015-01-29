#ifndef NODEITEM_H
#define NODEITEM_H

#include "InputConnectorItem.h"
#include "OutputConnectorItem.h"
#include "Node.h"
#include <QGraphicsItem>
#include <QKeyEvent>

namespace taets {

const int NODE_ITEM_WIDTH = 175;
const int NODE_ITEM_TITLE_HEIGHT = 25;
const int NODE_ITEM_CONNECTOR_SPACING = 5;
const int NODE_ITEM_CONNECTOR_DISTANCE =
        2 * NODE_ITEM_CONNECTOR_SPACING + 2 * CONNECTOR_ITEM_RADIUS;

class NodeItem : public QGraphicsItem
{

    //TODO: Preview
    //QRect _preview_rect;

//    *
//     * @brief setupConnectorItems erstellt zu den Connectors des Knoten die passenden ConnectorItems an
//     * den passenden Positionen.
//     * @param canvas
//     * @param node

    //void setupConnectorItems(GraphCanvas *canvas, Node *node);

    /**
     * Baut die ConnectorItems auf und setzt die Größe des Knotens
     */
    Node * _node;
    std::vector<InputConnectorItem *> _inputConnectorItems;
    std::vector<OutputConnectorItem *> _outputConnectorItems;
    QRect body;
    void setup();

protected:
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
    virtual void keyPressEvent(QKeyEvent * event);
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void arrange();

public:
    void addInputConnector(InputConnectorItem * item);
    void removeInputConnector(InputConnectorItem * item);
    explicit NodeItem(Node * node, QGraphicsItem *parent = 0);
    virtual ~NodeItem();

    //void setPreviewRect(const QRect & rect);
    virtual QRectF boundingRect() const;
    virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);

    /**
     * gibt alle InputConnectorItems zurück
     */
    const std::vector<InputConnectorItem *> inputConnectorItems() {
        return _inputConnectorItems;
    }

    /**
     * gibt alle OutputConnectorItems zurück
     */
    const std::vector<OutputConnectorItem *> outputConnectorItems() {
        return _outputConnectorItems;
    }

    /**
     * @brief node gibt den Knoten zurück, zu dem dieses Item gehört
     * @return
     */
    Node * node() {
        return _node;
    }
};

} // namespace taets

#endif // NODEITEM_H
