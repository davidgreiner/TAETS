#include "InspectorNode.h"

#include "Graph.h"

namespace taets {

/**
 * Bereite die Testdaten auf
 */
void InspectorNode::compute() {
    //später
    /* Vorsicht hier mit den Data-Dependencies, die Series werden (s.u.) gebraucht, um TestData abzuspeichern.
     * Sollen wir das direkt hier reinschreiben, damit bei compute direkt abgespeichert wird? Macht vieles einfacher... */
}

/**
 * Bereitet TestData auf, um sie danach zu analysieren
 */
TestData * InspectorNode::produceTestData() {
    TestData *data = new TestData(_graph->length());

    for(auto it : _inputConnectors) {
		if(it->source())
			data->addDataSeries(it->name(), it->source()->data());
    }

    return data;
}

}
