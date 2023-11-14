#include "Components/Architecture.h"

const uint N = 256;
const uint STAGE = 4;
const uint EXTRA = 0;

int main(int argc, char **argv){

	srand(time(NULL));
	
	/* Load Parameters */
	std::string JsonConfig = "../Misc/Architectures/A0.json";
	std::string DotGraph   = "../Misc/Graphs/debug.dot";
	
	/* Start Architecture */
	Architecture CGRA;
	bool Success = CGRA.Init(JsonConfig,DotGraph,N,STAGE,EXTRA);
	if(Success){
		CGRA.Connect(0);
		CGRA.Connect(1);
		CGRA.Connect(2);
		CGRA.Connect(3);

		CGRA.Disconnect(0);
		CGRA.Disconnect(1);
		CGRA.Disconnect(2);
		CGRA.Disconnect(3);
	}

	return 0;
}