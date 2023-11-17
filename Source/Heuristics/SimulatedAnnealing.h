#pragma once

#include "Heuristic.h"
#include <cmath>

#define DECAY 0.9999
#define MAX_TEMPERATURE 0.001
#define MIN_TEMPERATURE 0.0001

class SimulatedAnnealing : public Heuristic{
public:
    /* CONSTRUCTOR AND DESTRUCTOR */

    /* Initial Temperature, Max. Temperature, Min. Temperature */
    SimulatedAnnealing(float TMax=MAX_TEMPERATURE, float TMin=MIN_TEMPERATURE);
    
    /* PUBLIC METHODS */
    int Execute(Architecture *CGRA) override;
    int InitialSolution(Architecture *CGRA) override;
protected:
    /* PROTECTED METHODS */
    int Evaluate(Architecture *CGRA, int cost, uint pe0, uint pe1);

private:
    /* PRIVATE METHODS */
    inline bool StopCriterion() const override{
        return mTemperature < mMinTemperature;
    }
    inline void UpdateTemperature(){
        mTemperature *= DECAY;
    }
    inline double AcceptanceProbability(int DeltaCost){
        return exp(DeltaCost/mTemperature);
    }

    /* PRIVATE ATTRIBUTES */
    float mTemperature;
    float mMaxTemperature;
    float mMinTemperature;
};