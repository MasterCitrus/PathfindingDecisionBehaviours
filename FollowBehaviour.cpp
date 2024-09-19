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
	agent->SetColour({ 255, 127, 0, 255 });
	agent->Reset();
}

void FollowBehaviour::Exit(Agent* agent)
{
	agent->Reset();
}

float FollowBehaviour::Evaluate(Agent* agent)
{
	Agent* target = agent->GetTarget();
	float distance = glm::distance(target->GetPosition(), agent->GetPosition());

	float eval = 5 * agent->GetNodeMap()->GetCellSize() - distance;
	if (target->GetHP() <= 0) eval = 0;
	if (eval < 0) eval = 0;
	return eval;
}
