#ifndef PSNRNODE_H
#define PSNRNODE_H
#include <QCoreApplication>
#include "ModifierNode.h"

namespace taets {

class PSNRNode : public ModifierNode
{
    Q_DECLARE_TR_FUNCTIONS(PSNRNode)

    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive &ar, const unsigned int) {
        ar & boost::serialization::base_object<ModifierNode>(*this);
    }

protected:
    /**
     * Konstruktor, der für die Deserialisierung benötigt wird.
     */
    PSNRNode() {}

public:
    PSNRNode(Graph *graph, unsigned int id);
    /**
     * Berechnet den PSNR(=Peak Signal-to-Noise Ratio)-Wert zweier Bilder
     */
    void compute();
};

}

#endif
