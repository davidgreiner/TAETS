#ifndef TAETS_INPUTNODE_H
#define TAETS_INPUTNODE_H
#include "Node.h"

namespace taets {

class InputNode : public Node {
    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive &ar, const unsigned int) {
        ar & boost::serialization::base_object<Node>(*this);
    }

protected:
    /**
     * Konstruktor, der für die Deserialisierung benötigt wird.
     */
    InputNode() {}

public:
    InputNode(Graph *graph, unsigned int id, const std::string & name) :
        Node(graph, id, name) {}
};

}

#endif
