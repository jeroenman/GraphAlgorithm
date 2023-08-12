#include "Graph.h"
#include "Helper.h"
#include <algorithm>

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
        std::cerr << "Could not open file located at: "+ txtPath << std::endl;
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

        // CONNECT THEM UP BASED ON DIRECTION - FOR THIS PROGRAM WE ONLY CARE ABOUT THE In EDGES FOR EACH NODE
        if (nodeRelationship->direction == "->")
        {
            addEdge(node1, node2);
        }
        else
        if (nodeRelationship->direction == "<-")
        {
            addEdge(node2, node1);
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
    Edge* edge = new Edge(node1, node2, Bidirectional);
    node1->addEdgeFrom(edge);
    node2->addEdgeFrom(edge);
}

Node* Graph::addNode(string label)
{
    Node* node = new Node(label);
    nodes.push_back(node);

    return node;
}

Edge* Graph::addEdge(Node* nodeFrom, Node* nodeTo, EdgeType edgeType)
{
	Edge* edge = new Edge(nodeFrom, nodeTo, edgeType);
    nodeFrom->addEdgeTo(edge);
    nodeTo->addEdgeFrom(edge);

	return edge;
}

void Graph::removeEdge(Edge* edge)
{
    if (edge->edgeType == Directional)
    {
        edge->nodeFrom->removeEdge(edge);
        edge->nodeTo->removeEdge(edge);
    }
    else
    if (edge->edgeType == Bidirectional)
    {
        edge->nodeFrom->removeEdgeIn(edge);
        edge->nodeTo->removeEdgeIn(edge);
    }

    delete edge;
}


void Graph::removeEdgesPointingToNode(Node* node)
{
    // LOOP THROUGH ALL OF NODE'S EDGES
    int edgesOutLength = static_cast<int>(node->edgesOut.size());
    for (int j = edgesOutLength - 1; j >= 0; j--)
    {
        Edge* edgeOut = node->edgesOut[j];
        if (edgeOut->edgeType == Directional)
        {
            // LOOP THROUGH ALL OF CONNECTED NODE'S INCOMING EDGES
            Node* connectedNode = edgeOut->nodeTo;
            int edgeCountInLength = connectedNode->getEdgeCountIn();
            for (int k = edgeCountInLength - 1; k >= 0; k--)
            {
                // FIND THE EDGE INCOMING FROM THE NODE
                Edge* edgeIn = connectedNode->edgesIn[k];
                if (edgeIn->edgeType == Directional && edgeIn->nodeFrom == node)
                {
                    removeEdge(edgeIn);
                }
            }
        }
    }
    int edgesInLength = static_cast<int>(node->edgesIn.size());
    for (int j = edgesInLength - 1; j >= 0; j--)
    {
        Edge* edgeIn = node->edgesIn[j];
        if (edgeIn->edgeType == Bidirectional)
        {
            // FIND AND REMOVE EDGE FROM OTHER THAT POINTS TO NODE
            Node* nodeFrom = node != edgeIn->nodeFrom ? edgeIn->nodeFrom : edgeIn->nodeTo;
            int edgeCountCheck = nodeFrom->getEdgeCountIn();
            for (int k = edgeCountCheck - 1; k >= 0; k--)
            {
                Edge* edgeFrom = nodeFrom->edgesIn[k];
                if (edgeFrom->nodeFrom == node || edgeFrom->nodeTo == node)
                {
                    removeEdge(edgeFrom);
                    break;
                }
            }
        }
    }
}

void Graph::removeNode(Node* node, int index)
{
    removeEdgesPointingToNode(node);

    node->clean();
    spliceVectorAtIndex(nodes, index);

    delete node;
}

vector <Node*> Graph::getNodesWithEdgeCount(int edgeCount)
{
    vector <Node*> matchingNodes;

    int nodesLength = static_cast<int>(nodes.size());
    for (int i = nodesLength - 1; i >= 0; i--)
    {
        Node* node = nodes[i];

        // FIND NODE THAT MATCHES EDGE COUNT TARGET
        int edgeCountNode = node->getEdgeCountIn();
        if (edgeCountNode == edgeCount)
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
        int indexNode = getIndexOfElementInVector(nodes, node);
        removeNode(node, indexNode);
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
        vector<Edge*> edges = node->edgesIn;
        int edgesLength = static_cast<int>(edges.size());

        for (int j = 0; j < edgesLength; j++)
        {
            Edge* otherEdge = edges[j];
            Node* otherNode = node != otherEdge->nodeFrom ? otherEdge->nodeFrom : otherEdge->nodeTo;
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
    return;
    for (Node* node : nodes)
    {
        node->clean();
        delete node;
    }
    nodes.clear();
}