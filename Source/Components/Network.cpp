#include "Network.h"
#include "Architecture.h"

/* CONSTRUCTORS AND DESTRUCTOR */
Network::Network(Architecture *owner, uint size, uint stages, uint extras, uint radix):
	mOwner(owner),
	mSize(size),
	mStage(stages),
	mExtra(extras),
	mRadix(radix),
	mMask(size-1),
	mExtraPermutations( (1<<(2*extras)) )
{
	/* Allocate memory to the network */
	mNetwork = new int*[size];
	mMulticastConfig = new int*[size];
	for(int i=0; i<size; i++){
		mNetwork[i] = new int[ stages+extras ];
		mMulticastConfig[i] = new int[ stages+extras ];
	}
	/* Init. network */
	Clear();
}

Network::Network(){
	CreateNetwork();
}

Network::~Network(){
	DestroyNetwork();
}

/* PUBLIC METHODS */

/* Routing methods */
int Network::Route(uint input, uint output){
	
	/* Check valid path for each extra permutation */
	for(int extra=0; extra<mExtraPermutations; extra++){
		/* Select a possible path */
		bool FoundPath = true;
		int word = ConcatWord(input, extra, output);
		/* Verify if its a valid path */
		for(int j=0; j<mExtra+mStage; j++){
			int i = SlideWindow(word, j);
			if(mNetwork[i][j]){
				FoundPath = FoundPath && IsMulticast(word,i,j);
			}
		}
	
		if(FoundPath){
			InsertPath(word);
			return word;
		}
	}

	return ROUTE_FAILURE;
}

int Network::Route(Edge *edge){

	auto iport = mOwner->GetInput(edge->target->GetId());
	auto oport = mOwner->GetOutput(edge->source->GetId());
	
	/* Test all possible route configs */
	for(auto input : iport){
		for(auto output : oport){
			int word = this->Route(input, output);
			if(word >= 0){
				edge->label = word;
				return word;
			}
		}
	}
	return ROUTE_FAILURE;
}

int Network::Route(int word){
        
	/* Already has the path, 
	 * check if its a valid one */
	bool FoundPath = true;
	for(int j=0; j<mExtra+mStage; j++){
		int i = SlideWindow(word, j);
		if(mNetwork[i][j]){
			FoundPath = FoundPath && IsMulticast(word,i,j);
		}
	}

	if(FoundPath){
		InsertPath(word);
		return word;
	}

	return ROUTE_FAILURE;
}

int Network::UnRoute(int word){
	/* If is a possible path */
	if(word >= 0){
		/* un-route this path through all 
		 * network stages */
		for(int j=0; j<mStage+mExtra; j++){
			int i = SlideWindow(word, j);
			if(mNetwork[i][j] == 1)
				mMulticastConfig[i][j] = -1;
			mNetwork[i][j]--;
		}
		/* successfully un-routed path */
		return word;
	}
	return ROUTE_FAILURE;
}

/* Display network's current state */
void Network::Show() const{
	std::cout << "\t";
	for(int i=0; i<mStage+mExtra; i++)
		std::cout << i << " ";
	std::cout << "\n\n";
	for(int i=0; i<mSize; i++){
		std::cout << i << "\t";
		for(int j=0; j<mStage+mExtra; j++){
			std::cout << mNetwork[i][j] << " ";		
		} std::cout << "\n";
	}
}

/* Reset network state */
void Network::Clear(){
	for(int i=0; i<mSize; i++){
		for(int j=0; j<mStage+mExtra; j++){
			mNetwork[i][j] = 0;
			mMulticastConfig[i][j] = -1;
		}
	}
}

/* PRIVATE METHODS */

/* maintain network to a valid state */
void Network::CreateNetwork(){
	mNetwork = nullptr;
	mMulticastConfig = nullptr;
}

void Network::DestroyNetwork(){
	for(int i=0; i<mSize; i++){
		delete[] mNetwork[i];
		delete[] mMulticastConfig[i];
	}
	delete[] mNetwork;
	delete[] mMulticastConfig;
}

/* Route specific */
void Network::InsertPath(int word){
	/* mark path 'word' as done */
	#pragma omp parallel for
	for(int j=0; j<mStage+mExtra; j++){
		int i = SlideWindow(word, j);
		mNetwork[i][j]++; // allow multicast by adding +1  
		mMulticastConfig[i][j] = GetSwitchConfig(word, j);
	}
}