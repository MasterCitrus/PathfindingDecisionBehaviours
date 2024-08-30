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

float FollowBehaviour::Evaluate(Agent* agent)
{
	Agent* target = agent->GetTarget();
	float distance = glm::distance(target->GetPosition(), agent->GetPosition());

	float eval = 10 * agent->GetNodeMap()->GetCellSize() - distance;
	if (eval < 0) eval = 0;
	return eval;
}
