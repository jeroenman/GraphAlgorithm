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

    bool equals(NodeRelationship* nodeRelationship)
    {
        if (direction == nodeRelationship->direction)
        {
            if (node1Label == nodeRelationship->node1Label && node2Label == nodeRelationship->node2Label)
            {
                return true;
            }

            if (node2Label == nodeRelationship->node1Label && node1Label == nodeRelationship->node2Label)
            {
                return true;
            }
        }
          
        return false;
    }

    string toString()
    {
		return node1Label + direction + node2Label;
	}

    string node1Label;
    string node2Label;
    string direction;
};

struct Node;

struct Edge
{
    Edge(Node* _nodeFrom, Node* _nodeTo)
    {
        nodeFrom = _nodeFrom;
        nodeTo = _nodeTo;
    }

    Node* nodeFrom = nullptr;
    Node* nodeTo = nullptr;
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

    vector<Edge*> getEdgesIn() const
    {
        vector<Edge*> edgesIn;

        for (Edge* edge : edges)
        {
            if (edge->nodeTo == this)
            {
                edgesIn.push_back(edge);
            }
        }

        return edgesIn;
    }

    vector<Edge*> getEdgesOut() const
    {
        vector<Edge*> edgesIn;

        for (Edge* edge : edges)
        {
            if (edge->nodeFrom == this)
            {
                edgesIn.push_back(edge);
            }
        }

        return edgesIn;
    }

    int getEdgeCount() const { return static_cast<int>(edges.size()); }
    int getEdgeCountIn() const { return static_cast<int>(getEdgesIn().size()); }
    int getEdgeCountOut() const { return static_cast<int>(getEdgesOut().size()); }

    void addEdge(Edge* edge)
    {
        edges.push_back(edge);
    }

    void removeEdge(Edge* edge)
    {
        int i = getIndexOfElementInVector(edges, edge);
        spliceVectorAtIndex(edges, i);
    }

    void removeEdgeAtIndex(int index)
    {
        spliceVectorAtIndex(edges, index);
    }

    void clearEdges()
    {
        edges.clear();
    }
};

class Graph
{
    public:
        Graph(string txtFilePath);

        Node* addNode(string label = "");
        void removeNode(Node* node, int index);
        void removeNodesWithEdgeCount(int edgeCount);
        Edge* addEdge(Node* nodeFrom, Node* nodeTo);
        void removeEdge(Edge* edge);
        void printNodeRelationships();
        void clean();

    private:
        vector<NodeRelationship*> createNodeRelationships(string txtPath);
        void setupNodes(vector<NodeRelationship*> nodeRelationships);
        void removeEdgesFromNodeAndConnections(Node* node);
        vector <Node*> getNodesWithEdgeCount(int edgeCount);
        string getStringOfNodeRelationships();

        vector<Node*> nodes;
};
