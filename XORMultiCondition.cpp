#include "XORMultiCondition.h"

XORMultiCondition::~XORMultiCondition()
{
    //delete condition;
    //delete condition2;
}

bool XORMultiCondition::IsTrue(Agent* agent)
{
    bool result1 = condition->IsTrue(agent);
    bool result2 = condition2->IsTrue(agent);
    return result1 == true && result2 == false;
}
