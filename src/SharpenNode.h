#ifndef TAETS_SHARPENNODE_H
#define TAETS_SHARPENNODE_H
#include <QCoreApplication>
#include "ModifierNode.h"
#include "ChoiceSetting.h"
#include "OutputConnector.h"
#include "InputConnector.h"
#include "ImageSeries.h"
#include "Graph.h"

namespace taets {

class SharpenNode : public ModifierNode {
    Q_DECLARE_TR_FUNCTIONS(SharpenNode)

    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive &ar, const unsigned int) {
        ar & boost::serialization::base_object<ModifierNode>(*this);
    }

protected:
    /**
     * Konstruktor, der für die Deserialisierung benötigt wird.
     */
    SharpenNode() {}

  public:
    SharpenNode(Graph* graph, unsigned int id);

    /**
     * Schärft das Eingabebild
     */
    virtual void compute();
};
}
#endif
