#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>

using namespace std;

enum EdgeType { Directional, Bidirectional };

struct Node;

struct Edge
{
    Edge(Node* _nodeConnection, int _nodeConnectionIndex, EdgeType _edgeType)
    {
        nodeConnection = _nodeConnection;
        nodeConnectionIndex = _nodeConnectionIndex;
        edgeType = _edgeType;
    }

    Node* nodeConnection = nullptr;
    int nodeConnectionIndex = -1;
    EdgeType edgeType;
};

struct Node
{
    Node(string _label)
    {
        label = _label;
        edges.reserve(3);
    }

    string label = "";

    vector<Edge*> edges;

    int getEdgeCount() const { return static_cast<int>(edges.size()); }

    void connectTo(Node* node, EdgeType edgeType = EdgeType::Directional)
    {
        Edge* edge = new Edge(this, node->getEdgeCount(), edgeType);
        node->edges.push_back(edge);
    }

    void connectInOneDirectionTo(Node* node)
    {
        connectTo(node, EdgeType::Directional);
    }

    void connectInBothDirectionsTo(Node* node)
    {
        connectTo(node, EdgeType::Bidirectional);
        node->connectTo(this, EdgeType::Bidirectional);
    }

    void clean()
    {
        for (Edge* edge : edges)
        {
            delete edge;
        }
        edges.clear();
    }
};

struct StringEntry
{
    StringEntry(string _node1Label, string _direction, string _node2Label)
    {
        node1Label = _node1Label;
        direction = _direction;
        node2Label = _node2Label;
    }

    string node1Label;
    string node2Label;
    string direction;
};

template <typename T>
void spliceVectorAtIndex(std::vector<T>& vec, int index)
{
    vec.erase(vec.begin() + index);
}

void removeNodesWithEdgeCount(vector<Node*>& nodes, int edgeCount)
{
    int arrSize = static_cast<int>(nodes.size());

    for (int i = arrSize - 1; i >= 0; i--)
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
                if (edge->edgeType == EdgeType::Bidirectional)
                {
                    // IF NODE IS BIDIRECTIONAL CONNECTED TO ANOTHER, REMOVE EDGE FROM OTHER THAT POINTS TO NODE
                    Node* nodeConnection = edge->nodeConnection;
                    spliceVectorAtIndex(nodeConnection->edges, edge->nodeConnectionIndex);
                }
            }

            nodes.erase(nodes.begin() + i);
        }
    }
}
string getDirectionStringFromEdgeType(EdgeType edgeType)
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

vector<StringEntry*> getStringEntriesFromTxt(string txtPath)
{
    vector<StringEntry*> stringEntries;

    string s;
    ifstream in;

    in.open("Graph.txt");

    if (!in.is_open())
    {
        std::cerr << "Could not open Graph.txt" << std::endl;
        return stringEntries;
    }

    // CREATE A StringEntry FOR EACH LINE / RELATIONSHIP
    while (!in.eof())
    {
        getline(in, s);

        StringEntry* stringEntry = new StringEntry(s.substr(0, 1), s.substr(1, 2), s.substr(3, 1));
        stringEntries.push_back(stringEntry);
    }

    return stringEntries;
}

vector<Node*> getNodesFromStringEntries(vector<StringEntry*> stringEntries)
{
    vector<Node*> nodes;
    std::map<std::string, Node*> madeNodesMap;

    for (StringEntry* entry : stringEntries)
    {
        // CREATE NEW NODES IF THEY DON'T EXIST YET
        if (!madeNodesMap[entry->node1Label])
        {
            Node* node = madeNodesMap[entry->node1Label] = new Node(entry->node1Label);
            nodes.push_back(node);
        }
        if (!madeNodesMap[entry->node2Label])
        {
            Node* node = madeNodesMap[entry->node2Label] = new Node(entry->node2Label);
            nodes.push_back(node);
        }

        // GET NODES BASED ON LABEL
        Node* node1 = madeNodesMap[entry->node1Label];
        Node* node2 = madeNodesMap[entry->node2Label];

        // CONNECT THEM UP BASED ON DIRECTION
        if (entry->direction == "->")
        {
			node1->connectInOneDirectionTo(node2);
		}
        else
        if (entry->direction == "<-")
        {
            node2->connectInOneDirectionTo(node1);
        }
        else
        if (entry->direction == "<>")
        {
			node1->connectInBothDirectionsTo(node2);
		}
    }

    return nodes;
}

int main()
{
    vector<StringEntry*> stringEntries = getStringEntriesFromTxt("Graph.txt");
    vector<Node*> nodes = getNodesFromStringEntries(stringEntries);

    removeNodesWithEdgeCount(nodes, 3); // MODIFIES VECTOR

    // TODO? COULD CHECK AGAIN IF THERE ARE NO NODES WITH 3 EDGES, BUT NOT NECESSARY FOR THIS DATA
    
    // PRINT RESULT
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
            cout <<otherNode->label + dirString + node->label + "\n";

            if (otherEdge->edgeType == EdgeType::Bidirectional)
            {
                // REMOVE OTHER BIDIRECTIONAL EDGE, SO WE DON'T PRINT IT A SECOND TIME
                spliceVectorAtIndex(otherNode->edges, otherEdge->nodeConnectionIndex);
            }
        }
    }

    // CLEANUP
    for (StringEntry* entry : stringEntries)
    {
        delete entry;
    }
    stringEntries.clear();

    for (Node* node : nodes)
    {
        node->clean();
        delete node;
    }
    nodes.clear();

    return 0;
}
