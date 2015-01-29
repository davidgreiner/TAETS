#include "ConstantNode.h"
#include "FloatSeries.h"

namespace taets {

ConstantNode::ConstantNode(Graph *graph, unsigned int id) :
   InputNode(graph, id, tr("Constant Node").toStdString()) {
    _outputConnectors.push_back(new OutputConnector<FloatSeries>(this, tr("constant value").toStdString()));
    _settings.push_back(new NumberSetting(tr("value").toStdString(), 0, 10000, 0, false, true));
}

void ConstantNode::compute() {
    NumberSetting* value_c = static_cast<NumberSetting*>(_settings.front());
    float value = value_c->floatValue();
    FloatSeries* out = new FloatSeries(_graph->length());

    for (int i = 0; i < _graph->length(); i++) {
        out->setFloatItem(i, value);
    }

    static_cast<OutputConnector<FloatSeries>*>(_outputConnectors[0])->setData(out);
}
}
