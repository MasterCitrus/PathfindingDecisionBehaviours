#include "WanderBehaviour.h"
#include "Agent.h"

void WanderBehaviour::Update(Agent* agent, float deltaTime)
{
	if (agent->PathComplete())
	{
		agent->GoTo(agent->GetNodeMap()->GetRandomNode()->position);
	}
}

void WanderBehaviour::Enter(Agent* agent)
{
	agent->Reset();
	agent->SetColour({ 0, 255, 255, 255 });
}

void WanderBehaviour::Exit(Agent* agent)
{
	agent->Reset();
}

float WanderBehaviour::Evaluate(Agent* agent)
{
	Agent* target = agent->GetTarget();
	float distance = glm::distance(target->GetPosition(), agent->GetPosition());
	float eval = 0;

	if (target->GetHP() > 0)
	{
		eval = distance;
	}
	else eval = 1000;

	if (eval < 0) eval = 0;
	return eval;
}
