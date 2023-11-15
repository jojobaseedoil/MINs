#include "SimulatedAnnealing.h"
#include "../Utils/Random.h"

/* CONSTRUCTOR AND DESTRUCTOR */

/* Initial Temperature, Max. Temperature, Min. Temperature */
SimulatedAnnealing::SimulatedAnnealing(float TMax, float TMin):
    mTemperature(TMax),
    mMaxTemperature(TMax),
    mMinTemperature(TMin)
{
    Random::Init();
}

/* PUBLIC METHODS */
int SimulatedAnnealing::Execute(Architecture *CGRA){

    // std::cout << "\n\nANNEALING\n";

    int CurrentCost = CGRA->GetCost();
    int BestCost = CurrentCost;
    int objective = CGRA->GetGraph()->GetNumberOfEdges();

    int cnt1=0, cnt2=0, cnt3=0;

    while( !StopCriterion() ){
        
        /* Get 2 random PEs.
        This ensures that no invalid PEs will be selected */
        uint pe0 = Random::GetUIntRange(0, CGRA->GetSize()-1);
        uint pe1 = Random::GetUIntRange(0, CGRA->GetSize()-1);

        int NewCost = Evaluate(CGRA, CurrentCost, pe0, pe1);
        int DeltaCost = NewCost - CurrentCost;

        float rng = Random::GetFloat();

        if( DeltaCost > 0 || rng < AcceptanceProbability(DeltaCost) ){
            CurrentCost = NewCost;
            CGRA->ClearStack();
        } else{
            CGRA->GetBackToPreviousState(pe0, pe1);
        }

        UpdateTemperature();

        BestCost = (NewCost > BestCost ? NewCost : BestCost); // saves the best solution
        // check if found best...
        if(BestCost == objective) 
            return BestCost; 

        /* DEBUG STATISTIC */
        // std::cout << mTemperature << " " << CurrentCost << " " << BestCost << "\n";
        if(DeltaCost < 0)
            cnt1++;
        else if(DeltaCost == 0)
            cnt2++;
        else
            cnt3++;
        
        /* DEBUG STATISTIC */
    }

    std::cout << "piorou " << cnt1 << "\n";
    std::cout << "não mudou " << cnt2 << "\n";
    std::cout << "melhorou " << cnt3 << "\n"; 
    
    return BestCost;
}

int SimulatedAnnealing::InitialSolution(Architecture *CGRA){

    int cost = 0;
    int pe = CGRA->GetSize()-1;

    do{
        cost += CGRA->Connect(pe);
        CGRA->ClearStack();
    } while(pe--);

    return cost;
}

/* PRIVATE METHODS */
int SimulatedAnnealing::Evaluate(Architecture *CGRA, int cost, uint pe0, uint pe1){

    /* Calculate new amount of edges routed */
    cost -= CGRA->Disconnect(pe0);
    cost -= CGRA->Disconnect(pe1);
    CGRA->Swap(pe0, pe1);
    cost += CGRA->Connect(pe0);
    cost += CGRA->Connect(pe1);

    /* Calculate new graph fitness */

    return cost; /*CurrentCost/(K+1)*/ 
}