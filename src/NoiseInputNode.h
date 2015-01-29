#ifndef NOISEINPUTNODE_H
#define NOISEINPUTNODE_H
#include "InputNode.h"
#include "NumberSetting.h"
#include "ChoiceSetting.h"
#include "OutputConnector.h"
#include "ImageSeries.h"
#include "Graph.h"
#include <QCoreApplication>

namespace taets {

class NoiseInputNode : public InputNode {
    Q_DECLARE_TR_FUNCTIONS(NoiseInputNode)

    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive &ar, const unsigned int) {
        ar & boost::serialization::base_object<InputNode>(*this);
    }

protected:
    /**
     * Konstruktor, der für die Deserialisierung benötigt wird.
     */
    NoiseInputNode() {}

  public:
    NoiseInputNode(Graph* graph, unsigned int id);
    /**
     * Erzeugt ein Rauschen
     */
    virtual void compute();
};

}
#endif
