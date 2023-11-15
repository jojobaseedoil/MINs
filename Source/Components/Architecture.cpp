#include "Architecture.h"
#include "../Utils/DataLoader.h"
#include "Network.h"


/* CONSTRUCTORS AND DESTRUCTOR */
Architecture::Architecture():
    mStack(1024),
    mCost(0)
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

    /* Init solution */
    mPe2Node = NodeList(mPort->pes, nullptr);
    int i=0;
    for(auto node : mDFG->GetNodes()){
        mPe2Node[i++] = node;
    }
    std::random_shuffle(mPe2Node.begin(), mPe2Node.end());
    for(int j=0; j<GetSize(); j++){
        Node *node = mPe2Node[j];
        if(node != nullptr){
            node->SetPe(j);
        }
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

/* Remove/Add connections */
int Architecture::Connect(uint pe){
    // std::cout << "connecting node: ";
    if(mPe2Node[pe] == nullptr){
        // std::cout << "empty\n";
        return 0;
    }

    // std::cout << mPe2Node[pe]->GetId() << "\n";

    int added = 0;
    Node *node = mPe2Node[pe];

    for(auto edge : mDFG->GetEdges(node)){

        if( edge->label == UNVISITED && mOmega->Route(edge) != ROUTE_FAILURE ){
            int64_t item = edge->label | (static_cast<int64_t>(edge->id) << 24) | 
                                         (static_cast<int64_t>(0) << 32);
            // std::cout << "\t" << item << " ( " << edge->label << ", " << edge->id << ", " << 0 << " )\n";
            mStack.push(item);
            added++;
        }
    }

    return added;
}

int Architecture::Disconnect(uint pe){
    // std::cout << "disconnecting node: ";
    if(mPe2Node[pe] == nullptr){
        // std::cout << "empty\n";
        return 0;
    }

    // std::cout << mPe2Node[pe]->GetId() << "\n";

    int removed = 0;
    Node *node = mPe2Node[pe];

    for(auto edge : mDFG->GetEdges(node)){

        int word = edge->label;
        if(mOmega->UnRoute(word) != ROUTE_FAILURE){
            int64_t item = word | (static_cast<int64_t>(edge->id) << 24) | 
                                  (static_cast<int64_t>(1) << 32);
            // std::cout << "\t" << item << " ( " << word << ", " << edge->id << ", " << 1 << " )\n";
            edge->label = UNVISITED;
            mStack.push(item);
            removed++;
        }
    }

    return removed;
}

void Architecture::GetBackToPreviousState(uint pe0, uint pe1){

    while(!mStack.empty()){

        int64_t item = mStack.top();
        mStack.pop();

        int word = item & 0xFFFFFF;
        int id   = (item >> 24) & 0xFF;
        int v    = (item >> 32) & 0x1;
        Edge *edge = mDFG->GetEdges()[id];
    
        if(v){
            // std::cout << "connnected " << item << "\n";
            mOmega->Route(word);
            edge->label = word;
        } else{
            // std::cout << "disconnnected " << item << "\n";
            mOmega->UnRoute(word);
            edge->label = UNVISITED;
        }
    }

    Swap(pe0, pe1);
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
