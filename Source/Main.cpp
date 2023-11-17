#include "Heuristics/SimulatedAnnealing.h"
#include "Heuristics/LocalSearch.h"

const uint N = 256;
const uint STAGE = 4;
const uint EXTRA = 0;

int main(int argc, char **argv){

	srand(time(NULL));
	
	/* Load Parameters */
	std::string JsonConfig = "../Misc/Architectures/A3.json";
	std::string DotGraph   = "../Misc/Graphs/7_Synthetic_1_Mults1.dot";
	
	/* Start Architecture */
	Architecture CGRA;
	bool Success = CGRA.Init(JsonConfig,DotGraph,N,STAGE,EXTRA);
	if(Success){
		SimulatedAnnealing sa;
		LocalSearch ls;

		CGRA.SetCost( sa.InitialSolution(&CGRA) );
		std::cout << CGRA.GetCost() << "/" << CGRA.GetGraph()->GetNumberOfEdges() << "\n";
		CGRA.SetCost( sa.Execute(&CGRA) );
		std::cout << CGRA.GetCost() << "/" << CGRA.GetGraph()->GetNumberOfEdges() << "\n";
		CGRA.SetCost( ls.Execute(&CGRA) ); 
		std::cout << CGRA.GetCost() << "/" << CGRA.GetGraph()->GetNumberOfEdges() << "\n";
	}

	return 0;
}