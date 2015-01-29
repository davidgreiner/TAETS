#include "OutputConnectorBase.h"

#include "InputConnectorBase.h"

#include <algorithm>

namespace taets {

void OutputConnectorBase::connect(InputConnectorBase *other) {
    if(std::count(_incoming.begin(), _incoming.end(), other) == 0) {
        _incoming.push_back(other);
        other->connect(this);
    }
}

void OutputConnectorBase::disconnect(InputConnectorBase * other) {
    if(std::count(_incoming.begin(), _incoming.end(), other) != 0) {
        _incoming.erase(std::find(_incoming.begin(), _incoming.end(), other));
        other->disconnect();
    }
}

void OutputConnectorBase::disconnectAll() {
    while(!_incoming.empty()) {
        InputConnectorBase * i = _incoming.back();
        _incoming.pop_back();
        i->disconnect();
    }
}

}
