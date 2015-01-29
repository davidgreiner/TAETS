#ifndef TAETS_OUTPUTCONNECTORBASE_H
#define TAETS_OUTPUTCONNECTORBASE_H

#include "Connector.h"
#include "DataSeries.h"

namespace taets {

class OutputConnectorBase : public Connector
{
  private:
    unsigned int dependencyCount;
    //TODO: variablenbenennung...
    std::vector<InputConnectorBase*> _incoming;

    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive &ar, const unsigned int) {
        ar & boost::serialization::base_object<Connector>(*this);
        ar & _incoming;
    }

protected:
    /**
     * Konstruktor, der für die Deserialisierung benötigt wird.
     */
    OutputConnectorBase() {}

  public:
    OutputConnectorBase(Node * node, const std::string & name) : Connector(node, name) {
		dependencyCount = 0;
    }

    virtual DataSeries * data() = 0;

    /**
     * Erklärt die Abhängigkeit zu einer anderen Node
     */
    void declareDependency() {
        dependencyCount++;
    }

    /**
     * Entfernt die Abhängigkeit zu einer anderen Node
     */
    void releaseDependency() {
        dependencyCount--;
    }

    void connect(InputConnectorBase * other);

    void disconnect(InputConnectorBase * other);

    void disconnectAll();

    std::vector<InputConnectorBase*> incoming() {
        return _incoming;
    }
	// private virtual releaseTestData delete TestData
};
}
#endif // TAETS_OUTPUTCONNECTORBASE_H
