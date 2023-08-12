#pragma once

#include "Graph.h"
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
    Edge(Node* _nodeFrom, Node* _nodeTo, EdgeType _edgeType = Directional)
    {
        nodeFrom = _nodeFrom;
        nodeTo = _nodeTo;
        edgeType = _edgeType;
    }

    Node* nodeFrom = nullptr;
    Node* nodeTo = nullptr;
    EdgeType edgeType = Directional;
};

struct Node
{
    Node(string _label)
    {
        label = _label;
        edgesIn.reserve(3);
        edgesOut.reserve(3);
    }

    string label = "";

    vector<Edge*> edgesIn;
    vector<Edge*> edgesOut;

    int getEdgeCountIn() const { return static_cast<int>(edgesIn.size()); }
    int getEdgeCountOut() const { return static_cast<int>(edgesOut.size()); }

    void addEdgeFrom(Edge* edge)
    {
        edgesIn.push_back(edge);
    }

    void addEdgeTo(Edge* edge)
    {
        edgesOut.push_back(edge);
    }

    void removeEdge(Edge* edge)
    {
        if (edge->nodeFrom == this)
        {
            removeEdgeOut(edge);
		}
		else
        if (edge->nodeTo == this)
        {
            removeEdgeIn(edge);
        }
    }

    void removeEdgeOut(Edge* edge)
    {
        int i = getIndexOfElementInVector(edgesOut, edge);
        spliceVectorAtIndex(edgesOut, i);
    }

    void removeEdgeIn(Edge* edge)
    {
        int i = getIndexOfElementInVector(edgesIn, edge);
        spliceVectorAtIndex(edgesIn, i);
    }

    void removeEdgeInAtIndex(int index)
    {
        spliceVectorAtIndex(edgesIn, index);
    }

    void removeEdgeOutAtIndex(int index)
    {
        spliceVectorAtIndex(edgesOut, index);
    }

    void clean()
    {
        //return;
        int edgesLengthIn = static_cast<int>(edgesIn.size());
        for (int i = edgesLengthIn - 1; i >= 0; i--)
        {
            delete edgesIn[i];
        }

        int edgesLengthOut = static_cast<int>(edgesOut.size());
        for (int i = edgesLengthOut - 1; i >= 0; i--)
        {
            //delete edgesOut[i]; // PROBLEMS WITH BIDIRECTIONAL EDGES
        }

        edgesIn.clear();
        edgesOut.clear();
    }
};

class Graph
{
    public:
        Graph(string txtFilePath);

        Edge* addEdge(Node* nodeFrom, Node* nodeTo, EdgeType edgeType = Directional);
        void removeEdge(Edge* edge);
        void removeNodesWithEdgeCount(int edgeCount);
        void printNodeRelationships();
        void clean();

    private:
        vector<NodeRelationship*> createNodeRelationships(string txtPath);
        void setupNodes(vector<NodeRelationship*> nodeRelationships);
        void connectBidirectionalEdgeToNodes(Node* node1, Node* node2);
        Node* addNode(string label = "");
        void removeNode(Node* node, int index);
        void removeEdgesPointingToNode(Node* node);
        vector <Node*> getNodesWithEdgeCount(int edgeCount);
        string getStringOfNodeRelationships();
        string getDirectionStringFromEdgeType(EdgeType edgeType);

        vector<Node*> nodes;
};
