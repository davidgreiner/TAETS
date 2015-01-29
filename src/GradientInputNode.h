#ifndef GRADIENTINPUTNODE_H
#define GRADIENTINPUTNODE_H
#include <QCoreApplication>

#include "InputNode.h"
#include "ColorSetting.h"
#include "NumberSetting.h"
#include "OutputConnector.h"
#include "ImageSeries.h"
#include "Graph.h"

namespace taets {

class GradientInputNode : public InputNode {
    Q_DECLARE_TR_FUNCTIONS(GradientInputNode)

    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive &ar, const unsigned int) {
        ar & boost::serialization::base_object<InputNode>(*this);
    }

protected:
    /**
     * Konstruktor, der für die Deserialisierung benötigt wird.
     */
    GradientInputNode() {}

  public:
    GradientInputNode(Graph* graph, unsigned int id);
    /**
     * Erzeuge eine statische Farbe
     */
    virtual void compute();
};
}
#endif
