#include "FollowBehaviour.h"
#include "Agent.h"

void FollowBehaviour::Update(Agent* agent, float deltaTime)
{
	Agent* target = agent->GetTarget();

	float distance = glm::distance(target->GetPosition(), lastTargetPosition);

	if (distance > agent->GetNodeMap()->GetCellSize())
	{
		lastTargetPosition = target->GetPosition();
		agent->GoTo(lastTargetPosition);
	}
}

void FollowBehaviour::Enter(Agent* agent)
{
	agent->SetColour({ 255, 0, 0, 255 });
	agent->Reset();
}
