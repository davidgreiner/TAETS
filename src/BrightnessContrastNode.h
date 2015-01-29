#ifndef TAETS_BRIGHTNESSCONTRASTNODE_H
#define TAETS_BRIGHTNESSCONTRASTNODE_H
#include <QCoreApplication>
#include "ModifierNode.h"
#include "NumberSetting.h"
#include "OutputConnector.h"
#include "ImageSeries.h"
#include "InputConnectorBase.h"
#include "Graph.h"

namespace taets {

class BrightnessContrastNode : public ModifierNode {
    Q_DECLARE_TR_FUNCTIONS(BrightnessContrastNode)

    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive &ar, const unsigned int) {
        ar & boost::serialization::base_object<ModifierNode>(*this);
    }

protected:
    /**
     * Konstruktor, der für die Deserialisierung benötigt wird.
     */
    BrightnessContrastNode() {}

  public:
    BrightnessContrastNode(Graph* graph, unsigned int id);
    /**
     * Verändert die Helligkeit / Kontrast des Eingabebilds
     */
    virtual void compute();
};
}
#endif
