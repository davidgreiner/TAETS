#ifndef TAETS_BLURNODE_H
#define TAETS_BLURNODE_H
#include <QCoreApplication>
#include "ModifierNode.h"
#include "NumberSetting.h"
#include "ChoiceSetting.h"
#include "OutputConnector.h"
#include "InputConnector.h"
#include "ImageSeries.h"
#include "Graph.h"

namespace taets {

class BlurNode : public ModifierNode {
    Q_DECLARE_TR_FUNCTIONS(BlurNode)

    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive &ar, const unsigned int) {
        ar & boost::serialization::base_object<ModifierNode>(*this);
    }

protected:
    /**
     * Konstruktor, der für die Deserialisierung benötigt wird.
     */
    BlurNode() {}

  public:
    BlurNode(Graph* graph, unsigned int id);

    /**
     * Verschwimmt das Eingabebild
     */
    virtual void compute();
};
}
#endif
