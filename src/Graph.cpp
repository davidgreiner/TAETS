#include "boost_class_exports.h"
#include "Graph.h"
#include "Node.h"
#include "DataSeries.h"
#include <QtCore>
#include <boost/bind.hpp>
#include <QtConcurrent/QtConcurrentMap>

namespace taets {

Graph::Graph(unsigned int length) {
    this->_node_id = 0;
    this->_length = length;
    this->_inspector = new InspectorNode(this, _node_id++);
}

Graph::~Graph() {
    delete _inspector;
    for(auto it : _nodes) {
        delete it;
    }
}

void Graph::computeEverything() {
    if(_output_filename.empty())
        throw std::runtime_error("output-file not set");

	std::vector<std::vector<Node*>> nodes = topologicSort();

	//QThreadPool *threadPool = QThreadPool::globalInstance();

	std::vector<std::vector<Node*>>::iterator lIt;
	// Über die Liste der listen iterieren
	for(lIt = nodes.begin(); lIt != nodes.end(); ++lIt) {

		QVector<Node*> qVec = QVector<Node*>::fromStdVector((*lIt));


        QtConcurrent::map(qVec.begin(), qVec.end(), [](Node *node) {
				node->run();
			}).waitForFinished();


	}

    TestData *data = _inspector->produceTestData();
    data->save(_output_filename);
    delete data; //TODO: Data aufheben, damit sie gleich in der Analyse verwendet werden kann ohne neu geladen zu werden?
}

void Graph::computePreview() {
    //David
}

std::vector<std::vector<Node*> > Graph::topologicSort() {
	std::vector<std::vector<Node*> > result;
	// Alle Nodes aus dem Graphen mit Rang auflisten (rekursiv)
	std::vector<std::pair<int, Node*>> nodes = find(0, _inspector);

	// Absteigend sortieren
	std::sort(nodes.begin(), nodes.end(),
			  boost::bind(&std::pair<int, Node*>::first, _1) >
			  boost::bind(&std::pair<int, Node*>::first, _2));

	// In Reihenfolge einteilen
	std::vector<std::pair<int, Node*>>::iterator it = nodes.begin();
    if(it == nodes.end())
        return result;
    int position = it->first;
	result.push_back(vector<Node*>());
	for(; it != nodes.end(); ++it) {
		if(position == it->first) {
			result.back().push_back(it->second);
        } else {
            position = it->first;
			result.push_back(vector<Node*>(1, it->second));
        }
	}
	return result;
}

std::vector<std::pair<int,Node*>> Graph::find(int position, Node* node) {
	std::vector<std::pair<int,Node*>> nodes;
	std::vector<Node*>::iterator it;
	std::vector<Node*> connectedNodes = node->getConnectedIncomingNodes();
    if((connectedNodes.size() != node->inputConnectors().size()) && (node != _inspector))
        throw std::runtime_error("Node missing Input Connector");
	for(it = connectedNodes.begin(); it != connectedNodes.end(); ++it) {
		// Rückgabe zusammenführen
		std::vector<std::pair<int,Node*>> new_nodes = find(position+1, (*it));
        std::vector<std::pair<int,Node*>>::iterator nIt;
        for(nIt = new_nodes.begin(); nIt != new_nodes.end(); ++nIt) {
            std::vector<std::pair<int,Node*>>::iterator pos = std::find_if(nodes.begin(), nodes.end(), [&nIt](std::pair<int, Node*> const& pair_node) { return pair_node.second == (*nIt).second; });
            if(pos != nodes.end()) {
                if((*pos).first < (*nIt).first)
                    (*pos).first = (*nIt).first;
            } else
                nodes.push_back((*nIt));
            //nodes.insert(nodes.end(), new_nodes.begin(), new_nodes.end());
        }
	}
	// Füge Knoten in die Liste ein
    // Prüfen ob Knoten schon mal eingefügt wurde
    std::vector<std::pair<int,Node*>>::iterator pos = std::find_if(nodes.begin(), nodes.end(), [&node](std::pair<int, Node*> const& pair_node) { return pair_node.second == node; });
	if(pos != nodes.end())
        (*pos).first = position + 1;
	else {
		std::vector<OutputConnectorBase*>::iterator conIt;
		std::vector<OutputConnectorBase*> connectedOutputConnectors = node->getConnectedOutputConnectors();
		for(conIt = connectedOutputConnectors.begin(); conIt != connectedOutputConnectors.end(); ++conIt) {
			(*conIt)->declareDependency();
		}
		nodes.push_back(std::make_pair(position, node));
	}
	return nodes;
}

 /**
  * Entfernt eine Node vom Graph
  */
 void Graph::removeNode(Node* node) {
     if(node == _inspector) return; //ignore inspector

     _nodes.erase(std::remove(_nodes.begin(), _nodes.end(), node), _nodes.end());
     delete node;
 }

void Graph::save(std::string filename) {
    std::ofstream fs(filename.c_str());
    boost::archive::text_oarchive ar(fs);
    ar << *this;
}

Graph * Graph::load(std::string filename) {
    Graph *graph = new Graph();

    std::ifstream fs(filename.c_str());
    boost::archive::text_iarchive ar(fs);

    ar >> *graph;

    return graph;
}

}
