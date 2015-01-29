#ifndef TAETS_CONNECTOR_H
#define TAETS_CONNECTOR_H

#include "Node.h"
#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>

namespace taets {

class Connector {
    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive &ar, const unsigned int) {
        ar & _node;
        ar & _name;
    }

protected:
    /**
     * Konstruktor, der für die Deserialisierung benötigt wird.
     */
    Connector() {}

  public:
    Connector(Node* node, const std::string & name) {
        this->_node = node;
        this->_name = name;
    }

    virtual ~Connector() {}

    virtual std::string typeinfo() = 0;

    Node* node() {
        return _node;
    }

    std::string name() {
        return _name;
    }

    void setName(std::string name) {
        _name = name;
    }

  private:
    Node * _node;
    std::string _name;
};
}
#endif
