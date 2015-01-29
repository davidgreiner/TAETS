#ifndef TAETS_MIXNODE_H
#define TAETS_MIXNODE_H
#include <QCoreApplication>
#include "ModifierNode.h"
#include "Graph.h"
#include "NumberSetting.h"
#include "ChoiceSetting.h"
#include "OutputConnector.h"
#include "InputConnector.h"
#include "ImageSeries.h"
#include "FloatSeries.h"

namespace taets {

class MixNode : public ModifierNode {
    Q_DECLARE_TR_FUNCTIONS(MixNode)

    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive &ar, const unsigned int) {
        ar & boost::serialization::base_object<ModifierNode>(*this);
    }

protected:
    /**
     * Konstruktor, der für die Deserialisierung benötigt wird.
     */
    MixNode() {}

  public:
    MixNode(Graph* graph, unsigned int id);
    /**
     * Vermische zwei Bilder miteinander
     */
    virtual void compute();

  private:
    float addMixMode(float a, float b, float factor);
    float subtractMixMode(float a, float b, float factor);
    float screenMixMode(float a, float b, float factor);
    float multiplyMixMode(float a, float b, float factor);
    float alphaMixMode(float a, float b, float factor);
    float averageMixMode(float a, float b, float factor);
};
}
#endif
