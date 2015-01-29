#include "InputConnectorBase.h"
#include "OutputConnectorBase.h"
#include <map>
#include <queue>

namespace taets {

void InputConnectorBase::connect(OutputConnectorBase *other) {
    if(this->_source != other) {
        this->_source = other;
        other->connect(this);
    }
}

void InputConnectorBase::disconnect() {
    if(_source != nullptr) {
        OutputConnectorBase * s = _source;
        _source = nullptr;
        s->disconnect(this);
    }
}

bool InputConnectorBase::isConnectionPossible(OutputConnectorBase * out) {
    if(typeinfo().compare(out->typeinfo()) != 0) return false;

    Node * target = node();
    std::map<Node*, bool> visited;
    std::queue<Node *> q;
    q.push(out->node());
    while(!q.empty()) {
        Node * current = q.front();
        q.pop();
        visited[current] = true;
        if(current == target) break;
        for(InputConnectorBase * inCon : current->inputConnectors()) {
            if(!inCon->source()) continue;
            if(visited.count(inCon->source()->node()) == 0)
                q.push(inCon->source()->node());
        }
    }
    // Keine Kreise genau dann, wenn visited das Ziel nicht enthält
    return visited.count(target) == 0;
}


}
