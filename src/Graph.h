#ifndef TAETS_GRAPH_H
#define TAETS_GRAPH_H

#include <vector>
#include <utility>
#include <boost/serialization/access.hpp>
#include <QCoreApplication>

#include "InspectorNode.h"
#include "BinaryNode.h"

namespace taets {

/**
 * Diese Klasse stellt den Graph sowie das derzeitige Projekt dar
 */
class Graph {
   Q_DECLARE_TR_FUNCTIONS(Graph)
  private:
    std::vector<Node*> _nodes;
    InspectorNode* _inspector;
    unsigned int _node_id;
    unsigned int _length;
    std::string _output_filename;
    std::string _recovery_directory;

	std::vector<std::pair<int,Node*>> find(int position, Node* stack);

    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive &ar, const unsigned int) {
        ar & _nodes;
        ar & _inspector;
        ar & _node_id;
        ar & _length;
    }

protected:
    /**
     * Konstruktor, der für die Deserialisierung benötigt wird.
     */
    Graph() {}

  public:
    /**
     * @brief Graph Modell des Graphs
     */
    Graph(unsigned int length);
    ~Graph();

    template<class T>
    Node *createNode() {
        T *node = new T(this, _node_id++);
        _nodes.push_back(node);
        return node;
    }

    BinaryNode *createBinaryNode(BinaryTemplate *tmpl) {
        BinaryNode *node = new BinaryNode(tmpl, this, _node_id++);
        _nodes.push_back(node);
        return node;
    }

    /**
     * Entfernt eine Node vom Graph
     */
    void removeNode(Node* node);

    /**
     * Berechnet die durch den Graph definierten Prozess
     */
    void computeEverything();

    /**
     * Berechnet die Vorschau für die graphische Benutzeroberfläche
     */
    void computePreview();

    /**
     * Gibt die Länge des Projekts zurück
     */
    unsigned int length() {
        return _length;
    }

    /**
     * Setzt die Länge des Projekts
     */
    void setLength(unsigned int length) {
        this->_length = length;
    }

    /**
     * Gibt den Inspector zurück
     */
    InspectorNode* inspector() {
        return this->_inspector;
    }

    /**
     * Gibt den Dateinamen der Datei, in der die fertige TestData abgelegt wird, zurück.
     */
    std::string outputFilename() {
        return _output_filename;
    }

    /**
     * Gibt den Ordner, in den die Zwischenergebnisse der Knoten gespeichert werden.
     */
    std::string recoveryDirectory() {
        return _recovery_directory;
    }

    /**
     * Setzt die Ausgabedatei.
     */
     void setOutputFilename(std::string filename) {
         this->_output_filename = filename;
     }

    /**
     * Setzt das Recovery-Verzeichnis.
     */
     void setRecoveryDirectory(std::string directory) {
         this->_recovery_directory = directory;
     }

     std::vector<Node*> nodes() { return _nodes; }

     void save(std::string filename);
     static Graph * load(std::string filename);

  private:
    /**
     * Sortiert den Graph topologisch
     */
    std::vector<std::vector<Node*> > topologicSort();
};
}
#endif
