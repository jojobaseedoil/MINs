#include "LocalSearch.h"

LocalSearch::LocalSearch():
    mIsImproving(true)
{

}

int LocalSearch::Execute(Architecture *CGRA){

    int Optimum = CGRA->GetGraph()->GetNumberOfEdges();
    int CurrentCost = CGRA->GetCost();

    while( !StopCriterion() ){

        if(CurrentCost == Optimum)
            break;

        mIsImproving = false;

        for(int pe0=0; pe0<CGRA->GetSize(); pe0++){
            for(int pe1=pe0+1; pe1<CGRA->GetSize(); pe1++){
                
                int NewCost = Evaluate(CGRA, CurrentCost, pe0, pe1); // swap two nodes
                int DeltaCost = NewCost - CurrentCost;

                if( DeltaCost > 0 ){
                    mIsImproving = true;
                    CurrentCost = NewCost;
                    CGRA->ClearStack();
                } else {
                    CGRA->GetBackToPreviousState(pe0, pe1);
                }
            }
        }
    }

    return CurrentCost;
}