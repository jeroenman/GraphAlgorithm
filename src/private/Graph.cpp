#include "Graph.h"
#include "Helper.h"
#include <unordered_set>

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
        if (!madeNodesMap[nodeRelationship->nodeFromLabel])
        {
            Node* node = addNode(nodeRelationship->nodeFromLabel);
            madeNodesMap[nodeRelationship->nodeFromLabel] = node;
        }
        if (!madeNodesMap[nodeRelationship->nodeToLabel])
        {
            Node* node = addNode(nodeRelationship->nodeToLabel);
            madeNodesMap[nodeRelationship->nodeToLabel] = node;
        }

        // GET NODES BASED ON LABEL
        Node* nodeFrom = madeNodesMap[nodeRelationship->nodeFromLabel];
        Node* nodeTo = madeNodesMap[nodeRelationship->nodeToLabel];

        // CONNECT THEM UP BASED ON DIRECTION
        if (nodeRelationship->direction == "->")
        {
            addEdge(nodeFrom, nodeTo);
        }
        else
        if (nodeRelationship->direction == "<-")
        {
            addEdge(nodeTo, nodeFrom);
        }
        else
        if (nodeRelationship->direction == "<>")
        {
            addEdge(nodeFrom, nodeTo);
            addEdge(nodeTo, nodeFrom);
        }
    }
}

Node* Graph::addNode(string label)
{
    Node* node = new Node(label);
    nodes.insert(node);

    return node;
}

void Graph::removeNode(Node* node)
{
    node->clearEdges();
    nodes.erase(node);

    delete node;
}

void Graph::removeNodesWithEdgeCount(int edgeCount)
{
    vector <Node*> matchingNodes = getNodesWitIncomingNumberOfEdges(edgeCount);

    int nodesLength = static_cast<int>(matchingNodes.size());
    for (int i = nodesLength - 1; i >= 0; i--)
    {
        Node* node = matchingNodes[i];
        removeNode(node);
    }
}

Edge* Graph::addEdge(Node* nodeFrom, Node* nodeTo)
{
	Edge* edge = new Edge(nodeFrom, nodeTo);
    nodeFrom->addEdgeOut(edge);
    nodeTo->addEdgeIn(edge);

	return edge;
}

void Graph::removeEdge(Edge* edge)
{
    Node* nodeFrom = edge->nodeFrom;
    nodeFrom->removeEdgeOut(edge); // WILL ALSO REMOVE IT FROM THE OTHER NODE
}

vector <Node*> Graph::getNodesWitIncomingNumberOfEdges(int edgeCount)
{
    vector <Node*> matchingNodes;

    for (Node* node : nodes)
    {
        // FIND NODE THAT MATCHES EDGE COUNT TARGET
        int edgeCountNode = node->getEdgeCountIn();
        if (edgeCountNode == edgeCount)
        {
            matchingNodes.push_back(node);
        }
    }

    return matchingNodes;
}

vector<NodeRelationship*> Graph::getNodeRelationships()
{
    vector<NodeRelationship*> nodeRelationships;

    for (Node* node : nodes)
    {
        unordered_set<Edge*> edges = node->edgesIn;
        int edgesLength = static_cast<int>(edges.size());

        for (Edge* otherEdge : edges)
        {
            Node* otherNode = (node != otherEdge->nodeFrom ? otherEdge->nodeFrom : otherEdge->nodeTo);
            string dirString = "->";
            string nodeRelationshipStr = (otherNode->label + dirString + node->label);

            NodeRelationship* nodeRelationship = new NodeRelationship(otherNode->label, dirString, node->label);

            // LOOK IF RELATIONSHIP ALREADY EXISTS...
            bool foundMatch = false;
            int nodeRelationshipsLength = static_cast<int>(nodeRelationships.size());
            for (int k = 0; k < nodeRelationshipsLength; k++)
            {
                NodeRelationship* nodeRelationshipOther = nodeRelationships[k];
                if (nodeRelationshipOther->equals(nodeRelationship))
                {
                    // ...IF IT DOES, MARK IT BIDIRECTIONAL
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

    return nodeRelationships;
}

string Graph::getStringOfNodeRelationships()
{
    string result = "";
    vector<NodeRelationship*> nodeRelationships = getNodeRelationships();

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
    // LITTLE SHUFFLE TO REMOVE NODES IN THE NEXT LOOP
    std::vector<Node*> toRemove;
    for (Node* node : nodes)
    {
        toRemove.push_back(node);
    }

    for (Node* node : toRemove)
    {
        removeNode(node);
    }
}