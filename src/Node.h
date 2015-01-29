#ifndef TAETS_NODE_H
#define TAETS_NODE_H

#include <vector>
#include <string>
#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/vector.hpp>
#include "NodeSetting.h"

namespace taets {   class Graph;
                    class InputConnectorBase;
                    class OutputConnectorBase;
                }

namespace taets {

class Node {
  protected:
    Graph * _graph;
    unsigned int _id;
    std::string _name;
    std::vector<InputConnectorBase*> _inputConnectors;
    std::vector<OutputConnectorBase*> _outputConnectors;
    std::vector<NodeSetting*> _settings;
    float _layout_x, _layout_y;

    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive &ar, const unsigned int) {
        ar & _graph;
        ar & _id;
        ar & _name;
        ar & _inputConnectors;
        ar & _outputConnectors;
        ar & _settings;
        ar & _layout_x;
        ar & _layout_y;
    }

protected:
    /**
     * Konstruktor, der für die Deserialisierung benötigt wird.
     */
    Node() {}

  public:
    /**
     * Erstellt eine neue Node. Dieser Konstruktor darf nur von Graph::create() aufgerufen werden.
     */
    Node(Graph *graph, unsigned int id, const std::string &name);

    virtual ~Node();

    /**
     * Berechne einen Schritt
     */
    virtual void compute() = 0;

    /**
     * Schablone für compute(): Body sieht ungefähr so aus:
     * compute();
     * foreach inputConnector:
     *  inputConnector.source.releaseDependency()
     * if preview: emit outputChanged()
     */
	void run();

    /**
      * Gibt die Settings zurück
      */
    std::vector<NodeSetting*> getSettings() {
        return this->_settings;
    }

    /**
      * Gibt die inputConnectors zurück
      */
    std::vector<InputConnectorBase *> inputConnectors() {
        return _inputConnectors;
    }

    /**
      * Gibt die outputConnectors zurück
      */
    std::vector<OutputConnectorBase *> outputConnectors() {
        return _outputConnectors;
    }

    /** Gibt den Graphen zurück
     *
     */
    Graph * graph() {
        return _graph;
    }

    /**
     * Gibt den Namen der Node zurück.
     */
    std::string name() {
        return _name;
    }

    /**
     * Gibt die ID der Node zurück.
     */
    unsigned int id() {
        return _id;
    }
    /**
     * vergleicht den Node mit einem anderen Node und gibt zurück, ob die beiden gleich sind.
     * Bisher nur für den Vergleich beim Erstellen von ConnectionItems. Falls nicht klar sein sollte,
     * dass das andere Objekt auch ein Node ist, muss das hier umgeschrieben werden.
     */
    int equals(Node * node) {
        return (_id == node->id());
    }

    /**
     * Gibt den Index der Output-Konnektors zurück, der für das Vorschaubild genutzt werden soll oder -1, wenn der Knoten keine Vorschau unterstützt.
     */
    int getPreviewConnectorIndex();

    /**
     * Wird ausgelöst, wenn im Vorschaumodus gerendert wird und sich ein oder mehrere Output-Konnektoren geändert haben.
     */
     //QT-Signal eigentlich
     void outputChanged();

	 std::vector<InputConnectorBase*> getConnectedIncomingConnectors();

	 std::vector<OutputConnectorBase*> getConnectedOutputConnectors();
     /**
      * Gibt einkommende Knoten zurück
      */
	 std::vector<Node*> getConnectedIncomingNodes();
	 /**
	  * Gibt ausgehende Knoten zurück
	  */
	 std::vector<Node*> getConnectedOutgoingNodes();

     void setLayoutX(float x) {
         _layout_x = x;
     }

     void setLayoutY(float y) {
         _layout_y = y;
     }

     float layoutX() {
         return _layout_x;
     }

     float layoutY() {
         return _layout_y;
     }
};

}
#endif
