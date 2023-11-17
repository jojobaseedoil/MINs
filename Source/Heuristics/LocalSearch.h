#pragma once

#include "SimulatedAnnealing.h"

class LocalSearch : public SimulatedAnnealing{
public:
    LocalSearch();
    int Execute(Architecture *CGRA) override;

private:
    inline bool StopCriterion() const override{
        return !mIsImproving;
    }

    bool mIsImproving;

};