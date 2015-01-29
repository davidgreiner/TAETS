#ifndef TAETS_INSPECTORNODE_H
#define TAETS_INSPECTORNODE_H

using namespace std;
#include "Node.h"
#include "InputConnector.h"
#include "TestData.h"
#include "FloatSeries.h"
#include "ImageSeries.h"
#include <vector>
#include <QCoreApplication>
#include <boost/serialization/access.hpp>

namespace taets {

class InspectorNode : public Node {
    Q_DECLARE_TR_FUNCTIONS(InspectorNode)

    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive &ar, const unsigned int) {
        ar & boost::serialization::base_object<Node>(*this);
    }

protected:
    /**
     * Konstruktor, der für die Deserialisierung benötigt wird.
     */
    InspectorNode() {}

  public:
    InspectorNode(Graph *graph, unsigned int id) : Node(graph, id, tr("Inspector").toStdString()) {
        _inputConnectors.push_back(new InputConnector<ImageSeries>(this, tr("image").toStdString()));
        _inputConnectors.push_back(new InputConnector<FloatSeries>(this, tr("float").toStdString()));
    }

    /**
     * Bereite die Testdaten auf
     */
    virtual void compute();

    /**
     *Fügt einen neuen Connector hinzu
     */
    void addConnector(InputConnectorBase * con) {
        _inputConnectors.push_back(con);
    }

    /**
     * Entfernt einen Connector, der mit dem Inspector verbunden war
     * @param [in] connector Connector, der entfernt werden soll
     */
    void removeConnector(InputConnectorBase * connector) {
        _inputConnectors.erase(std::find(_inputConnectors.begin(), _inputConnectors.end(), connector));
    }

    void renameConnector(InputConnectorBase * connector, const std::string & name) {
        connector->setName(name);
    }

    /**
     * Bereitet TestData auf, um sie danach zu analysieren
     */
    TestData * produceTestData();
};
}
#endif
