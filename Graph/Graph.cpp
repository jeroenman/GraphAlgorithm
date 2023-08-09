#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

const int SIZE = 5;

struct node
{
    node(string _tag) {
        tag = _tag;
    }

    string tag = "";

    vector<node*> edges;

    int getEdgeCount() const { return edges.size(); }

    void connectInOneDirectionTo(node* node)
    {
        node->edges.push_back(this);
    }

    void connectInBothDirectionsTo(node* node)
    {
        connectInOneDirectionTo(node);
        node->connectInOneDirectionTo(this);
    }
};

void removeWithEdgeCount(vector<node*>& nodes, int edgeCount)
{
    int arrSize = nodes.size();

    for (int i = arrSize - 1; i >= 0; i--)
    {
        node* currentNode = nodes[i];

        // FIND NODE THAT MATCHES EDGE COUNT TARGET
        int edgeCountMatchingNode = currentNode->getEdgeCount();
        if (edgeCountMatchingNode == edgeCount)
        {
            // LOOP THROUGH ALL OF NODE'S EDGES AND REMOVE EDGE IF IT IS NODE
            for (int j = edgeCountMatchingNode - 1; j >= 0; j--)
            {
                node* edge = currentNode->edges[j];
                int edgeCountCheck = edge->getEdgeCount();
                for (int k = edgeCountCheck - 1; k >= 0; k--)
                {
                    node* edge2 = edge->edges[k];
                    if (edge2 == currentNode)
                    {
                        edge->edges.erase(edge->edges.begin() + k);
                    }
                }
            }

            nodes.erase(nodes.begin() + i);
        }
    }
}

int main()
{
    node* nodeA = new node("A");
    node* nodeB = new node("B");
    node* nodeC = new node("C");
    node* nodeD = new node("D");
    node* nodeE = new node("E");
    node* nodeF = new node("F");
    node* nodeG = new node("G");

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

    vector<node*> allEdges = { nodeA, nodeB, nodeC, nodeD, nodeE, nodeF, nodeG };

    for (int i = 0; i < allEdges.size(); i++)
    {
        node* node = allEdges[i];
        cout << "\nNode" + node->tag + " edges: " << node->getEdgeCount();
    }

    removeWithEdgeCount(allEdges, 3); // MODIFIES VECTOR

    for (int i = 0; i < allEdges.size(); i++)
    {
        node* node = allEdges[i];
        cout << "\nNew Node" + node->tag + " edges: " << node->getEdgeCount();
    }

    return 0;
}
