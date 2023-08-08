#include <iostream> 

using namespace std;

const int SIZE = 5;

struct node
{
    node* edges[SIZE] = {};
    int size = 0;

    void connectInOneDirectionTo(node* node)
    {
        edges[size] = node;
        size++;
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

    cout << "\nNodeA eges: " << nodeA->size;
    cout << "\nNodeB eges: " << nodeB->size;
    cout << "\nNodeC eges: " << nodeC->size;

    return 0;
}
