#include "HealthCondition.h"
#include "Agent.h"

bool HealthCondition::IsTrue(Agent* agent)
{
    if (!self) return agent->GetTarget()->GetHP() <= hpLimit;
    else return agent->GetHP() <= hpLimit;
}
