#pragma once

#include "Graph.h"
#include "Helper.h"
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <unordered_set>

using namespace std;

struct NodeRelationship
{
    NodeRelationship(string _nodeFromLabel, string _direction, string _nodeToLabel)
    {
        nodeFromLabel = _nodeFromLabel;
        direction = _direction;
        nodeToLabel = _nodeToLabel;
    }

    bool equals(NodeRelationship* nodeRelationship)
    {
        if (direction == nodeRelationship->direction)
        {
            if (nodeFromLabel == nodeRelationship->nodeFromLabel && nodeToLabel == nodeRelationship->nodeToLabel)
            {
                return true;
            }

            if (nodeToLabel == nodeRelationship->nodeFromLabel && nodeFromLabel == nodeRelationship->nodeToLabel)
            {
                return true;
            }
        }
          
        return false;
    }

    string toString()
    {
		return nodeFromLabel + direction + nodeToLabel;
	}

    string nodeFromLabel;
    string nodeToLabel;
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
        edgesIn.reserve(3);
        edgesOut.reserve(3);
    }

    string label = "";

    unordered_set<Edge*> edgesIn;
    unordered_set<Edge*> edgesOut;

    int getEdgeCount() const { return static_cast<int>(edgesIn.size() + edgesOut.size()); }
    int getEdgeCountIn() const { return static_cast<int>(edgesIn.size()); }
    int getEdgeCountOut() const { return static_cast<int>(edgesOut.size()); }

    void addEdgeIn(Edge* edge)
    {
        edgesIn.insert(edge);
    }

    void addEdgeOut(Edge* edge)
    {
        edgesOut.insert(edge);
    }

    void removeEdgeIn(Edge* edgeIn)
    {
        // REMOVE EDGE FROM OTHER NODE
        edgeIn->nodeFrom->edgesOut.erase(edgeIn);

        // REMOVE EDGE FROM SELF
        edgesIn.erase(edgeIn);

        // FINALLY, DELETE EDGE
        delete edgeIn;
    }

    void removeEdgeOut(Edge* edgeOut)
    {
        // REMOVE EDGE FROM OTHER NODE
        edgeOut->nodeTo->edgesIn.erase(edgeOut);

        // REMOVE EDGE FROM SELF
        edgesOut.erase(edgeOut);

        // FINALLY, DELETE EDGE
        delete edgeOut;
    }

    void clearEdges()
    {
        // REMOVE EDGES IN
        std::vector<Edge*> toRemoveIn;
        for (Edge* edgeIn : edgesIn)
        {
            toRemoveIn.push_back(edgeIn);
        }
        for (Edge* edgeIn : toRemoveIn)
        {
            removeEdgeIn(edgeIn);
        }

        // REMOVE EDGES OUT
        std::vector<Edge*> toRemoveOut;
        for (Edge* edgeOut : edgesOut)
        {
            toRemoveOut.push_back(edgeOut);
        }
        for (Edge* edgeOut : toRemoveOut)
        {
            removeEdgeOut(edgeOut);
        }
    }
};

class Graph
{
    public:
        Graph(string txtFilePath);

        Node* addNode(string label = "");
        void removeNode(Node* node);
        void removeNodesWithEdgeCount(int edgeCount);
        Edge* addEdge(Node* nodeFrom, Node* nodeTo);
        void removeEdge(Edge* edge);
        void printNodeRelationships();
        void clean();

    private:
        vector<NodeRelationship*> createNodeRelationships(string txtPath);
        void setupNodes(vector<NodeRelationship*> nodeRelationships);
        vector <Node*> getNodesWitIncomingNumberOfEdges(int edgeCount);
        vector<NodeRelationship*> getNodeRelationships();
        string getStringOfNodeRelationships();

        unordered_set<Node*> nodes;
};
