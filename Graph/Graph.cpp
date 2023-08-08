#include <iostream>
#include <vector>

using namespace std;

const int SIZE = 5;

struct node
{
    vector<node*> edges;

    int getEdgeCount() const { return edges.size(); }

    void connectInOneDirectionTo(node* node)
    {
        edges.push_back(node);
    }

    void connectInBothDirectionsTo(node* node)
    {
        connectInOneDirectionTo(node);
        node->connectInOneDirectionTo(this);
    }
};

int main()
{
    node* nodeA = new node();
    node* nodeB = new node();
    node* nodeC = new node();
    node* nodeD = new node();
    node* nodeE = new node();
    node* nodeF = new node();
    node* nodeG = new node();

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

    cout << "\nNodeA eges: " << nodeA->getEdgeCount();
    cout << "\nNodeB eges: " << nodeB->getEdgeCount();
    cout << "\nNodeC eges: " << nodeC->getEdgeCount();

    return 0;
}