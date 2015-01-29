#include "Node.h"
#include "Graph.h"
#include "InputConnector.h"
#include "OutputConnector.h"
#include "NodeSetting.h"

namespace taets {

Node::Node(Graph *graph, unsigned int id, const std::string & name) {
    _graph = graph;
    _id = id;
    _name = name;
    _layout_x = _layout_y = 0.0;
}

Node::~Node() {
    for(auto it : _inputConnectors) {
        it->disconnect();
        delete it;
    }
    for(auto it : _outputConnectors) {
        it->disconnectAll();
        delete it;
    }
    for(auto it : _settings) {
        delete it;
    }
}

void Node::run() {
    //David
	// compute
	// release dependency every connected node (input)
	compute();

}

int Node::getPreviewConnectorIndex() {
    //David
    return 0;
}

std::vector<InputConnectorBase*> Node::getConnectedIncomingConnectors() {
	std::vector<InputConnectorBase*> connectedConnectors;
	std::vector<InputConnectorBase*>::iterator it;
	for(it = _inputConnectors.begin(); it != _inputConnectors.end(); ++it) {
		if((*it)->source() != nullptr)
			 connectedConnectors.push_back((*it));
	}
	return connectedConnectors;
}

std::vector<OutputConnectorBase*> Node::getConnectedOutputConnectors() {
	std::vector<OutputConnectorBase*> connectedConnectors;
	std::vector<OutputConnectorBase*>::iterator it;
	for(it = _outputConnectors.begin(); it != _outputConnectors.end(); ++it) {
		if((*it)->incoming().size() != 0)
			connectedConnectors.push_back((*it));
	}
	return connectedConnectors;
}

std::vector<Node*> Node::getConnectedIncomingNodes() {
    std::vector<Node*> connectedNodes;
    std::vector<InputConnectorBase*>::iterator it;
	std::vector<InputConnectorBase*> connectedConnectors = getConnectedIncomingConnectors();
	for(it = connectedConnectors.begin(); it != connectedConnectors.end(); ++it) {
		if((*it)->source() != nullptr)
			connectedNodes.push_back((*it)->source()->node());
    }
    return connectedNodes;
}

std::vector<Node*> Node::getConnectedOutgoingNodes() {
    std::vector<Node*> outgoingNodes;
    std::vector<OutputConnectorBase*>::iterator it;
	for(it = getConnectedOutputConnectors().begin(); it != getConnectedOutputConnectors().end(); ++it) {
        std::vector<InputConnectorBase*>::iterator nIt;
        for(nIt = (*it)->incoming().begin(); nIt != (*it)->incoming().end(); ++nIt)
            outgoingNodes.push_back((*nIt)->source()->node());
    }
    return outgoingNodes;
}
}
