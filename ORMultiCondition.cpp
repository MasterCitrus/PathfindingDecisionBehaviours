#include "ORMultiCondition.h"

bool ORMultiCondition::IsTrue(Agent* agent)
{
    bool test = false;
    for (auto& c : conditions)
    {
        test = c->IsTrue(agent);
        if (test == result) return true;
    }
    return false;
}
