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
            addEdge(node1, node2);
            addEdge(node2, node1);
        }
    }
}

Node* Graph::addNode(string label)
{
    Node* node = new Node(label);
    nodes.push_back(node);

    return node;
}

void Graph::removeNode(Node* node, int index)
{
    removeEdgesFromNodeAndConnections(node);
    spliceVectorAtIndex(nodes, index);

    delete node;
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

Edge* Graph::addEdge(Node* nodeFrom, Node* nodeTo)
{
	Edge* edge = new Edge(nodeFrom, nodeTo);
    nodeFrom->addEdge(edge);
    nodeTo->addEdge(edge);

	return edge;
}

void Graph::removeEdge(Edge* edge)
{
    // REMOVE EDGE IN NODE FROM
    Node* nodeFrom = edge->nodeFrom;
    int nodeFromEdgesLength = static_cast<int>(nodeFrom->edges.size());
    for (int j = nodeFromEdgesLength - 1; j >= 0; j--)
    {
        Edge* fromEdge = nodeFrom->edges[j];

        if (fromEdge->nodeTo == edge->nodeTo)
            nodeFrom->removeEdge(fromEdge);
    }

    // REMOVE EDGE IN NODE TO
    Node* nodeTo = edge->nodeTo;
    int nodeToEdgesLength = static_cast<int>(nodeTo->edges.size());
    for (int j = nodeToEdgesLength - 1; j >= 0; j--)
    {
        Edge* toEdge = nodeTo->edges[j];

        if (toEdge->nodeFrom == edge->nodeFrom)
            nodeTo->removeEdge(toEdge);
    }

    // ONLY NOW, DELETE IT
    delete edge;
}

void Graph::removeEdgesFromNodeAndConnections(Node* node)
{
    // LOOP THROUGH ALL OF NODE'S EDGES
    int edgesLength = static_cast<int>(node->edges.size());
    for (int i = edgesLength - 1; i >= 0; i--)
    {
        Edge* edge = node->edges[i];

        // COULD SIMPLY USE THIS, BUT BELOW IS MORE EFFICIENT, SINCE WE CAN CLEAR ALL THE NODE'S EDGES AT ONCE
        //removeEdge(edge);

        // FIND OTHER NODE THAT EDGE POINTS TO
        Node* otherNode = edge->nodeFrom;
        if (edge->nodeTo != node)
        {
            otherNode = edge->nodeTo;
        }

        // REMOVE EDGE FROM OTHER NODE
        int otherEdgesLength = static_cast<int>(otherNode->edges.size());
        for (int j = otherEdgesLength - 1; j >= 0; j--)
        {
            Edge* otherEdge = otherNode->edges[j];
            if (otherEdge->nodeFrom == node || otherEdge->nodeTo == node)
            {
                otherNode->removeEdgeAtIndex(j);
            }
        }

        // ONLY NOW, DELETE IT
        delete edge;
    }

    node->clearEdges();
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

string Graph::getStringOfNodeRelationships()
{
    // FIND THE MODE RELATIONSHIPS
    vector<NodeRelationship*> nodeRelationships;

    int nodesLength = static_cast<int>(nodes.size());
    for (int i = 0; i < nodesLength; i++)
    {
        Node* node = nodes[i];
        vector<Edge*> edges = node->getEdgesIn();
        int edgesLength = static_cast<int>(edges.size());

        for (int j = 0; j < edgesLength; j++)
        {
            Edge* otherEdge = edges[j];
            Node* otherNode = (node != otherEdge->nodeFrom ? otherEdge->nodeFrom : otherEdge->nodeTo);
            string dirString = "->";
            string nodeRelationshipStr = (otherNode->label + dirString + node->label);

            NodeRelationship* nodeRelationship = new NodeRelationship(otherNode->label, dirString, node->label);

            // LOOK IF RELATIONSHIP ALREADY EXISTS
            bool foundMatch = false;
            int nodeRelationshipsLength = static_cast<int>(nodeRelationships.size());
            for (int k = 0; k < nodeRelationshipsLength; k++)
            {
                NodeRelationship* nodeRelationshipOther = nodeRelationships[k];
                if (nodeRelationshipOther->equals(nodeRelationship))
                {
                    // IF IT DOES, MARK IT BIDIRECTIONAL
                    nodeRelationshipOther->direction = "<>";
                    foundMatch = true;
					break;
				}
            }

            if (foundMatch)
            {
                delete nodeRelationship;
                continue;
            }

            nodeRelationships.push_back(nodeRelationship);
        }
    }

    // BUILD RESULT STRING WITH MODE RELATIONSHIPS
    string result = "";

    int nodeRelationshipsLength = static_cast<int>(nodeRelationships.size());
    for (int i = 0; i < nodeRelationshipsLength; i++)
    {
		NodeRelationship* nodeRelationship = nodeRelationships[i];
		result += nodeRelationship->toString() + "\n";

        delete nodeRelationship;
	}

    return result;
}

void Graph::printNodeRelationships()
{
    string nodeRelationshipStr = getStringOfNodeRelationships();
    cout << nodeRelationshipStr;
}

void Graph::clean()
{
    int nodesLength = static_cast<int>(nodes.size());
    for (int i = nodesLength - 1; i >= 0; i--)
    {
        Node* node = nodes[i];
        removeNode(node, i);
    }
}