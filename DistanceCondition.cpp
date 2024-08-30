#include "DistanceCondition.h"
#include "glm/glm.hpp"
#include "Agent.h"

bool DistanceCondition::IsTrue(Agent* agent)
{
    return (glm::distance(agent->GetPosition(), agent->GetTarget()->GetPosition()) < m_distance) == m_lessThan;
}
