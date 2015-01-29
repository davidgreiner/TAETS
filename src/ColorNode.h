#ifndef TAETS_COLORNODE_H
#define TAETS_COLORNODE_H
#include <QCoreApplication>
#include "InputNode.h"
#include "NumberSetting.h"
#include "OutputConnector.h"
#include "Graph.h"


namespace taets {

class ColorNode : public InputNode {
    Q_DECLARE_TR_FUNCTIONS(ColorNode)

    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive &ar, const unsigned int) {
        ar & boost::serialization::base_object<InputNode>(*this);
    }

protected:
    /**
     * Konstruktor, der für die Deserialisierung benötigt wird.
     */
    ColorNode() {}

  public:
    ColorNode(Graph* graph, unsigned int id);
    /**
     * Erzeuge eine statische Farbe
     */
    virtual void compute();
};
}
#endif
