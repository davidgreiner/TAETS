#ifndef IMAGEINPUTNODE_H
#define IMAGEINPUTNODE_H
#include "InputNode.h"
#include "NumberSetting.h"
#include "FileSetting.h"
#include "OutputConnector.h"
#include "ImageSeries.h"
#include "Graph.h"
#include <QCoreApplication>

namespace taets {

class ImageInputNode : public InputNode {
    Q_DECLARE_TR_FUNCTIONS(ImageInputNode)

    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive &ar, const unsigned int) {
        ar & boost::serialization::base_object<InputNode>(*this);
    }

protected:
    /**
     * Konstruktor, der für die Deserialisierung benötigt wird.
     */
    ImageInputNode() {}

  public:
    ImageInputNode(Graph* graph, unsigned int id);

    /**
     * Lese und konvertiere gegebenfalls das Eingabevideo
     */
    virtual void compute();

};
}
#endif // IMAGEINPUTNODE_H
