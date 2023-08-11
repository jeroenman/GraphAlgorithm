#include "NodesManager.h"
#include "Helper.h"

NodesManager::NodesManager(string txtFilePath)
{
    // CREATE NODES BASED ON TXT FILE
    vector<NodeRelationship*> nodeRelationships = createNodeRelationships(txtFilePath);
    setupNodes(nodeRelationships);

    // CLEANUP nodeRelationships
    for (NodeRelationship* nodeRelationship : nodeRelationships)
    {
        delete nodeRelationship;
    }
}

vector<NodeRelationship*> NodesManager::createNodeRelationships(string txtPath)
{
    vector<NodeRelationship*> nodesRelationships;
    nodesRelationships.reserve(10);

    string s;
    ifstream in;

    in.open("Graph.txt");

    if (!in.is_open())
    {
        std::cerr << "Could not open Graph.txt" << std::endl;
        return nodesRelationships;
    }

    // CREATE A NodeRelationship FOR EACH LINE / RELATIONSHIP
    while (!in.eof())
    {
        getline(in, s);

        NodeRelationship* nodesRelationship = new NodeRelationship(s.substr(0, 1), s.substr(1, 2), s.substr(3, 1));
        nodesRelationships.push_back(nodesRelationship);
    }

    return nodesRelationships;
}

void NodesManager::setupNodes(vector<NodeRelationship*> nodeRelationships)
{
    nodes.reserve(26);

    std::map<std::string, Node*> madeNodesMap;

    for (NodeRelationship* nodeRelationship : nodeRelationships)
    {
        // CREATE NEW NODES IF THEY DON'T EXIST YET
        if (!madeNodesMap[nodeRelationship->node1Label])
        {
            Node* node = addNode(nodeRelationship->node1Label);
            madeNodesMap[nodeRelationship->node1Label] = node;
        }
        if (!madeNodesMap[nodeRelationship->node2Label])
        {
            Node* node = addNode(nodeRelationship->node2Label);
            madeNodesMap[nodeRelationship->node2Label] = node;
        }

        // GET NODES BASED ON LABEL
        Node* node1 = madeNodesMap[nodeRelationship->node1Label];
        Node* node2 = madeNodesMap[nodeRelationship->node2Label];

        // CONNECT THEM UP BASED ON DIRECTION
        if (nodeRelationship->direction == "->")
        {
            node2->receiveEdgeFrom(node1);
        }
        else
        if (nodeRelationship->direction == "<-")
        {
            node1->receiveEdgeFrom(node2);
        }
        else
        if (nodeRelationship->direction == "<>")
        {
            node2->receiveEdgeFrom(node1, Bidirectional);
            node1->receiveEdgeFrom(node2, Bidirectional);
        }
    }
}

Node* NodesManager::addNode(string label)
{
    Node* node = new Node(label);
    nodes.push_back(node);

    return node;
}

void NodesManager::removeNode(Node* node, int index)
{
    node->clean();
    spliceVectorAtIndex(nodes, index);
}

void NodesManager::removeNodesWithEdgeCount(int edgeCount)
{
    int nodesLength = static_cast<int>(nodes.size());

    for (int i = nodesLength - 1; i >= 0; i--)
    {
        Node* node = nodes[i];

        // FIND NODE THAT MATCHES EDGE COUNT TARGET
        int edgeCountMatchingNode = node->getEdgeCount();
        if (edgeCountMatchingNode == edgeCount)
        {
            // LOOP THROUGH ALL OF NODE'S EDGES
            for (int j = edgeCountMatchingNode - 1; j >= 0; j--)
            {
                Edge* edge = node->edges[j];
                if (edge->edgeType == Bidirectional)
                {
                    // IF EDGE IS BIDIRECTIONALLY CONNECTED TO ANOTHER, REMOVE EDGE FROM OTHER THAT POINTS TO NODE
                    Node* nodeConnection = edge->nodeConnection;
                    nodeConnection->removeEdgeAtIndex(edge->nodeConnectionIndex);
                }
            }

            removeNode(node, i);
        }
    }
}

string NodesManager::getStringOfNodeRelationships()
{
    string result = "";
    std::map<string, bool> nodeRelationshipsToIgnoreMap;

    int nodesLength = static_cast<int>(nodes.size());

    for (int i = 0; i < nodesLength; i++)
    {
        Node* node = nodes[i];
        vector<Edge*> edges = node->edges;
        int edgesLength = static_cast<int>(edges.size());

        for (int j = 0; j < edgesLength; j++)
        {
            Edge* otherEdge = edges[j];
            Node* otherNode = otherEdge->nodeConnection;
            string dirString = getDirectionStringFromEdgeType(otherEdge->edgeType);
            string nodeRelationshipStr = (otherNode->label + dirString + node->label);

            if (nodeRelationshipsToIgnoreMap[nodeRelationshipStr])
            {
                // ALREADY ADDED 
                continue;
            }

            result += nodeRelationshipStr + "\n";

            if (otherEdge->edgeType == Bidirectional)
            {
                // STORE WHICH BIDIRECTIONAL CONNECTIONS TO IGNORE, SINCE WE JUST ADDED ONE
                string nodeRelationshipStrInv = (node->label + dirString + otherNode->label);
                nodeRelationshipsToIgnoreMap[nodeRelationshipStrInv] = true;
            }
        }
    }

    return result;
}

void NodesManager::printNodeRelationships()
{
    string nodeRelationshipStr = getStringOfNodeRelationships();
    cout << nodeRelationshipStr;
}

string NodesManager::getDirectionStringFromEdgeType(EdgeType edgeType)
{
    switch (edgeType)
    {
    case Directional:
        return "->";
    case Bidirectional:
        return "<>";
    }

    return "";
}

void NodesManager::clean()
{
    for (Node* node : nodes)
    {
        node->clean();
        delete node;
    }
    nodes.clear();
}