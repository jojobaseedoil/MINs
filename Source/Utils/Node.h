#pragma once

#include <vector>
#include <iostream>

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
        mId(id),
        mPe(-1)
    {
        mDegree[0] = 0; // degree
        mDegree[1] = 0; // in degree
        mDegree[2] = 0; // out degree
    }
    inline void link(Edge *edge){ /* increment node degree here */
        mDegree[0]++;        
        mEdges.emplace_back(edge);
    }
    inline const EdgeList &GetEdges() const{
        return mEdges;
    }
    inline uint GetId() const{
        return mId;
    }
    inline uint GetPe() const{
        return mPe;
    }
    inline void SetPe(uint pe){
        mPe = pe;
    }
    inline int GetDegree(){
        return mDegree[0];
    }
    inline int GetInDegree(){
        return mDegree[1];
    }
    inline int GetOutDegree(){
        return mDegree[2];
    }

private:
    int mDegree[3];
    EdgeList mEdges;
    uint mId;
    uint mPe;
};