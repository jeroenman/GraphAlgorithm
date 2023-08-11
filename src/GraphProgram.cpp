#include "Graph.h"

int main()
{
    Graph* graph = new Graph("data/Graph.txt");
    graph->removeNodesWithEdgeCount(3);
    graph->printNodeRelationships();
    graph->clean();

    delete graph;

    return 0;
}
