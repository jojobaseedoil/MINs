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

    int Optimum = CGRA->GetGraph()->GetNumberOfEdges();
    int CurrentCost = CGRA->GetCost();

    while( !StopCriterion() ){
        
        if(CurrentCost == Optimum)
            break;

        /* Get 2 random PEs.
        This ensures that no invalid PEs will be selected.
        can select 2 empty pes, fix that later if needed */
        uint pe0 = Random::GetUIntRange(0, CGRA->GetSize()-1);
        uint pe1 = Random::GetUIntRange(0, CGRA->GetSize()-1);

        int NewCost = Evaluate(CGRA, CurrentCost, pe0, pe1); // swap two nodes
        int DeltaCost = NewCost - CurrentCost;

        float rng = Random::GetFloat();

        if( DeltaCost > 0 || rng < AcceptanceProbability(DeltaCost) ){
            CurrentCost = NewCost;
            CGRA->ClearStack();
        } else{
            CGRA->GetBackToPreviousState(pe0, pe1);
        }

        UpdateTemperature();
    }

    return  CurrentCost;
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

    return cost;
}