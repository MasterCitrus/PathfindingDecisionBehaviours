#include "ANDMultiCondition.h"

bool ANDMultiCondition::IsTrue(Agent* agent)
{
    int trueCount = 0;
    bool test;
    for (auto& c : conditions)
    {
        test = c->IsTrue(agent);
        if (test != result) return false;
    }
    if (trueCount == conditions.size()) return true;
    else return false;
}
