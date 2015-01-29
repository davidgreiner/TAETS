#ifndef TAETS_INPUTCONNECTORBASE_H
#define TAETS_INPUTCONNECTORBASE_H

#include "Connector.h"
#include "OutputConnectorBase.h"

namespace taets {

class InputConnectorBase : public Connector
{
private:
  OutputConnectorBase* _source;

  friend class boost::serialization::access;
  template<class Archive> void serialize(Archive &ar, const unsigned int) {
      ar & boost::serialization::base_object<Connector>(*this);
      ar & _source;
  }

protected:
  /**
   * Konstruktor, der für die Deserialisierung benötigt wird.
   */
  InputConnectorBase() {}


public:
  InputConnectorBase(Node * node, const std::string & name) : Connector(node, name) {
      _source = nullptr;
  }

  bool isConnectionPossible(OutputConnectorBase * out);

  /**
   * Verbindet einen InputConnector mit einem OutputConnector
   * @param [in] other OutputConnector, mit dem verbunden werden soll
   */
  void connect(OutputConnectorBase * other);

  /**
   * Löscht die Verbindung mit dem OutputConnector.
   */
  void disconnect();

  OutputConnectorBase* source() {
      return _source;
  }

};
}
#endif // TAETS_INPUTCONNECTORBASE_H
