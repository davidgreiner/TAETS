#ifndef TAETS_CONSTANTNODE_H
#define TAETS_CONSTANTNODE_H
#include "InputNode.h"
#include "NumberSetting.h"
#include "FloatSeries.h"
#include "Graph.h"
#include <QCoreApplication>

namespace taets {

class ConstantNode : public InputNode {
    Q_DECLARE_TR_FUNCTIONS(ConstantNode)

    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive &ar, const unsigned int) {
        ar & boost::serialization::base_object<InputNode>(*this);
    }

protected:
    /**
     * Konstruktor, der für die Deserialisierung benötigt wird.
     */
    ConstantNode() {}

  public:
    ConstantNode(Graph* graph, unsigned int id);

    virtual void compute();
};

}
#endif
