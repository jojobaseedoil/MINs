#pragma once

#include "../Architecture.h"

class Heuristic{ /* This is the Heuristic interface */
public:
    virtual ~Heuristic();
    virtual void Execute(Architecture *CGRA) = 0;
    virtual int InitialSolution(Architecture *CGRA) = 0;
private:
    virtual int Evaluate(Architecture *CGRA) = 0;
};