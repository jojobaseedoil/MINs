#include "Architecture.h"
#include "../Utils/DataLoader.h"
#include "Network.h"


/* CONSTRUCTORS AND DESTRUCTOR */
Architecture::Architecture():
    mStack(1024)
{
    CreateArchitecture();
}

Architecture::~Architecture(){
    DestroyArchitecture();
}

/* PUBLIC METHODS */

/* Init CGRA */
bool Architecture::Init(
    const std::string&JsonConfig,
    const std::string&DotGraph,
    uint N, uint STAGE, uint EXTRA, uint RADIX
){
    /* Set CGRA config based 
    on .json config file */
    mPort = parser::LoadConfig(JsonConfig);
    if(this == nullptr){
        std::cerr << "Fail to load Architecture\n";
        return false;
    }

    /* Set the Data Flow Graph */  
    mDFG = parser::LoadGraph(DotGraph);
    if(mDFG == nullptr){
        std::cerr << "Fail to load Graph.\n";
        return false;
    }

    /* Set the Multistage Interconnection Network */
    mOmega = new Network(this,N,STAGE,EXTRA);

    /* Init solution as empty */
    mPe2Node = NodeList(mPort->pes, nullptr);
    int i=0;
    for(auto node : mDFG->GetNodes()){
        mPe2Node[i++] = node;
    }
    
    return true;
}

/* access PE */
Node* Architecture::operator[](int index) const{
    return mPe2Node[index];
}

/* Get a PE config */
const std::vector<int> &Architecture::GetInput(uint pe) const{
    return mPort->input[pe];
}

const std::vector<int> &Architecture::GetOutput(uint pe) const{
    return mPort->output[pe];
}

/* Get number of PEs */
uint Architecture::GetSize() const{
    return mPort->pes;
}

/* Remove/Add connections */
int Architecture::Connect(uint pe){
    
    /* Remove this later... we'll make sure that 
    only valid pe's enter this function */
    if(pe >= mPort->pes || mPe2Node[pe] == nullptr){
        std::cerr << "there is no node to connect at position " << pe << ".\n";
        exit(EXIT_FAILURE);
    }

    int added = 0;
    Node *node = mPe2Node[pe];

    for(auto edge : mDFG->GetEdges(node)){

        if( edge->label == UNVISITED && mOmega->Route(edge) != ROUTE_FAILURE ){
            int64_t item = edge->label | (static_cast<int64_t>(edge->id) << 24) | 
                                         (static_cast<int64_t>(0) << 32);
            mStack.push(item);
            added++;
        }
    }

    return added;
}

int Architecture::Disconnect(uint pe){
    /* Remove this later... we'll make sure that 
    only valid pe's enter this function */
    if(pe >= mPort->pes || mPe2Node[pe] == nullptr){
        std::cerr << "there is no node to connect at position " << pe << ".\n";
        exit(EXIT_FAILURE);
    }

    int removed = 0;
    Node *node = mPe2Node[pe];

    for(auto edge : mDFG->GetEdges(node)){

        int word = edge->label;
        if(mOmega->UnRoute(word) != ROUTE_FAILURE){
            int64_t item = word | (static_cast<int64_t>(edge->id) << 24) | 
                                  (static_cast<int64_t>(1) << 32);
            edge->label = UNVISITED;
            mStack.push(item);
            removed++;
        }
    }

    return removed;
}

/* PRIVATE METHODS */
void Architecture::CreateArchitecture(){
    mOmega = nullptr;
    mDFG = nullptr;
    mPort = nullptr;
}

void Architecture::DestroyArchitecture(){
    delete mOmega;
    delete mDFG;
    delete mPort;
}
