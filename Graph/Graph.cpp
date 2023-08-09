#include <iostream>
#include <vector>
#include <unordered_map>

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
    }

    string label = "";

    vector<Edge*> edges;

    int getEdgeCount() const { return edges.size(); }

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
};

template <typename T>
void spliceVectorAtIndex(std::vector<T>& vec, int index)
{
    vec.erase(vec.begin() + index);
}

void removeWithEdgeCount(vector<Node*>& nodes, int edgeCount)
{
    int arrSize = nodes.size();

    for (int i = arrSize - 1; i >= 0; i--)
    {
        Node* node = nodes[i];

        // FIND NODE THAT MATCHES EDGE COUNT TARGET
        int edgeCountMatchingNode = node->getEdgeCount();
        if (edgeCountMatchingNode == edgeCount)
        {
            // LOOP THROUGH ALL OF NODE'S EDGES AND REMOVE EDGE IF IT IS NODE
            for (int j = edgeCountMatchingNode - 1; j >= 0; j--)
            {
                Edge* edge = node->edges[j];
                if (edge->edgeType == EdgeType::Bidirectional)
                {
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

int main()
{
    Node* nodeA = new Node("A");
    Node* nodeB = new Node("B");
    Node* nodeC = new Node("C");
    Node* nodeD = new Node("D");
    Node* nodeE = new Node("E");
    Node* nodeF = new Node("F");
    Node* nodeG = new Node("G");

    nodeA->connectInBothDirectionsTo(nodeB);
    nodeB->connectInOneDirectionTo(nodeC);
    nodeE->connectInOneDirectionTo(nodeA);
    nodeB->connectInOneDirectionTo(nodeE);
    nodeD->connectInOneDirectionTo(nodeE);
    nodeG->connectInOneDirectionTo(nodeE);
    nodeD->connectInBothDirectionsTo(nodeG);
    nodeF->connectInOneDirectionTo(nodeG);
    nodeF->connectInOneDirectionTo(nodeD);
    nodeF->connectInOneDirectionTo(nodeA);
    nodeF->connectInOneDirectionTo(nodeC);

    vector<Node*> allEdges = { nodeA, nodeB, nodeC, nodeD, nodeE, nodeF, nodeG };

    for (int i = 0; i < allEdges.size(); i++)
    {
        Node* node = allEdges[i];
        cout << "\nNode" + node->label + " edges: " << node->getEdgeCount();
    }

    removeWithEdgeCount(allEdges, 3); // MODIFIES VECTOR
    
    // COULD CHECK AGAIN IF THERE ARE NO NODES WITH 3 EDGES, BUT NOT NECESSARY FOR THIS DATA

    for (int i = 0; i < allEdges.size(); i++)
    {
        Node* node = allEdges[i];

        vector<Edge*> edges = node->edges;
        int n = edges.size();
        for (int j = 0; j < n; j++)
        {
            Edge* otherEdge = edges[j];
            Node* otherNode = otherEdge->nodeConnection;
            string dirString = getDirectionStringFromEdgeType(otherEdge->edgeType);
            cout << "\n" + otherNode->label + dirString + node->label;

            if (otherEdge->edgeType == EdgeType::Bidirectional)
            {
                // REMOVE OTHER BIDIRECTIONAL EDGE, SO WE DON'T PRINT IT A SECOND TIME
				spliceVectorAtIndex(otherNode->edges, otherEdge->nodeConnectionIndex);
			}
        }

    }

    return 0;
}
