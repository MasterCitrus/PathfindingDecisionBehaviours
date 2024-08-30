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
