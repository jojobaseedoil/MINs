#pragma once

#include <vector>
#include <iostream>
#include "../Utils/Node.h"

typedef std::vector<Node*> NodeList;

class Graph{
public:
    /* CONSTRUCTOR AND DESTRUCTOR */
    Graph();
    ~Graph();

    /* PUBLIC METHODS */

    /* Get Node/Edge list */
    const NodeList &GetNodes() const;
    const EdgeList &GetEdges() const;

    /* Insert/remove Node/Edges */
    void AddNode(Node *node);
    void AddEdge(Node *source, Node *target);

    /* Get graph parameters */
    uint GetNumberOfNodes() const;
    uint GetNumberOfEdges() const;

    /* Get adjacency nodes */
    const EdgeList &GetEdges(Node *node) const;

private:
    /* PRIVATE ATTRIBUTES */
    NodeList mNodes;
    EdgeList mEdges;
    uint mNumberOfNodes;
    uint mNumberOfEdges;
};








