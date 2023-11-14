#pragma once

#include "../Utils/MyStack.h"
#include "Graph.h"

struct Config{
    std::vector<std::vector<int>> input;
    std::vector<std::vector<int>> output;
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
    uint GetSize() const;

    /* Remove/Add connections */
    int Connect(uint pe);
    int Disconnect(uint pe);

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
    int mCost;
};