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
	agent->SetColour({ 0, 255, 255, 255 });
	agent->Reset();
}

void WanderBehaviour::Exit(Agent* agent)
{
	agent->Reset();
}

float WanderBehaviour::Evaluate(Agent* agent)
{
	Agent* target = agent->GetTarget();
	float distance = glm::distance(target->GetPosition(), agent->GetPosition());

	float eval = distance;
	if (eval < 0) eval = 0;
	return eval;
}
