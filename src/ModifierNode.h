#ifndef TAETS_MODIFIERNODE_H
#define TAETS_MODIFIERNODE_H
#include "Node.h"

namespace taets {

class ModifierNode : public Node {

    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive &ar, const unsigned int) {
        ar & boost::serialization::base_object<Node>(*this);
    }

protected:
    /**
     * Konstruktor, der für die Deserialisierung benötigt wird.
     */
    ModifierNode() {}

public:
    ModifierNode(Graph *graph, unsigned int id, const std::string & name) :
        Node(graph, id, name) {}
};

}

#endif
