#include "NodesManager.h"

int main()
{
    NodesManager* nodesManager = new NodesManager("Graph.txt");
    nodesManager->removeNodesWithEdgeCount(3);
    nodesManager->printNodeRelationships();
    nodesManager->clean();

    delete nodesManager;

    return 0;
}
