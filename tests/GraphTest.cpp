#include <unittest++/UnitTest++.h>
#include "Graph.h"
#include "Node.h"
#include "VideoInputNode.h"
#include "BlurNode.h"
#include "InspectorNode.h"

using namespace taets;

SUITE(GraphTests) {

TEST(GraphBasic) {
    Graph *graph = new Graph(10);
    Node* inputnode = graph->createNode<VideoInputNode>();
    Node* blurnode = graph->createNode<BlurNode>();
    Node* toremove = graph->createNode<BlurNode>();
    InspectorNode* inspector = graph->inspector();

    //basic tests
    graph->removeNode(toremove);
    CHECK(10 == graph->length());
    graph->setLength(5);
    CHECK(5 == graph->length());
    graph->setOutputFilename("dummy.txt");
    graph->setRecoveryDirectory("/dummyfolder");
    CHECK("dummy.txt" == graph->outputFilename());
    CHECK("/dummyfolder" == graph->recoveryDirectory());
}

}
