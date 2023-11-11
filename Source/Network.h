#pragma once

#include <iostream>
#define ROUTE_FAILURE (-1)

class Network{ /* This is a Multistage Interconnection Network Representation */
public:	
	/* CONSTRUCTORS AND DESTRUCTOR */
	explicit Network(uint size, uint stages, uint extras, uint radix=4);
	Network();
	~Network();

	/* PUBLIC METHODS */
	
	/* Routing methods */
	int Route(uint input, uint output);
	int Route(int word);
	int UnRoute(int word);

	/* Display network's current state */
	void Show() const;

	/* Reset network state */
	void Clear();
private:
	/* PRIVATE METHODS */
	
	/* maintain network to a valid state */
	void CreateNetwork();
	void DestroyNetwork();
	
	/* Route specific */
	inline int ConcatWord(uint input, uint extra, uint output) const{
		return output | (input<<(2*(mStage+mExtra)) | (extra<<(2*mStage)));
	}
	inline int SlideWindow(int word, int column) const{
		return (word >> (2*(mStage+mExtra-column-1))) & mMask;
	}
	inline int GetSwitchConfig(int word, int column) const{
		return (word >> (2*(2*mStage+mExtra-column-1))) & 3;
	}
	inline bool IsMulticast(int word, int row, int column) const{
		return mMulticastConfig[row][column] == GetSwitchConfig(word, column);
	}
	void InsertPath(int word);

	/* PRIVATE ATTRIBUTES */
	int **mNetwork;
	int **mMulticastConfig;
	uint mSize, mStage, mExtra;
	uint mRadix, mMask, mExtraPermutations;
};
