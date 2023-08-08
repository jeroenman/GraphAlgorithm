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

    nodeA->connectInOneDirectionTo(nodeB);
    nodeA->connectInBothDirectionsTo(nodeC);

    cout << "\nNodeA eges: " << nodeA->size;
    cout << "\nNodeB eges: " << nodeB->size;
    cout << "\nNodeC eges: " << nodeC->size;

    return 0;
}
