#ifndef TAETS_TRANSFORMNODE_H
#define TAETS_TRANSFORMNODE_H
#include <QCoreApplication>
#include "ModifierNode.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include "NumberSetting.h"
#include "ChoiceSetting.h"
#include "OutputConnector.h"
#include "ImageSeries.h"
#include "InputConnector.h"
#include "Graph.h"

namespace taets {

class TransformNode : public ModifierNode {
    Q_DECLARE_TR_FUNCTIONS(TransformNode)

    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive &ar, const unsigned int) {
        ar & boost::serialization::base_object<ModifierNode>(*this);
    }

protected:
    /**
     * Konstruktor, der für die Deserialisierung benötigt wird.
     */
    TransformNode() {}

  public:
    TransformNode(Graph* graph, unsigned int id);
    /**
     * Skaliert das Eingabebild
     */
    virtual void compute();

  private:
    cv::Mat matrix;
};
}
#endif
