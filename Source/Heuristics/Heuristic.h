#pragma once

#include "../Components/Architecture.h"

class Heuristic{ /* This is the Heuristic interface */
public:
    virtual ~Heuristic() = default;
    virtual int Execute(Architecture *CGRA) = 0;
    virtual int InitialSolution(Architecture *CGRA) = 0;
private:
    virtual bool StopCriterion() const = 0;
    // virtual int Evaluate(Architecture *CGRA);
};