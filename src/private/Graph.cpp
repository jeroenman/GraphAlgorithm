#include "Graph.h"
#include "Helper.h"

Graph::Graph(string txtFilePath)
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

vector<NodeRelationship*> Graph::createNodeRelationships(string txtPath)
{
    vector<NodeRelationship*> nodesRelationships;
    nodesRelationships.reserve(10);

    string s;
    ifstream in;

    in.open(txtPath);

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

void Graph::setupNodes(vector<NodeRelationship*> nodeRelationships)
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

        // CONNECT THEM UP BASED ON DIRECTION - FOR THIS PROGRAM WE ONLY CARE ABOUT THE INCOMING EDGES FOR EACH NODE
        if (nodeRelationship->direction == "->")
        {
            node2->addEdgeFrom(node1);
        }
        else
        if (nodeRelationship->direction == "<-")
        {
            node1->addEdgeFrom(node2);
        }
        else
        if (nodeRelationship->direction == "<>")
        {
            connectBidirectionalEdgeToNodes(node1, node2);
        }
    }
}

void Graph::connectBidirectionalEdgeToNodes(Node* node1, Node* node2)
{
    // THESE ALWAYS HAVE TO GO TOGETHER FOR BIDIRECTIONAL
    node1->addEdgeFrom(node2, Bidirectional);
    node2->addEdgeFrom(node1, Bidirectional);
}

Node* Graph::addNode(string label)
{
    Node* node = new Node(label);
    nodes.push_back(node);

    return node;
}

void Graph::removeNode(Node* node, int index)
{
    node->clean();
    spliceVectorAtIndex(nodes, index);
}

vector <Node*> Graph::getNodesWithEdgeCount(int edgeCount)
{
    vector <Node*> matchingNodes;

    int nodesLength = static_cast<int>(nodes.size());
    for (int i = nodesLength - 1; i >= 0; i--)
    {
        Node* node = nodes[i];

        // FIND NODE THAT MATCHES EDGE COUNT TARGET
        int edgeCountMatchingNode = node->getEdgeCount();
        if (edgeCountMatchingNode == edgeCount)
        {
            matchingNodes.push_back(node);
        }
    }

    return matchingNodes;
}

void Graph::removeNodesWithEdgeCount(int edgeCount)
{
    vector <Node*> matchingNodes = getNodesWithEdgeCount(edgeCount);

    int nodesLength = static_cast<int>(matchingNodes.size());
    for (int i = nodesLength - 1; i >= 0; i--)
    {
        Node* node = matchingNodes[i];

        // LOOP THROUGH ALL OF NODE'S EDGES
        for (int j = edgeCount - 1; j >= 0; j--)
        {
            // IF EDGE IS BIDIRECTIONALLY CONNECTED TO ANOTHER...
            Edge* edge = node->edges[j];
            if (edge->edgeType == Bidirectional)
            {
                // FIND AND REMOVE EDGE FROM OTHER THAT POINTS TO NODE
                Node* nodeFrom = edge->nodeFrom;
                int edgeCountCheck = nodeFrom->getEdgeCount();
                for (int k = edgeCountCheck - 1; k >= 0; k--)
                {
                    Edge* edgeFrom = nodeFrom->edges[k];
                    if (edgeFrom->nodeFrom == node)
                    {
                        nodeFrom->removeEdgeAtIndex(k);
                        break;
                    }
                }
            }
        }

        removeNode(node, i);
    }
}

string Graph::getStringOfNodeRelationships()
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
            Node* otherNode = otherEdge->nodeFrom;
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

void Graph::printNodeRelationships()
{
    string nodeRelationshipStr = getStringOfNodeRelationships();
    cout << nodeRelationshipStr;
}

string Graph::getDirectionStringFromEdgeType(EdgeType edgeType)
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

void Graph::clean()
{
    for (Node* node : nodes)
    {
        node->clean();
        delete node;
    }
    nodes.clear();
}