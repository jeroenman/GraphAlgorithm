#pragma once

#include "NodesManager.h"
#include "Helper.h"
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>

using namespace std;

struct NodeRelationship
{
    NodeRelationship(string _node1Label, string _direction, string _node2Label)
    {
        node1Label = _node1Label;
        direction = _direction;
        node2Label = _node2Label;
    }

    string node1Label;
    string node2Label;
    string direction;
};

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

    void receiveEdgeFrom(Node* node, EdgeType edgeType = Directional)
    {
        Edge* edge = new Edge(node, getEdgeCount(), edgeType);
        edges.push_back(edge);
    }

    void removeEdgeAtIndex(int index)
    {
        Edge* edge = edges[index];
        delete edge;

        spliceVectorAtIndex(edges, index);
    }

    void clean()
    {
        int edgesLength = static_cast<int>(edges.size());
        for (int i = edgesLength - 1; i >= 0; i--)
        {
            removeEdgeAtIndex(i);
        }
    }
};

class NodesManager
{
    public:
        NodesManager(string txtFilePath);

        void removeNodesWithEdgeCount(int edgeCount);
        void printNodeRelationships();
        void clean();

    private:
        vector<NodeRelationship*> createNodeRelationships(string txtPath);
        void setupNodes(vector<NodeRelationship*> nodeRelationships);
        Node* addNode(string label = "");
        void removeNode(Node* node, int index);
        string getStringOfNodeRelationships();
        string getDirectionStringFromEdgeType(EdgeType edgeType);

        vector<Node*> nodes;
};
