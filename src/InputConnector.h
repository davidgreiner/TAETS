#ifndef TAETS_INPUTCONNECTOR_H
#define TAETS_INPUTCONNECTOR_H


#include "InputConnectorBase.h"
#include "OutputConnector.h"
#include <typeinfo>

namespace taets {

template<class T>
class InputConnector : public InputConnectorBase {

    friend class boost::serialization::access;
    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive &ar, const unsigned int) {
        ar & boost::serialization::base_object<InputConnectorBase>(*this);
    }

protected:
    /**
     * Konstruktor, der für die Deserialisierung benötigt wird.
     */
    InputConnector() {}

public:
    InputConnector(Node * node, const std::string & name) : InputConnectorBase(node, name) {

    }

    virtual std::string typeinfo() {
        return std::string(typeid(T).name());
    }
};
}
#endif
