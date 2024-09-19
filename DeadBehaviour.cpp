#include "DeadBehaviour.h"
#include "Agent.h"

void DeadBehaviour::Update(Agent* agent, float deltaTime)
{

}

void DeadBehaviour::Enter(Agent* agent)
{
	agent->SetColour({ 127, 127, 127, 255 });
	agent->Reset();
}

void DeadBehaviour::Exit(Agent* agent)
{

}

float DeadBehaviour::Evaluate(Agent* agent)
{
	if (agent->GetHP() <= 0) return 10000.0f;
	else return 0.0f;
}
