#include "Graph.h"
#include <algorithm>
#include "../Utils/DataLoader.h"

/* CONSTRUCTOR AND DESTRUCTOR */
Graph::Graph():
    mNumberOfNodes(0),
    mNumberOfEdges(0)
{
    
}

Graph::~Graph(){
    for(auto node : mNodes)
        delete node;
    for(auto edge : mEdges)
        delete edge;
}

/* PUBLIC METHODS */

/* Get Node/Edge list */
const NodeList &Graph::GetNodes() const{
    return mNodes;
}

const EdgeList &Graph::GetEdges() const{
    return mEdges;
}

/* Insert/remove Node/Edges */
void Graph::AddNode(Node *node){
    mNodes.emplace_back(node);
    mNumberOfNodes++;
}

void Graph::AddEdge(Node *source, Node *target){
    Edge *edge = new Edge(source, target, mNumberOfEdges);
    source->link(edge);
    target->link(edge);
    mEdges.emplace_back(edge);
    mNumberOfEdges++;
}

/* Get graph parameters */
uint Graph::GetNumberOfNodes() const{
    return mNumberOfNodes;
}

uint Graph::GetNumberOfEdges() const{
    return mNumberOfEdges;
}

/* Get adjacency nodes */
const EdgeList &Graph::GetEdges(Node *node) const{
    return node->GetEdges();
}
