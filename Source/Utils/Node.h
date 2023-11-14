#pragma once

#include <vector>
#include <iostream>

#define IN 1
#define OUT 2
#define UNVISITED (-1)

class Node;

struct Edge{
    Edge(Node *source, Node *target, uint id):
        source(source),
        target(target),
        label(UNVISITED),
        id(id)
    {

    }
    uint id;
    Node *source;
    Node *target;
    int label;
};

typedef std::vector<Edge*> EdgeList;

class Node{
public:
    Node(int id):
        mId(id)
    {
        mDegree[0] = mDegree[1] = mDegree[3] = 0;
    }
    inline void link(Edge *edge){
        mEdges.emplace_back(edge);
    }
    inline const EdgeList &GetEdges() const{
        return mEdges;
    }
    inline uint GetId() const{
        return mId;
    }
    inline int GetDegree(uint type=0){
        return mDegree[type];
    }

private:
    int mDegree[3];
    EdgeList mEdges;
    uint mId;
};