#include <unittest++/UnitTest++.h>
#include "Graph.h"
#include "VideoInputNode.h"
#include "BlurNode.h"

using namespace taets;

TEST(LinearGraph) {
    std::cout << "LinearGraph" << std::endl;
    Graph *graph = new Graph(1);

    Node* inputnode = graph->createNode<VideoInputNode>();
    FileSetting* file =  static_cast<FileSetting*>(inputnode->getSettings().at(2));
    file->setFilename("tests/res/waterfall_cif_352x288.yuv");
    Node* blurnode = graph->createNode<BlurNode>();
    InspectorNode* inspector = graph->inspector();

    blurnode->inputConnectors().front()->connect(inputnode->outputConnectors().front());
    inspector->inputConnectors().front()->connect(blurnode->outputConnectors().front());

	graph->setOutputFilename("dummy.dat");

    graph->computeEverything();
}
