#pragma once

#include "../Utils/MyStack.h"
#include "Graph.h"

struct Config{
    std::vector<std::vector<int>> input;
    std::vector<std::vector<int>> output;
    std::vector<int> degree;
    uint pes;
};

class Network;

class Architecture{
public:
    /* CONSTRUCTORS AND DESTRUCTOR */
    Architecture();
    ~Architecture();

    /* PUBLIC METHODS */

    /* Start CGRA */
    bool Init(
        const std::string&JsonConfig,
        const std::string&DotGraph,
        uint N, uint STAGE, uint EXTRA, uint RADIX=4
    );

    /* access PE */
    Node* operator[](int index) const;

    /* Get a PE config */
    const std::vector<int> &GetInput(uint pe) const;
    const std::vector<int> &GetOutput(uint pe) const;

    /* Get number of PEs */
    inline uint GetSize() const{
        return mPort->pes;
    }

    /* Remove/Add connections */
    int Connect(uint pe);
    int Disconnect(uint pe);

    /* Swap two nodes inside pe0 and pe1 */
    inline void Swap(uint pe0, uint pe1){
        if(mPe2Node[pe0] != nullptr) mPe2Node[pe0]->SetPe(pe1);
        if(mPe2Node[pe1] != nullptr) mPe2Node[pe1]->SetPe(pe0);
        std::swap(mPe2Node[pe0],mPe2Node[pe1]);
    }
    /* undo swap */
    void GetBackToPreviousState(uint pe0, uint pe1);

    /* Get/Set CGRA solution cost */
    inline int GetCost() const{
        return mCost;
    }
    inline void SetCost(const int cost){
        mCost = cost;
    }
    inline int GetDegree(uint pe) const{
        return mPort->degree[pe];
    }
    inline int GetFitness() const{
        return mFitness;
    }

    /* Get Graph */
    inline const Graph *GetGraph() const{
        return mDFG;
    }

    /* Clear stack */
    inline void ClearStack(){
        mStack.clear();
    }

private:
    /* PRIVATE METHODS */
    void CreateArchitecture();
    void DestroyArchitecture();

    /* PRIVATE ATTRIBUTES */
    
    /* data */
    MyStack<int64_t> mStack;
    Network *mOmega;
    Graph *mDFG;    // data flow graph
    Config *mPort;
    NodeList mPe2Node;
    int mCost, mFitness;
};